all:
	gcc -Wall -O -o test ncurses-test.c -lncurses

clean:
	rm -f *~ *.o *.out test
