CC = g++
FLAGS = -g -I/usr/local/include/
LIBS = -L/usr/local/lib/ -lserial -lpthread
OBJECTS = Rx90.o

all: test $(OBJECTS)

test: test.cpp $(OBJECTS)
	$(CC) -o $@ $(FLAGS) $< $(OBJECTS) $(LIBS)

Rx90.o: Rx90.cpp Rx90.h
	$(CC) -c $(FLAGS) $<

clean:
	rm -f test *.o
	rm -f *~

rebuild: clean all

