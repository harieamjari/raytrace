CC = clang
#CFLAGS = -fsanitize=address -fsanitize=undefined -ggdb3
CFLAGS =  -ggdb3
LDFLAGS = -lm
main : utils.c math.c main.c tracer.c
