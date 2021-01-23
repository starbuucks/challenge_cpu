all : cpu

cpu: cpu.o process.o
	gcc -g -no-pie -o cpu cpu.o process.o

cpu.o: cpu.c process.h
	gcc -g -c -o cpu.o cpu.c

process.o: process.c process.h
	gcc -g -c -o process.o process.c

clean:
	rm -f cpu
	rm -f *.o