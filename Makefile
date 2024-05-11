CFLAGS=-g -Wall -Wextra $(shell pkg-config --cflags gtk4) -Isrc
LDFLAGS=$(shell pkg-config --libs gtk4)


app:
	cc $(CFLAGS) -o app main.c src/*.c  $(LDFLAGS)
clean:
	rm app
