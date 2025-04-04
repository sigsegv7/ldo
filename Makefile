CFILES = $(shell find src/ -name "*.c")
CC = gcc

bin/ldo: $(CFILES)
	mkdir -p $(@D)
	$(CC) $^ -o $@ -I src/include/
