readwrite: readwrite.o 
	gcc readwrite.o -g  -o readwrite -lpthread

readwrite.o: readwrite.c
	gcc -c -g readwrite.c

linkedlist.o: linkedlist.c
	gcc -c -g linkedlist.c

clean:
	rm *.o readwrite
