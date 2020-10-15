all: runUPGMA

USERNAME=$(shell whoami)

runUPGMA: App.cpp src/HamParties.cpp src/matrixDist.cpp src/guide_tree.cpp
	g++ -o runUPGMA App.cpp src/HamParties.cpp src/matrixDist.cpp src/guide_tree.cpp -I/home/$(USERNAME) -I/home/$(USERNAME)/boost_1_71_0 -std=c++11 /home/$(USERNAME)/libscapi/libscapi.a -lboost_system -L/home/$(USERNAME)/boost_1_71_0/stage/lib -lssl -lcrypto -lgmp -g -pthread -Wall -Werror









