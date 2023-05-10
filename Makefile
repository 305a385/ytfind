include config.mk

default: build

build: clean
	$(CC) -Wall -o $(NAME) main.c util.c -l curl -ljson-c

clean:
	rm -rf $(NAME) 

test: build
	./$(NAME) roar

#json: clean
#	$(CC) json.c -ljson-c -o json
