CC = gcc
CFLAGS = -Wall -g -std=c99 -Werror -D_POSIX_C_SOURCE=200809L
OBJ = main.o helper_func.o SMT_fetch_data.o SMT_Logic.o SMT_plot_func.o SMT_structs.o
HEADERS = helper_func.h SMT_fetch_data.h SMT_Logic.h SMT_plot_func.h SMT_structs.h

all: a3

a3: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o a3