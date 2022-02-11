hw2: main.o libmyfstream.a 
	g++ -o hw2 main.o -L. -lmyfstream 
	gdb -q hw2
%.o: %.cpp
	g++ -c $<

libmyfstream.a: myfstream.o
	ar rcs $@ myfstream.o

clean:
	rm -f *.o libmyfstream.a hw2 
