all: client
	rm -rf *.o

client: client.o ClientModBUS.o
	g++ client.o ClientModBUS.o -o client -lmodbus

client.o: client.cpp
	g++ -c client.cpp

ClientModBUS.o: ClientModBUS.cpp
	g++ -c ClientModBUS.cpp

clean:
	rm -rf *o client

install-libmodbus:
	cp libmodbus-3.0.6/lib/libmodbus.* /usr/lib
