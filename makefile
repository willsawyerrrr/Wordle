all:
	@gcc wordle.c wordle.h -o wordle -pedantic -Wall -std=gnu99 -I/local/courses/csse2310/include -L/local/courses/csse2310/lib -lcsse2310a1
