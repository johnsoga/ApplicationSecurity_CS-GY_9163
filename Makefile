CC=gcc

default: prog

get-deps:
	sudo apt-get update
	sudo apt-get install -y build-essential check wget valgrind
	wget http://lcamtuf.coredump.cx/afl/releases/afl-latest.tgz
	tar xvf afl-latest.tgz
	cd afl-*
	make && sudo make install
	cd

.DELETE_ON_ERROR:
dictionary.o: dictionary.c
	gcc -Wall -c dictionary.c dictionary.h

.DELETE_ON_ERROR:
spell.o: spell.c
	gcc -Wall -c spell.c

test.o: test_main.c
	gcc -Wall -c test_main.c

.DELETE_ON_ERROR:
main.o: main.c
	gcc -Wall -c main.c

test: dictionary.o spell.o test_main.o
	gcc -Wall -o test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	./test_main

prog: dictionary.o spell.o main.o
	gcc -Wall -o spell_check dictionary.o spell.o main.o
	./spell_check test1.txt wordlist.txt

val: dictionary.o spell.o main.o
	gcc -Wall -o spell_check dictionary.o spell.o main.o
	valgrind --leak-check=yes --track-origins=yes ./spell_check test1.txt wordlist.txt

clean:
	rm *.o

cleanall:clean
	rm spell_check test_main
