CXX = clang++
CXXFLAGS = -std=c++11 -Wall -Wextra

SRCS = lexer.cpp parser.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = my_program

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

