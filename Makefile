FILE=spell_check
CC=gcc

.DELETE_ON_ERROR:
build: spell.o dictionary.o
	gcc spell.o dictionary.o -o $(FILE)

clean:
	rm *.o $(FILE)
