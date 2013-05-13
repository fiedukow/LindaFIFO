tests:
	clang++ -std=c++11 -c parser.cpp
	clang++ -std=c++11 parserTest.cpp parser.o
	./a.out
