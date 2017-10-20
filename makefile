CC = g++
STD = c++11
DEBUG = -g
CFLAGS = -std=$(STD) -Wall -c $(DEBUG)
LFLAGS = -lglfw3 -lGL -lglut -lGLEW -lm -ldl -lXinerama -lXrandr -lXi -lXcursor -lX11 -lXxf86vm -lpthread 
OBJS = main.o vertex_array_object.o gl_utils.o
SHADERS = vertex.shader fragment.shader
EXEC = a.out

run: $(EXEC)
	@clear
	@./$(EXEC)

$(EXEC): $(OBJS)
	@$(CC) $(OBJS) -o $(EXEC) $(LFLAGS)

main.o: main.cpp vertex_array_object.hpp gl_utils.hpp $(SHADERS)
	@$(CC) $(CFLAGS) main.cpp

%.o: %.cpp %.hpp
	@$(CC) $(CFLAGS) $<

all: clean run

clean:
	@rm -f *.o .*.swp $(EXEC)
