all : compile link run

compile: 
	g++ -IC:\SFML-2.6.1\include -c test.cpp -o test.o

link:
	g++ -LC:\SFML-2.6.1\lib test.o -o test.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main -mwindows

run:
	.\test
# clean:
# 	erase -f main *.o
