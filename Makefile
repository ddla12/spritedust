CFLAGS=-g -Wall -Wextra $(shell pkg-config --cflags gtk4) -Isrc
LDFLAGS=$(shell pkg-config --libs gtk4)


app:
	cc $(CFLAGS) -o app src/* main.c  $(LDFLAGS) -lm
clean:
	rm app
