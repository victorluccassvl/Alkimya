CPP = learningSDL.cpp glad.c

COMPILE_FLAGS = -Wall -Werror

LINKER_FLAGS = -lSDL2 -lSDL2_mixer -lGL -lX11 -lpthread -lXi -ldl -lXrandr

EXEC = learningSDL

compile: $(CPP)
	g++ $(CPP) $(COMPILE_FLAGS) $(LINKER_FLAGS) -o $(EXEC)

clean: $(EXEC)
	rm $(EXEC)

run: $(EXEC)
	./$(EXEC)