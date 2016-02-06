################################################################################
## Makefile for HISO CACHE service by  Jacky Wang

CC = g++
CFLAGS = -fPIC -w -Wno-deprecated -g
INC = -I$(ACE_INC) -I$(BOOST_INC) -I$(PROTOBUF_INC) -I./inc -I./src -I./tinyxml -I../share/inc -I../logger/inc

LIB = -L$(ACE_LIB) -lACE -lpthread -L$(PROTOBUF_LIB) -lprotobuf -L$(BOOST_LIB) -lboost_system -lboost_filesystem -L../../bin -lshare -llogger
CXXFLAGS = $(CFLAGS) $(INC)
TARGET = libnetcore.so
SRCS = $(wildcard src/*.cpp src/tinyxml/*.cpp)
OBJS = $(SRCS:.cpp=.o)

$(TARGET) : $(OBJS)
	$(CC) -fPIC -shared $(OBJS) $(LIB) -o $(TARGET)
	cp -f $(TARGET) ../../bin
clean :
	rm -f ./$(TARGET)
	rm -f ../../bin/$(TARGET)
	rm -f ./src/*~ $(OBJS)
