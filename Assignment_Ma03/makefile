CXX = g++
CXXFLAGS = -std=c++11

all: Gemini_version Claude_version

Gemini_version: Gemini_version.cpp
	$(CXX) $(CXXFLAGS) -o Gemini_version Gemini_version.cpp

Claude_version: Claude_version.cpp
	$(CXX) $(CXXFLAGS) -o Claude_version Claude_version.cpp

clean:
	rm -f Gemini_version Claude_version
