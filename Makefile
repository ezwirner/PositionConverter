LIBPATH=-L.
LIBS=-lrapidcheck -lm

all:
	g++ position_converter.cpp -o position_converter $(LIBPATH) $(LIBS)

clean:
	rm -f position_converter
