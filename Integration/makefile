all:
	g++ -std=c++0x -g -Wall lproxy.cc ../Client/rabinclient.h ../Client/rabinclient.cc -o intproxy

debug:
	g++ -std=c++0x -g -Wall lproxy.cc ../Client/rabinclient.h ../Client/rabinclient.cc -o intproxy -DDEBUG=1

server:
	g++ -std=c++0x -g -Wall sproxy.cc ../Server/rabinserver.h ../Server/rabinserver.cc -o servproxy

