LIBPATH=-L.
LIBS=-lrapidcheck -lm

INCLUDES=-Iinclude

all:
	g++ position_converter.cpp $(INCLUDES) -o position_converter $(LIBPATH) $(LIBS)

clean:
	rm -f position_converter
