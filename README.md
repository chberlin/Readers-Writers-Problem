# Readers-Writers-Problem


`make` to compile

Run `make cleanText` between runs to get new textfiles. If you don't, text files will contain results from previous runs.

Code starts with `W` number of writers and `R` number of readers. Each of the `W` writers add `N` random integers, one at a time, to a shared linked list. That is, each writer obtains permission to write within critical section entry code, writea one value, then exita the critical section, and does this `N` times. Each of the `R` reader threads inspect the linked list `N` times, to count and return values. 

`W`, `R`, and `N` are all specified as required command line arguments to your executable, as follows:
```
        ./readwrite N R W
```

Each writer will sleep for a few seconds briefly between each write. Reader i will count the number of values currently stored in the linked list that "end in i" (that is, the
number of values v for which v % 10 == i). 


Reader i will write its output to a file named reader_i.txt formatted to match the following example: 
```
   Reader 4: Read 1: 3 values ending in 4
   Reader 4: Read 2: 4 values ending in 4
   Reader 4: Read 3: 7 values ending in 4
   Reader 4: Read 4: 10 values ending in 4
```
etc.

So running the program as follows:
        `./readwrite 40 3 7`
Will result in 3 files, named reader_1.txt, reader_2.txt, and reader_3.txt. Each file will contain exactly 40 lines, formatted as shown above.

In addition to the Readers and Writers threads, one additional thread will be waiting around (on a condition variable) to report when only a single reader remains. That thread will print "Almost Done!" to stdout. 
