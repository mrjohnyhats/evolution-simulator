OBJS = evolution.cpp globals.cpp Entity.cpp Disease.cpp Pop_counter.cpp

OBJ_NAME = evolution

w : $(OBJS)
	g++ $(OBJS) -std=gnu++11 -g -IC\:\mingw_dev_lib\include\SDL2 -LC\:\mingw_dev_lib\lib -w -lmingw32 -lSDL2main -lSDL2 -o $(OBJ_NAME)
linux : $(OBJS)
	g++ $(OBJS) -std=gnu++11 -ggdb -lSDL2 -o $(OBJ_NAME)
osx : $(OBJS)
	clang++ $(OBJS) -std=gnu++11 -ggdb -O0 -I/Library/Frameworks/SDL2.framework/Headers -framework SDL2 -framework SDL2_ttf -o $(OBJ_NAME)