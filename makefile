main: main.cpp
	clang++ -std=c++17 main.cpp -o main `pkg-config --cflags --libs raylib`

run: main
	@./main
