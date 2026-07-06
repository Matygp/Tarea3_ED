CXX = g++
CXXFLAGS = -Wall -std=c++11 -O2

# Archivos fuente
SOURCES = src/main.cpp src/openhash.cpp src/HashFunctions.cpp src/TweetData.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main_hash

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(EXECUTABLE)