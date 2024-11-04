TARGET = cooke_analyzer

CXX = g++
CXXFLAGS = -std=c++17 -Wall

$(TARGET): project1.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) project1.cpp

clean:
	rm -f $(TARGET)
