CFLAGS= $(shell pkg-config --cflags opencv)
LIBS= $(shell pkg-config --libs opencv)

TARGET=optflow
all: optflow.cpp
	g++ $(CFLAGS) -L/usr/local/cuda-6.5/lib64 $< -o $(TARGET) $(LIBS)

clean:
	rm -rf $(TARGET) *.o
