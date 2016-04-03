all: client server

client:
	g++ -std=c++0x -g -Wall lproxy.cc ../Client/rabinclient.h ../Client/rabinclient.cc -o intproxy

server:
	g++ -std=c++0x -g -Wall sproxy.cc ../Server/rabinserver.h ../Server/rabinserver.cc -o servproxy

