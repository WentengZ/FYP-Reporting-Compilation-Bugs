
CC = g++
CFLAGS =

all: C-Locate C-Locate-Function

C-Locate: ./compiler.o ./linker.o ./main.o ./parser.o ./loader.o
	$(CC) $(CFLAGS) -o C-Locate ./compiler.o ./linker.o ./main.o ./parser.o ./loader.o

C-Locate-Function:
	clang++-11 -std=c++17 main2.cpp `llvm-config-11 --cxxflags --ldflags --libs --system-libs` -lclangTooling -lclangASTMatchers -lclangFrontend -lclangSerialization -lclangDriver -lclangParse -lclangSema -lclangEdit -lclangAnalysis -lclangAST -lclangLex -lclangBasic -o C-Locate-Function

compiler.o: ./source/compiler.cpp ./include/compiler.h
	$(CC) $(CFLAGS) -c ./source/compiler.cpp

linker.o: ./source/linker.cpp ./include/linker.h
	$(CC) $(CFLAGS) -c ./source/linker.cpp

parser.o: ./source/parser.cpp ./include/parser.h
	$(CC) $(CFLAGS) -c ./source/parser.cpp

loader.o: ./source/loader.cpp ./include/loader.h
	$(CC) $(CFLAGS) -c  ./source/loader.cpp

main.o: ./source/main.cpp ./include/compiler.h ./include/linker.h ./include/loader.h ./include/parser.h 
	$(CC) $(CFLAGS) -c ./source/main.cpp

clean:
	rm -f C-Locate C-Locate-Function ./source/compiler.o ./source/linker.o ./source/main.o ./source/parser.o ./compiler.o ./linker.o ./parser.o ./loader.o

.PHONY: clean
