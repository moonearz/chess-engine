SRCS = $(wildcard *.c)
NAME = methane

all:
	gcc $(SRCS) -o $(NAME) -O2