CXXFLAGS=-DPLATFORM_OSX -I/opt/homebrew/include
LDFLAGS=-L/opt/homebrew/lib -lglfw -lGLEW -framework OpenGL


main: main.cpp
	g++ $(CXXFLAGS) $< -o $@ $(LDFLAGS)