CC = g++
CFLAGS = -O -Wall -std=c++11
# CFLAGS += `llvm-config --cxxflags`

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp,%.o,$(SRC))

scc : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o : %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY : clean run

clean :
	rm -f scc $(OBJ)

run : 
	timeout 1 ./scc