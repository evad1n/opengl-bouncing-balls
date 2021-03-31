CXXFLAGS=-Wall
GLUTFLAGS=-lglut -lGLU -lGL

NAME=bounce

n?=20

all: bounce.o vector2.o helper.o drawing.o bouncyball.o
	g++ -o $(NAME).exe bounce.o vector2.o helper.o drawing.o bouncyball.o $(GLUTFLAGS)

run: all
	./$(NAME).exe $(n)

clean:
	rm -f *.exe *.o
