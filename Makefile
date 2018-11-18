CPP = learningSDL.cpp

COMPILE_FLAGS = -Wall -Werror

LINKER_FLAGS = -lSDL2 -lSDL2_mixer

EXEC = learningSDL

compile: $(CPP)
	g++ $(CPP) $(COMPILE_FLAGS) $(LINKER_FLAGS) -o $(EXEC)

clean: $(EXEC)
	rm $(EXEC)

run: $(EXEC)
	./$(EXEC)