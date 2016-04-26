CC = g++
CFLAGS = -c -Wall -std=gnu++11

ui.exe: ui.o flow.o host.o link.o node.o packet.o router.o rtable.o
	$(CC) ui.o flow.o host.o link.o node.o packet.o router.o rtable.o -o ui.exe

ui.o: ui.cpp 
	$(CC) $(CFLAGS) ui.cpp

flow.o: flow.cpp flow.h
	$(CC) $(CFLAGS) flow.cpp

host.o: host.cpp host.h
	$(CC) $(CFLAGS) host.cpp

link.o: link.cpp link.h
	$(CC) $(CFLAGS) link.cpp

node.o: node.cpp node.h
	$(CC) $(CFLAGS) node.cpp

packet.o: packet.cpp packet.h
	$(CC) $(CFLAGS) packet.cpp

router.o: router.cpp router.h
	$(CC) $(CFLAGS) router.cpp

rtable.o: rtable.cpp rtable.h
	$(CC) $(CFLAGS) rtable.cpp

debug:
	$(CC) -Wall -std=gnu++11 -g -o ui.cpp flow.cpp host.cpp router.cpp rtable.cpp link.cpp node.cpp packet.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
