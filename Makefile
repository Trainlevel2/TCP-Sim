CC = g++
CFLAGS = -c -Wall -std=gnu++11

ui.exe: ui.o flow.o host.o link.o node.o packet.o
	$(CC) ui.o heap.o hash.o graph.o -o ui.exe

ui.o: ui.cpp
	$(CC) $(CFLAGS) ui.cpp

flow.o: flow.cpp
	$(CC) $(CFLAGS) flow.cpp

host.o: host.cpp
	$(CC) $(CFLAGS) host.cpp

link.o: $(NAME).cpp
	$(CC) $(CFLAGS) link.cpp

node.o: node.cpp
	$(CC) $(CFLAGS) node.cpp

packet.o: $(NAME).cpp
	$(CC) $(CFLAGS) packet.cpp


debug:
	$(CC) -g -o ui.cpp flow.cpp host.cpp link.cpp node.cpp packet.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
