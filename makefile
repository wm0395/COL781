FILE = starter_code
compile: 
	g++ -Isrc/include -Lsrc/lib -Bsrc/bin -o $(FILE) $(FILE).cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
run: compile
	./$(FILE) 

all: 
	g++ -I /src/include -L /src/lib -c *.cpp -o OUT -lmingw32 -lSDL2main -lSDL2