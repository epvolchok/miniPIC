CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra

TARGET = program

SRC = main.cpp clsWorld.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp clsWorld.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
