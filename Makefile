all: learn1.c
	gcc -Wall -Werror -fsanitize=address -o learn1 learn1.c
clean:
	rm -f learn1


