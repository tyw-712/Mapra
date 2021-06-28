CXX = g++
CXXFLAGS = -std=c++17 -DSET_FORMAT -Wall -Wextra -Wpedantic -O2
.PHONY: clean

A4: mapraview.o htree.o io.o GreyScale.o Filter.o unit.o
	$(CXX) $(CXXFLAGS) -o $@ mapraview.o htree.o io.o GreyScale.o Filter.o unit.o
# T: test.o io.o GreyScale.o Filter.o 
# 	$(CXX) $(CXXFLAGS) -o $@ test.o io.o GreyScale.o Filter.o 
test.o: test.cpp GreyScale.h
	$(CXX) $(CXXFLAGS) -c $<	
mapraview.o: mapraview.cpp GreyScale.h
	$(CXX) $(CXXFLAGS) -c $<
htree.o: htree.cpp htree.h
	$(CXX) $(CXXFLAGS) -c $<
io.o: io.cpp GreyScale.h htree.h
	$(CXX) $(CXXFLAGS) -c $<	
GreyScale.o: GreyScale.cpp GreyScale.h 
	$(CXX) $(CXXFLAGS) -c $<
Filter.o: Filter.cpp GreyScale.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f A4 mapraview.o GreyScale.o Filter.o test.o T
