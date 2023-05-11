include config.mk

default: build

build: clean
	$(CC) -Wall -o $(NAME) main.c util.c -l curl -ljson-c

clean:
	rm -rf $(NAME) 

test: build
	./$(NAME) hello world

install: build
	cp $(NAME) /usr/bin/$(NAME)
