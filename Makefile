OBJS = evolution.cpp globals.cpp Entity.cpp Disease.cpp Pop_counter.cpp Terrain.cpp

OBJ_NAME = evolution

w : $(OBJS)
	g++ $(OBJS) -std=gnu++0x -g -lmingw32 -lSDL2_ttf -lSDL2main -lSDL2 -o $(OBJ_NAME)
linux : $(OBJS)
	g++ $(OBJS) -std=gnu++11 -ggdb -w -lSDL2 -lSDL2_ttf -o $(OBJ_NAME)
osx : $(OBJS)
	clang++ $(OBJS) -std=gnu++11 -D __OSX__ -ggdb -I/Library/Frameworks/SDL2.framework/Headers -framework SDL2 -framework SDL2_ttf -o $(OBJ_NAME)
