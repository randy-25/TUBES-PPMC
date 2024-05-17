output: main.o greedy.o bruteforce.o bfs.o dfs.o
	gcc main.o greedy.o bruteforce.o bfs.o dfs.o -o app

main.o: main.c
	gcc -c main.c

greedy.o: greedy.c greedy.h
	gcc -c greedy.c

bruteforce.o: bruteforce.c bruteforce.h
	gcc -c bruteforce.c

bfs.o: bfs.c bfs.h
	gcc -c bfs.c

dfs.o: dfs.c dfs.h
	gcc -c dfs.c

clean:
	del *.o app.exe
