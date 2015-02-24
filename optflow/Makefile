CFLAGS= $(shell pkg-config --cflags opencv)
LIBS= $(shell pkg-config --libs opencv)

#SRCS= optflow_demo.cpp
#TARGET= optflow_demo
SRCS= optflow.cpp
TARGET= optflow
all: $(SRCS)
	g++ -o $(TARGET) $(SRCS) -L/usr/local/cuda-6.5/lib64 $(CFLAGS) $(LIBS)
clean:
	rm -rf $(TARGET) *.o
