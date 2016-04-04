all: client server proxy

client:
	g++ -std=c++0x -g -Wall lproxy.cc ../Client/rabinclient.h ../Client/rabinclient.cc -o intproxy

server:
	g++ -std=c++0x -g -Wall sproxy.cc ../Server/rabinserver.h ../Server/rabinserver.cc -o servproxy

proxy:
	gcc -g -Wall normalproxy.c -o proxy

try2: hrd psb proxy

hrd:
	g++ -std=c++0x -g -Wall hrd.cc ../Client/rabinclient.h ../Client/rabinclient.cc -o hdrdr

psb:
	g++ -std=c++0x -g -Wall psb.cc ../Server/rabinserver.h ../Server/rabinserver.cc -o prdr

