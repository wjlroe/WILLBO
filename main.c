#include "minunit.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKENS 1024

typedef enum NodeType {
  NODE_TOPLEVEL,
  NODE_STRUCT,
  NODE_IDENT,
  NODE_SYNTAX,
  NODE_RESERVED_WORD,
  NODE_OPERATOR,
  NODE_NUMBER,
  NODE_WORD,
} NodeType;

typedef enum OperatorType {
  OP_EQ,
  OP_PLUS,
} OperatorType;

typedef union {
  char syntax_character;
  char *identifier;
  char *reserved_word;
  char *word;
  char *digits;
  OperatorType op_type;
} TokenValue;

typedef struct NodeToken {
  NodeType type;
  TokenValue value;
} NodeToken;

typedef struct OperatorMap {
  char *text;
  OperatorType type;
} OperatorMap;

OperatorMap WB_OPERATORS[] = {{"=", OP_EQ}, {"+", OP_PLUS}};
char *WB_RESERVED_WORDS[] = {"int", NULL};

bool node_token_eq(NodeToken *left, NodeToken *right) {
  if (left->type != right->type) {
    return false;
  }

  switch (left->type) {
  case NODE_IDENT:
    return !strcmp(left->value.identifier, right->value.identifier);
  case NODE_RESERVED_WORD:
    return !strcmp(left->value.reserved_word, right->value.reserved_word);
  case NODE_OPERATOR:
    return left->value.op_type == right->value.op_type;
  case NODE_NUMBER:
    return !strcmp(left->value.digits, right->value.digits);
  case NODE_TOPLEVEL:
  case NODE_STRUCT:
  case NODE_SYNTAX:
  case NODE_WORD:
    return false;
  }

  return true;
}

void die(const char *s) {
  perror(s);
  exit(1);
}

typedef struct TokenizerState {
  char buffer[1024];
  size_t buffer_size;
} TokenizerState;

TokenizerState state_from_token_stream(NodeToken tokens[], size_t num_tokens) {
  TokenizerState state = {};

  if (num_tokens > 0) {
    NodeToken last_token = tokens[num_tokens - 1];
    switch (last_token.type) {
    case NODE_WORD:
    case NODE_TOPLEVEL:
    case NODE_STRUCT:
    case NODE_IDENT:
    case NODE_SYNTAX:
    case NODE_RESERVED_WORD:
    case NODE_OPERATOR:
    case NODE_NUMBER:
      break;
    }
  }

  return state;
}

bool is_separator(char character) {
  return (isblank(character) || character == ';');
}

