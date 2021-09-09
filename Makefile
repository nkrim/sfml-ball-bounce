CXX = g++
CXXFLAGS = -std=c++11 -Wall
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

OUT = sfbounce
OBJS = main.o utils.o ball.o

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(SFMLFLAGS) $(OBJS) -o $(OUT)

main.o: ball.o utils.o
utils.o: 
ball.o: utils.o


.PHONY: clean
clean:
	rm -rf *.o $(OUT)