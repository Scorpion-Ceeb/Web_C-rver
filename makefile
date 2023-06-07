port ?=5000
root ?=$(HOME)

.PHONY: run
run:
		gcc ./*.c -lpthread -lcurl -o main && ./main $(port) $(root)

.PHONY: build
build:
		gcc ./*c -lpthread -lcurl -o main