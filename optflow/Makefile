CFLAGS= $(shell pkg-config --cflags opencv)
LIBS= $(shell pkg-config --libs opencv)

SRCS= optflow.cpp
TARGET= optflow
all: $(TARGET)
	g++ $(CFLAGS) -L/usr/local/cuda-6.5/lib64 $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -rf $(TARGET) *.o
