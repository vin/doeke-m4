LDFLAGS=-lSDL
CXXFLAGS=-Wall -Werror

sources = $(wildcard *.cpp)

m4: main.o gfx.o plotter.o palette.o input.o
	$(LINK.cc) $(OUTPUT_OPTION) $^

clean:
	rm -f *.o m4

%.d: %.cpp
	$(COMPILE.cpp) -M $(OUTPUT_OPTION) $^

include $(sources:.cpp=.d)
