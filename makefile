words: main.o words.o hash.o
	g++ main.o words.o hash.o -o words

main.o: main.cpp words.h hash.h
	g++ -c main.cpp

words.o: words.cpp words.h hash.h
	g++ -c words.cpp

hash.o: hash.cpp hash.h
	g++ -c hash.cpp	

clean:
	rm *.o
	rm words

run: words
	./words
