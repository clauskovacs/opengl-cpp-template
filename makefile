############################################################
CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++17 -I $(HEADERINCLUDE)
EXC = build
OBJ = obj
HEADERINCLUDE = /home/itsme/Desktop/git_repos/opengl-cpp-template/include
############################################################

OUTPUT_NAME = opengl_cpp_template

all: $(OBJ)/main.o $(OBJ)/test.o
	$(CXX) $(CXXFLAGS) -o $(EXC)/$(OUTPUT_NAME) $(OBJ)/main.o $(OBJ)/test.o

$(OBJ)/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c src/main.cpp

$(OBJ)/test.o: src/test.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c src/test.cpp

clean:
	$(RM) $(OBJ)/main.o $(OBJ)/test.o

run: $(EXC)/$(OUTPUT_NAME)
	./$(EXC)/$(OUTPUT_NAME)
