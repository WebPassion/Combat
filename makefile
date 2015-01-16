#Makefile
PGM=Combat
C_FLAG=-Wall
CC=gcc
SRC=$(wildcard *.c)
OBJ=$(SRC:*.c=*.o)
LD=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

all:$(PGM)

personnage.o:personnage.c personnage.h
	$(CC) $< -c $(C_FLAG)
fenetre.o:fenetre.c fenetre.h personnage.h
	$(CC) $< -c $(C_FLAG)	

$(PGM):$(OBJ)
	$(CC) $^ -o $@ $(LD)