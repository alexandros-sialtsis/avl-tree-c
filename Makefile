
CC = gcc
CCFLAGS = -Wall -g

tree1: tree1.c hbtree.o
	$(CC) $(CCFLAGS) tree1.c hbtree.o -o tree1
hbtree.o: hbtree.c
	$(CC) $(CCFLAGS) -c hbtree.c 

clean:
	rm hbtree.o tree1