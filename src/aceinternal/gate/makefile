################################################################################
## Makefile for HISO CACHE service by  Jacky Wang

CC = g++
CFLAGS = -w -Wno-deprecated -g
INC = -I../../dep/easyxml -I./src/client -I./src/gameserver -I./src -I./inc -I../hiso/shared/inc -I../../dep/easyxml/inc -I../../dep/protobuf/src -I../common -I../common/gate/template/cpp/protocol -I../common/login/template/cpp/protocol -I../../../common/inc -I../../../common/template/cpp -I../../../common/template/cpp/protocol -I./src/roleserver -I./src/net -I./src/login -I./src/stat -I../../../common/material-cpp -I../share/inc -I../logger/inc -I../simplexml/inc -I$(ACE_INC) -I$(BOOST_INC) -I$(PROTOBUF_INC)

LIB = -L$(ACE_LIB) -lACE -lpthread -L$(PROTOBUF_LIB) -lprotobuf -L$(BOOST_LIB) -lboost_system -lboost_filesystem -lboost_date_time -L../../bin -lshare -llogger -lsimplexml
CXXFLAGS = $(CFLAGS) $(INC)
TARGET = gate
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
	cp -f ../../../common/material-cpp/protocol/msg_login.pb.cc ./src/msg_login.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_player.pb.cc ./src/msg_player.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_gate.pb.cc ./src/msg_gate.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_error.pb.cc ./src/msg_error.pb.cpp
