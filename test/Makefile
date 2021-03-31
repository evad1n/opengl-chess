CXXFLAGS=-Wall -Werror
GLUTFLAGS=-lglut -lGLU -lGL

NAME=test

all:
	g++ *.cpp -o $(NAME).exe $(GLUTFLAGS) 
debug:
	g++ -g *.cpp -o $(NAME).exe $(GLUTFLAGS) 
run: all
	./$(NAME).exe
clean:
	rm -f *.exe *.o
