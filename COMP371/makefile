CC = g++
STD = c++11
DEBUG = -g
CFLAGS =  -std=$(STD) -c $(DEBUG) -Wno-write-strings
LFLAGS = -lglfw3 -lGL -lGLEW -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread -lSOIL
OBJS = Main.o objloader.o vertex_array_object.o gl_utils.o terrian.o item.o QuickHull.o l_system.o frame_buffer_object.o
SHADERS = vertex.shader fragment.shader
EXEC = a.out

run: $(EXEC)
	@clear
	@./$(EXEC)

$(EXEC): $(OBJS)
	@$(CC) $(OBJS) -o $(EXEC) $(LFLAGS)

Main.o: Main.cpp $(OBJ)
	@$(CC) $(CFLAGS) Main.cpp

QuickHull.o: ../quickhull/QuickHull.cpp ../quickhull/QuickHull.hpp
	@$(CC) $(CFLAGS) $<

%.o: %.cpp %.hpp
	@$(CC) $(CFLAGS) $<

all: clean run

clean:
	@rm -f *.o .*.swp $(EXEC)
