CXXFLAGS=-I include/

%.o: %.cc
	$(CXX) -c -o $@ $< $(CXXFLAGS)

source: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f */*.o source