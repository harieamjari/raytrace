CC = clang
#CFLAGS = -fsanitize=address -fsanitize=undefined -ggdb3 -Wall -Wextra
CFLAGS =  -ggdb3 -O3
LDFLAGS = -lm
main : utils.c math.c main.c tracer.c
