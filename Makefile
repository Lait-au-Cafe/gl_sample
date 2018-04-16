CXXFLAGS=-std=c++11 -Wall -Wextra -Werror -g
LIBS=-lglut -lGLU -lGL -lglfw
TARGET=gl_sample

$(TARGET): source.cpp
	g++ -o $@ $< $(CXXFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(TARGET)
