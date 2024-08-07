CFLAGS = -Wall -pedantic -ansi -std=c99
CFLAGS = ""
SRCS := $(wildcard source/*.c)
OBJS := $(SRCS:.c=.o)

main: $(OBJS)
	@gcc $(CFLAGS) $(OBJS) -o main
	@rm source/*.o
	@./main


%.o: %.c
	@gcc $(CFLAGS) -c $< -o $@
