run: main.o magic.o ui.o
	clang main.o magic.o ui.o -o run

main.o: main.c
	clang -c main.c

magic.o: magic.c
	clang -c magic.c

ui.o: ui.c 
	clang -c ui.c

clean:
	rm *.o run