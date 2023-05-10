all: clean build run

build: main.cpp
	clang++ -std=c++11 main.cpp
clean:
	rm -f a.out image.ppm 
run: a.out
	./a.out > image.ppm