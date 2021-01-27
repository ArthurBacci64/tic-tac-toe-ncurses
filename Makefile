all: main

ted: main.c
		$(CC) -o main main.c -lncurses -Wall -W -pedantic -std=c99

dev: main.c
		$(CC) -o main main.c -lncurses -Wall -W -pedantic -std=c99 -fsanitize=address

clean:
		rm main