void tokenize(NodeToken tokens[], size_t *num_tokens, const char *buffer,
              size_t buf_size) {
  TokenizerState state = state_from_token_stream(tokens, *num_tokens);

  char cur_char;
  for (size_t n = 0; n < buf_size; ++n) {
    cur_char = buffer[n];
    if (!is_separator(cur_char)) {
      state.buffer[state.buffer_size++] = cur_char;
    } else {
      if (state.buffer_size > 0) {
        state.buffer[state.buffer_size] = '\0';

        // Is it a reserved word?
        for (int res_word_i = 0; WB_RESERVED_WORDS[res_word_i]; res_word_i++) {
          size_t res_size = strlen(WB_RESERVED_WORDS[res_word_i]);
          if (state.buffer_size == res_size) {
            if (!strncmp(state.buffer, WB_RESERVED_WORDS[res_word_i],
                         res_size)) {
              NodeToken new_token = {};
              new_token.type = NODE_RESERVED_WORD;
              new_token.value.reserved_word = WB_RESERVED_WORDS[res_word_i];
              tokens[*num_tokens] = new_token;
              *num_tokens += 1;
              state.buffer_size = 0;
              break;
            }
          }
        }

        size_t num_ops = sizeof(WB_OPERATORS) / sizeof(WB_OPERATORS[0]);
        // Is it an operator?
        for (int op_word_i = 0; op_word_i < num_ops; op_word_i++) {
          OperatorMap op_map = WB_OPERATORS[op_word_i];
          size_t op_size = strlen(op_map.text);
          if (state.buffer_size == op_size) {
            if (!strncmp(state.buffer, op_map.text, op_size)) {
              state.buffer_size = 0;
              NodeToken new_token = {};
              new_token.type = NODE_OPERATOR;
              new_token.value.op_type = op_map.type;
              tokens[*num_tokens] = new_token;
              *num_tokens += 1;
              break;
            }
          }
        }

        // Is it a number?
        if (state.buffer_size > 0 && isdigit(state.buffer[0])) {
          bool is_num = true;
          for (size_t num_i = 0; num_i < state.buffer_size; num_i++) {
            if (!isdigit(state.buffer[num_i])) {
              is_num = false;
              break;
            }
          }
          if (is_num) {
            NodeToken new_token = {};
            new_token.type = NODE_NUMBER;
            new_token.value.digits = malloc(state.buffer_size + 1);
            strncpy(new_token.value.digits, state.buffer,
                    state.buffer_size + 1);
            state.buffer_size = 0;
            tokens[*num_tokens] = new_token;
            *num_tokens += 1;
          }
        }

        // Is it an identifier then?
        // check starts with alpha, all other chars are ok (alnum)
        if (state.buffer_size > 0 && isalpha(state.buffer[0])) {
          NodeToken new_token = {};
          new_token.type = NODE_IDENT;
          new_token.value.identifier = malloc(state.buffer_size + 1);
          strncpy(new_token.value.identifier, state.buffer,
                  state.buffer_size + 1);
          state.buffer_size = 0;
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

test test_node_token_eq(TestRun *test_run) {
  NodeToken reserved_word_int = {.type = NODE_RESERVED_WORD,
                                 .value = {.reserved_word = "int"}};
  NodeToken operator_eq = {.type = NODE_OPERATOR, .value = {.op_type = OP_EQ}};
  NodeToken identifier1 = {.type = NODE_IDENT, .value = {.identifier = "woot"}};
  NodeToken identifier2 = {.type = NODE_IDENT, .value = {.identifier = "abcd"}};
  NodeToken number1 = {.type = NODE_NUMBER, .value = {.digits = "1"}};
  NodeToken number9 = {.type = NODE_NUMBER, .value = {.digits = "9"}};

  NodeToken tokens[] = {reserved_word_int, operator_eq, identifier1,
                        identifier2,       number1,     number9};
  size_t num_tokens = sizeof(tokens) / sizeof(NodeToken);
  for (size_t i = 0; i < num_tokens; i++) {
    for (size_t j = 0; j < num_tokens; j++) {
      if (i != j) {
        mu_assert(!node_token_eq(&tokens[i], &tokens[j]));
      } else {
        mu_assert(node_token_eq(&tokens[i], &tokens[j]));
      }
    }
  }
}

test test_tokenize_file1(TestRun *test_run) {
  char *test_filename = ".\\examples\\autogenerated\\test1.wb";
  char *test_program = "int intj = 4 + 2;\n";
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
      {.type = NODE_IDENT, .value = {.identifier = "intj"}},
      {.type = NODE_OPERATOR, .value = {.op_type = OP_EQ}},
      {.type = NODE_NUMBER, .value = {.digits = "4"}},
      {.type = NODE_OPERATOR, .value = {.op_type = OP_PLUS}},
      {.type = NODE_NUMBER, .value = {.digits = "2"}},
  };
  size_t expected_len = sizeof(expected) / sizeof(expected[0]);
  NodeToken tokens[MAX_TOKENS];
  size_t token_length;
  tokenize_file(test_filename, tokens, &token_length);
  mu_assert_array_equal(node_token_eq, tokens, token_length, expected,
                        expected_len);

  remove(test_filename);
}

static void run_the_tests(TestRun *test_run) {
  mu_run_test(test_node_token_eq);
  mu_run_test(test_tokenize_file1);
}

RUN_TESTS(run_the_tests);
