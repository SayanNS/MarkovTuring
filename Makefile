compile: source.cpp
	g++ $< -o algorithm
run: algorithm markov.txt
	./algorithm > turing.txt