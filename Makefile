TARGET = build/kv_store
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
CC ?= cc
CFLAGS ?= -Weverything -Wno-unsafe-buffer-usage -Wno-declaration-after-statement

all: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f build/*

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $?

obj/%.o : src/%.c
	$(CC) $(CFLAGS) -Iinc -c -o $@ $<
