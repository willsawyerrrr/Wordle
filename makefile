all:
	@gcc -I/local/courses/csse2310/include -pedantic -Wall -std=gnu99 -L/local/courses/csse2310/lib -lcsse2310a1 wordle.c wordle.h -o wordle
