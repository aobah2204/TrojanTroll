FILES = main.cc
EXE = server
CXXFLAGS = -Wall

all: build

build: $(FILES)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(FILES)

clean:
	rm -f *.o $(EXE)

rebuild: clean build