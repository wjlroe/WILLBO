all: fmt test vet lint build

fmt:
	@gofmt -w .

test: build
	@go test ./...

vet:
	@go vet ./...

lint:
	@golint ./...

build:
	@go build ./...
