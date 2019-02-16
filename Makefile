CC	= g++
CCFLAGS	= -O3 -Wall -DNDEBUG -DUSE_SPACE_GRID
#LNFLAGS	= -lglut -lGL -lGLU -lX11 
LNFLAGS	= -lXi -lX11 -lXext -lGL -lGLU -lglut -lX11 

PROG1 = cnf3d2
OBJS1 = Vector.o Node.o Graph.o SpaceGrid.o SceneParameters.o cnf3d2.o

progs: $(PROG1)

all: progs

$(PROG1): $(OBJS1)
	$(CC) $(LNFLAGS) -o $@ $(OBJS1)

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

depend:
	g++ -MM *.cpp

clean:
	rm -f $(OBJS1)

realclean: clean
	rm -f $(PROG1)


# dependencies generated with 'make depend'
Graph.o: Graph.cpp Graph.h Node.h Vector.h SpaceGrid.h SceneParameters.h
Node.o: Node.cpp Node.h Vector.h
SceneParameters.o: SceneParameters.cpp SceneParameters.h
SpaceGrid.o: SpaceGrid.cpp SpaceGrid.h Node.h Vector.h
Vector.o: Vector.cpp Vector.h
