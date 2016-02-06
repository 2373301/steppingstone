#make file
##
##

GCC:=gcc
CCFLAG:=-g 

BINS:=client

all:$(BINS)


client:client.c
	gcc -g -o client client.c

.PHONY : clean
clean:
	rm client 
 	