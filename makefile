############################################################
CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -lGL -lGLU -lglut -std=c++17 -I $(HEADERINCLUDE)
EXC = build
OBJ = obj
PROJECTNAME = opengl-template
FOLDERNAME = opengl-template
HEADERINCLUDE = /home/itsme/Desktop/git_repos/$(FOLDERNAME)/include
############################################################

OUTPUT_NAME = opengl_template

all: $(OBJ)/$(PROJECTNAME).o $(OBJ)/graphicsRendering.o $(OBJ)/fpsCounter.o
	$(CXX) $(CXXFLAGS) -o $(EXC)/$(OUTPUT_NAME) $(OBJ)/$(PROJECTNAME).o $(OBJ)/graphicsRendering.o $(OBJ)/fpsCounter.o

$(OBJ)/$(PROJECTNAME).o: src/$(PROJECTNAME).cpp
	$(CXX) $(CXXFLAGS) -o $@ -c src/$(PROJECTNAME).cpp

$(OBJ)/graphicsRendering.o: src/graphicsRendering.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c src/graphicsRendering.cpp

$(OBJ)/fpsCounter.o: src/fpsCounter.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c src/fpsCounter.cpp

clean:
	$(RM) $(OBJ)/$(PROJECTNAME).o $(OBJ)/graphicsRendering.o $(OBJ)/fpsCounter.o

run: $(EXC)/$(OUTPUT_NAME)
	./$(EXC)/$(OUTPUT_NAME)
