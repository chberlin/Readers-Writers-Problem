readwrite: readwrite.o 
	gcc readwrite.o -o readwrite -lpthread

readwrite.o: readwrite.c
	gcc -c readwrite.c

linkedlist.o: linkedlist.c
	gcc -c linkedlist.c

clean:
	rm *.o readwrite
