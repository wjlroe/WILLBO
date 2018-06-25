#include "minunit.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKENS 1024

char *WB_RESERVED_WORDS[] = {"int", NULL};

typedef struct StructNode {
  char *name;
} StructNode;

typedef enum NodeType {
  NODE_TOPLEVEL,
  NODE_STRUCT,
  NODE_IDENT,
  NODE_SYNTAX,
  NODE_RESERVED_WORD,
  NODE_WORD,
} NodeType;

typedef union {
  StructNode struct_node;
  char syntax_character;
  char *identifier;
  char *reserved_word;
  char *word;
} TokenValue;

typedef struct NodeToken {
  NodeType type;
  TokenValue value;
} NodeToken;

bool node_token_eq(NodeToken *left, NodeToken *right) {
  if (left->type != right->type) {
    return false;
  }

  // TODO: Make this aware of enum data in the union
  return true;
}

void die(const char *s) {
  perror(s);
  exit(1);
}

typedef struct TokenizerState {
  bool in_word;
  char buffer[1024];
  size_t buffer_size;
} TokenizerState;

TokenizerState state_from_token_stream(NodeToken tokens[], size_t num_tokens) {
  TokenizerState state = {};

  if (num_tokens > 0) {
    NodeToken last_token = tokens[num_tokens - 1];
    switch (last_token.type) {
      case NODE_WORD: {
        state.in_word = true;
      } break;
    }
  }

  return state;
}

void tokenize(NodeToken tokens[], size_t *num_tokens, const char *buffer,
              size_t buf_size) {
  TokenizerState state = state_from_token_stream(tokens, *num_tokens);

  char cur_char;
  for (size_t n = 0; n < buf_size; ++n) {
    cur_char = buffer[n];
    if (isalnum(cur_char) && (state.in_word || (isalpha(cur_char) || state.buffer_size > 0))) {
      state.buffer[state.buffer_size++] = cur_char;
      state.in_word = true;
    } else if (isblank(cur_char)) {
      if (state.buffer_size > 0 && state.in_word) {
        state.buffer[state.buffer_size] = '\0';

        // Is it a reserved word?
        for (int res_word_i = 0; WB_RESERVED_WORDS[res_word_i]; res_word_i++) {
          size_t res_size = strlen(WB_RESERVED_WORDS[res_word_i]);
          if (state.buffer_size == res_size) {
            if (!strncmp(state.buffer, WB_RESERVED_WORDS[res_word_i], res_size)) {
              NodeToken new_token = {};
              new_token.type = NODE_RESERVED_WORD;
              new_token.value.reserved_word = WB_RESERVED_WORDS[res_word_i];
              tokens[*num_tokens] = new_token;
              *num_tokens += 1;
              state.buffer_size = 0;
              state.in_word = false;
              break;
            }
          }
        }

        // Is it an identifier then?
         if (state.buffer_size > 0 && state.in_word) {
           NodeToken new_token = {};
           new_token.type = NODE_IDENT;
           new_token.value.identifier = malloc(state.buffer_size + 1);
           strncpy(new_token.value.identifier, state.buffer, state.buffer_size + 1);
           state.buffer_size = 0;
           state.in_word = false;
           tokens[*num_tokens] = new_token;
           *num_tokens += 1;
        }
      }
    }
  }
}

void tokenize_file(char *filename, NodeToken tokens[], size_t *num_tokens) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    die("fopen failed");
  }

  *num_tokens = 0;

  char buf[1024];
  size_t nread;
  while ((nread = fread(buf, 1, sizeof(buf), fp)) > 0) {
    tokenize(tokens, num_tokens, buf, nread);
  }

  fclose(fp);
}

test test_tokenize_file1(TestRun *test_run) {
  char *test_filename = ".\\examples\\autogenerated\\test1.wb";
  char *test_program = "int a = 4 + 2;\n";
  FILE *tmpfd;
  if ((tmpfd = fopen(test_filename, "w+")) == NULL) {
    printf("Cannot open a temp file!\n");
    mu_assert(false);
  }
  fputs(test_program, tmpfd);
  fflush(tmpfd);
  fclose(tmpfd);

  NodeToken expected[] = {
    {.type = NODE_RESERVED_WORD, .value = {.reserved_word = "int"}},
    {.type = NODE_IDENT, .value = {.identifier = "a"}}
  };
  size_t expected_len = sizeof(expected) / sizeof(expected[0]);
  NodeToken tokens[MAX_TOKENS];
  size_t token_length;
  tokenize_file(test_filename, tokens, &token_length);
  mu_assert_array_equal(node_token_eq, tokens, token_length, expected,
                        expected_len);

  remove(test_filename);
}

static void run_the_tests(TestRun *test_run) { test_tokenize_file1(test_run); }

RUN_TESTS(run_the_tests);
