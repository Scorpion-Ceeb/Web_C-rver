port ?=5000
root ?=$(HOME)

.PHONY: dev
dev:
		gcc ./*.c -lpthread -lcurl -o test && ./test $(port) $(root)

.PHONY: build
build:
		gcc ./*c -lpthread -lcurl -o test