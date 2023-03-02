CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter -I include

default: bin/cli bin/compiler bin/prettyprint

src/c_parser.tab.cpp src/c_parser.tab.hpp:
	bison -v -d src/c_parser.y -o src/c_parser.tab.cpp

src/c_lexer.yy.cpp:
	flex -o src/c_lexer.yy.cpp src/c_lexer.flex

bin/cli: src/cli.o src/c_parser.tab.o src/c_lexer.yy.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/cli $^

bin/compiler: src/cli.o src/compiler.o src/c_parser.tab.o src/c_lexer.yy.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/compiler $^

bin/prettyprint: src/cli.o src/prettyprint.o src/c_parser.tab.o src/c_lexer.yy.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/prettyprint $^

clean:
	rm -f src/*.yy.cpp
	rm -f src/*.tab.hpp
	rm -f src/*.tab.cpp
	rm -f src/*.output
	rm -f src/*.o
	rm -rf bin