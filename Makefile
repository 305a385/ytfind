include config.mk

default: build

build: clean
	$(CC) -Wall -o $(NAME) main.c util.c -l curl -ljson-c

clean:
	rm -rf $(NAME) 

test: build
	./$(NAME) never gonna give you up

install: build
	cp $(NAME) /usr/bin/$(NAME)
	install -g 0 -o 0 -m 0644 ./manpages/$(NAME).1 /usr/local/man/man1/
	gzip /usr/local/man/man1/$(NAME).1

uninstall:
	rm -f /usr/bin/$(NAME)
	rm -f /usr/local/man/man1/$(NAME).1.gz
