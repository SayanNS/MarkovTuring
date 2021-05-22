output = turing.txt
source = source.cpp
executable = algorithm

$(executable): $(source)
	g++ $< -o $@

run: $(executable)
	./$(executable) > $(output)
