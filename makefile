############################################################
CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -lGL -lGLU -lglut -std=c++17 -I $(HEADERINCLUDE)
EXC = build
OBJ = obj
PROJECTNAME = opengl-cpp-template
FOLDERNAME = opengl-cpp-template
HEADERINCLUDE = /home/itsme/Desktop/git_repos/$(FOLDERNAME)/include
############################################################

OUTPUT_NAME = opengl_cpp_template

all: $(OBJ)/$(PROJECTNAME).o $(OBJ)/test.o
	$(CXX) $(CXXFLAGS) -o $(EXC)/$(OUTPUT_NAME) $(OBJ)/$(PROJECTNAME).o $(OBJ)/test.o

$(OBJ)/$(PROJECTNAME).o: src/$(PROJECTNAME).cpp
	$(CXX) $(CXXFLAGS) -o $@ -c src/$(PROJECTNAME).cpp

$(OBJ)/test.o: src/test.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c src/test.cpp

clean:
	$(RM) $(OBJ)/$(PROJECTNAME).o $(OBJ)/test.o

run: $(EXC)/$(OUTPUT_NAME)
	./$(EXC)/$(OUTPUT_NAME)
