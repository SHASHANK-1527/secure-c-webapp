CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIBS = -lmicrohttpd -lsqlite3 -lssl -lcrypto

SRC = src/main.c src/db.c src/auth.c src/utils.c
OUT = server

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)
