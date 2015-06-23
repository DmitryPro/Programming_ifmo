all: control_server main run_server run_main

control_server: semaphore.o control_server.o
	gcc -o control_server control_server.o semaphore.o

main: semaphore.o main.o
	gcc -o main semaphore.o main.o

semaphore.o: semaphore.c
	gcc -o semaphore.o semaphore.c -c

control_server.o: control_server.c
	gcc -o control_server.o control_server.c -c

main.o: main.c
	gcc -o main.o main.c -c

run_server:
	./control_server

run_main:
	./main

clean:
	killall control_server && rm -rf *.o *.log all