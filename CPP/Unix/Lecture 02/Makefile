main: main.c
	gcc -S -o main.s main.c
	gcc main.c -g -o main
	gdb -q main
.PHONY:clean
clean:
	rm *.o main main.s

