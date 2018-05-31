all: queue main thread scheduler
	gcc -pthread obj/main.o obj/queue.o obj/scheduler.o obj/thread.o -o bin/scheduler
	
main:
	gcc -pthread -Wall -c src/main.c -o obj/main.o -I include/ 

queue:
	gcc -pthread -Wall -c src/queue.c -o obj/queue.o -I include/  

scheduler:
	gcc -pthread -Wall -c src/scheduler.c -o obj/scheduler.o -I include/ 

thread:
	gcc -pthread -Wall -c src/thread.c -o obj/thread.o -I include/ 

clean:
	rm -f obj/*.o
	rm -f bin/scheduler

run:
	cd bin && ./scheduler
	