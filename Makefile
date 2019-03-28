readwrite: readwrite.o
	gcc readwrite.o -o readwrite

readwrite.o: readwrite.c
	gcc -c readwrite.c

clean:
	rm *.o readwrite
