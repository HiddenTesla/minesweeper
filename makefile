main : main.o game.o cli.o
	g++ main.o game.o cli.o -o main
main.o : main.cpp game.h cli.h
	g++ -c main.cpp
game.o : game.cpp game.h
	g++ -c game.cpp
cli.o : cli.cpp cli.h
	g++ -c cli.cpp
clean :
	rm -f *.o *~ main
edit :
	vim -p main.cpp game.h game.cpp cli.h cli.cpp
