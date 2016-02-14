################################################################################
## Makefile for apss cache by Jacky Wang

CC = g++
CFLAGS = -w -Wno-deprecated -g -fPIC
INC = -I./inc -I./src -I../share/inc -I../logger/inc -I../../../common/inc -I../common/material-cpp -I../../../common/material-cpp -I../common/cachesys/inc -I$(ACE_INC) -I$(BOOST_INC) -I$(PROTOBUF_INC)

LIB = -L$(ACE_LIB) -lACE -lpthread -L$(PROTOBUF_LIB) -lprotobuf -L$(BOOST_LIB) -lboost_system -lboost_filesystem -lboost_date_time -L../../bin -lshare -llogger -lprotocol

CXXFLAGS = $(CFLAGS) $(INC) -fPIC

TARGET = libcache.so
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

$(TARGET) : $(OBJS)
	$(CC) -fPIC -shared $(OBJS) $(LIB)   -o  $(TARGET)
	cp -f $(TARGET) ../../bin
clean :
	rm -f ./src/*~ $(OBJS)
	rm -f ./$(TARGET)
	rm -f ./src/MiniDumper.h
	rm -f ./src/MiniDumper.cpp
prepare:
	ls > /dev/null