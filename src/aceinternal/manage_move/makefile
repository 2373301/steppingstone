################################################################################
## Makefile for apss terminal service by Jacky Wang

CC = g++
CFLAGS = -fPIC -w -Wno-deprecated -g
INC = -I$(ACE_INC) -I$(BOOST_INC) -I$(PROTOBUF_INC) -I./inc -I./src -I../../typedef/inc -I../../share/inc -I../../logger/inc -I../../scene/inc -I../../gamelogic/inc -I../../common/material-cpp -I../../../../common/inc -I../../cache/inc -I../../../../common/material-cpp -I../../terminal/inc -I../../simplexml/inc

LIB = -L$(ACE_LIB) -lACE -lpthread -L$(PROTOBUF_LIB) -lprotobuf -L$(BOOST_LIB) -lboost_system -lboost_filesystem -L../../../bin -lshare -llogger -lprotocol -lscene

CXXFLAGS = $(CFLAGS) $(INC)
TARGET = libManageMove.so
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

$(TARGET) : $(OBJS)
	$(CC) -fPIC -shared $(OBJS) $(LIB) -o $(TARGET)
	cp -f $(TARGET) ../../../bin/modules
clean :
	rm -f ./$(TARGET)
	rm -f ../../../bin/modules/$(TARGET)
	rm -f ./src/*~ $(OBJS)
