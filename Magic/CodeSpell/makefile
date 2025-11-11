output: main.o ui.o
	gcc main.o ui.o -o output

main.o: main.c
	gcc -c main.c

ui.o: ui.c 
	gcc -c ui.c

clean:
	rm *.o output