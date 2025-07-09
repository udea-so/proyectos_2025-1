CC = gcc
CFLAGS = -Iinclude -lncurses
SRC = src/main.c src/process_monitor.c src/alerts.c
BIN = build/monitor

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

clean:
	rm -f $(BIN)