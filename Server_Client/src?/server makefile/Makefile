all: server Rx90.o
	rm -rf *.o

server: server.cpp serverModBUS.o Rx90.o
	g++ -o $@ -g -I/usr/local/include/ $< serverModBUS.o Rx90.o -
L/usr/local/lib/ -lserial -lpthread -lmodbus

#server.o: server.cpp
#	g++ -c server.cpp
#clear:
#	rm -rf *.o

serverModBUS.o: serverModBUS.cpp
	g++ -c serverModBUS.cpp

Rx90.o: Rx90.cpp Rx90.h
	g++ -c -g -I/usr/local/include/ $< -lpthread

clean:
	rm -rf *o server

install-libmodbus:
	cp libmodbus-3.0.6/lib/libmodbus.* /usr/lib
