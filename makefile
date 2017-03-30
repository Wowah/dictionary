PROJECT = dictionary
CC = g++
CFLAGS = -c -g -Wall -std=c++11 \
  -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0 \
  -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0 \
  -L/usr/local/lib -lmongocxx -lbsoncxx

all: $(PROJECT)

$(PROJECT): dic.o dic_add.o
	$(CC) dic.o dic_add.o -o $(PROJECT) \
  -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0 \
  -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0 \
  -L/usr/local/lib -lmongocxx -lbsoncxx

dic.o: dic.cpp
	$(CC) $(CFLAGS) dic.cpp
	
dic_add.o: dic_add.cpp
	$(CC) $(CFLAGS) dic_add.cpp

clean:
	rm -rf *.o $(PROJECT)
	
	
