compile: $(file)
	g++ -o $(file) $(file).cpp

run: $(file) compile
	./$(file)