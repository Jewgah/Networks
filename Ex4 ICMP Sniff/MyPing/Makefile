# Makefile for myping

all: myping 

myping: myping.c
	gcc -o myping myping.c

clean:
	rm -f *.o myping

runs:
	./myping


runs-strace:
	strace -f ./myping

