# Makefile

CFLAGS= -g -Wall -pedantic
CC=gcc

heatSim: main.o matrix2d.o leQueue.o mplib3.o
	$(CC) $(CFLAGS) -pthread -o heatSim main.o matrix2d.o leQueue.o mplib3.o

main.o: main.c matrix2d.h leQueue.h mplib3.h
	$(CC) $(CFLAGS) -c main.c

matrix2d.o: matrix2d.c matrix2d.h
	$(CC) $(CFLAGS) -c matrix2d.c

leQueue.o: leQueue.c leQueue.h
	$(CC) $(CFLAGS) -c leQueue.c

mplib3.o: mplib3.c mplib3.h	leQueue.h
	$(CC) $(CFLAGS) -c mplib3.c

clean:
	rm -f *.o heatSim

zip:
	zip heatSim_ex01_solucao.zip main.c matrix2d.c matrix2d.h leQueue.c leQueue.h mplib3.c mplib3.h Makefile

run:
	./heatSim 10 10.0 10.0 0.0 0.0 10 5 0
