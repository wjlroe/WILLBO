all: fmt test vet lint

fmt:
	@gofmt -w .

test:
	@go test ./...

vet:
	@go vet ./...

lint:
	@golint ./...

build:
	@go build

