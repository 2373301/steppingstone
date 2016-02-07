################################################################################
## Makefile for HISO CACHE service by Jacky Wang

CC = g++
CFLAGS = -w -Wno-deprecated -g
INC = -I./src -I./inc -I../common -I../../../common/inc -I../common/material-cpp -I../share/inc -I../logger/inc -I../simplexml/inc -I../netcore/inc -I$(ACE_INC) -I$(BOOST_INC) -I$(PROTOBUF_INC)

LIB = -L$(ACE_LIB) -lACE -lpthread -L$(PROTOBUF_LIB) -lprotobuf -L$(BOOST_LIB) -lboost_system -lboost_filesystem -lboost_date_time -L../../bin -lshare -llogger -lsimplexml -lnetcore
CXXFLAGS = $(CFLAGS) $(INC)
TARGET = guidsys
SRCS = $(wildcard src/*.cpp src/stat/*.cpp src/gameserver/*.cpp src/login/*.cpp src/client/*.cpp src/roleserver/*.cpp src/net/*.cpp)
OBJS = $(SRCS:.cpp=.o)

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) $(LIB)   -o  $(TARGET)
	cp -f $(TARGET) output/
clean :
	rm -f ./src/*~ $(OBJS)
	rm -f ./$(TARGET)
	rm -f ./src/MiniDumper.h
	rm -f ./src/MiniDumper.cpp
prepare:
	cp ../common/material-cpp/protocol/msg_guid.pb.cc ./src/msg_guid.pb.cpp