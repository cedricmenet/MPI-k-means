CC=gcc
CFLAGS=-W -Wall -I/usr/include/mysql -DBIG_JOINS=1  -fno-strict-aliasing  -g
LDFLAGS= -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -ldl
EXEC=Mpimain SQLmain

all: $(EXEC)

Appli : appli.o kmeans.o
	mpicc -o AppliRun  kmeans.o appli.o  -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -ldl
	
appli.o : appli.c kmeans.h
	mpicc -o appli.o -c appli.c  -I/usr/include/mysql -DBIG_JOINS=1  -fno-strict-aliasing  -g -W -Wall
	
	
Appli2 : appli2.o
	mpicc -o Appli2Run appli2.o kmeans.o -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -ldl
	
appli2.o : appli2.c
	mpicc -o appli2.o -c appli2.c  -I/usr/include/mysql -DBIG_JOINS=1  -fno-strict-aliasing  -g -W -Wall
	
Appli2Run: Appli2
	mpirun -np 10 ./Appli2Run
	
kmeans.o: kmeans.c
	gcc -o kmeans.o -c kmeans.c -lm
	
AppliRun: Appli
	mpirun -np 10 ./AppliRun

Mpimain: Mpimain.c
	mpicc Mpimain.c -o Mpimain -lm
	
MpiRun: Mpimain
	mpirun -np 2 ./Mpimain
test: test.c kmeans.h
	gcc -g -o test test.c -lm
	
MpiCirclemain: MpiCirclemain.c
	mpicc MpiCirclemain.c -o MpiCirclemain -lm
	
MpiCircleRun: MpiCirclemain
	mpirun -np 10 ./MpiCirclemain
	
SQLmain: SQLmain.o
	gcc -o SQLmain SQLmain.o -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -ldl

SQLmain.o:
	gcc -o SQLmain.o -c SQLmain.c  -I/usr/include/mysql -DBIG_JOINS=1  -fno-strict-aliasing  -g -W -Wall


clean:
	rm -rf *.o
	rm -rf MpiCirclemain Mpimain

mrproper: clean
	rm -rf $(EXEC)