all: p1_ p2_ prxy

p1_:
	g++ -std=c++0x -g -Wall hrd.cc ../Client/rabinclient.h ../Client/rabinclient.cc -o p1

p2_:
	g++ -std=c++0x -g -Wall psb.cc ../Server/rabinserver.h ../Server/rabinserver.cc -o p2


prxy:
	gcc -g -Wall normalproxy.c -o proxy



