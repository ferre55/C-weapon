all:
	gcc -Wall -c Scheduler.c -o Scheduler.o
    gcc -Wall -c Software_Timers.c -o Software_Timers.o
	gcc -Wall -c Main.c -o Main.o
	gcc Main.o Software_Timers.o  Scheduler.o -o main.exe
	./main.exe
	
clean:
	rm *.exe *.o