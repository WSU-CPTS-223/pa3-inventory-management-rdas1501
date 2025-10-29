out: clean compile execute

compile: main.cpp
	g++ -g -Wall -std=c++14 main.cpp -o mainexe

execute: mainexe
	./mainexe

clean:
	rm -f mainexe