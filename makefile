PROJECT = dictionary
CC = g++
CFLAGS = -c -Wall -std=c++11

all: $(PROJECT)

$(PROJECT): dic.o dic_add.o
	$(CC) dic.o dic_add.o -o $(PROJECT)

dic.o: dic.cpp
	$(CC) $(CFLAGS) dic.cpp
	
dic_add.o: dic_add.cpp
	$(CC) $(CFLAGS) dic_add.cpp

clean:
	rm -rf *.o $(PROJECT)
	
	
