CC=gcc

# option de compilateur
CFLAGS=-Wall -std=c99
EXEC=backgammon

all: backgammon

backgammon: test.o 
	gcc test.o -o backgammon


ia: bot.o
	gcc -shared -o bot.so bot.o

bot.o: bot.c
	gcc -fPIC -c bot.c

gui:


commun:


Comun/test.o: Commun/test.c
	gcc Commun/test.c -o Commun/test.o



gcc -Wall -std=c99 -fPIC -c bot.c

    gcc -Wall -std=c99 -shared -o bot.so bot.o
    
     gcc -Wall main.c -o main -ldl