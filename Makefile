CPP = main.cpp

COMPILE_FLAGS = -Wall -Werror

LINKER_FLAGS = -lSDL2 -lSDL2_mixer -lSDL2_image -lGL -lX11 -lpthread -lXi -ldl -lXrandr -lassimp 

EXEC = main

compile: $(CPP)
	g++ $(CPP) $(COMPILE_FLAGS) $(LINKER_FLAGS) -o $(EXEC)

clean: $(EXEC)
	rm $(EXEC)

run: $(EXEC)
	./$(EXEC)