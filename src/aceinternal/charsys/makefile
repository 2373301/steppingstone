###############################################################################
## Makefile for HISO CACHE service by Jacky Wang

CC = g++
CFLAGS = -w -Wno-deprecated -g
TARGET = charsys

INC = -I/usr/include/mysql -I./ -I./inc -I./src -I./src/cache -I./src/checkname -I./src/database -I./src/gate -I./src/net -I../common/cachesys/inc -I../../../common/inc -I../share/inc -I../cache/inc -I../netcore/inc -I../logger/inc -I../simplexml/inc -I../../../common/material-cpp -I../../../common/material-cpp/entity -I../../../common/material-cpp/protocol -I../common/material-cpp -I$(ACE_INC) -I$(MYSQLPP_INC) -I$(BOOST_INC) -I$(PROTOBUF_INC)
LIB = -L$(ACE_LIB) -lACE -L$(BOOST_LIB) -lpthread -L$(MYSQLPP_LIB) -lmysqlpp -L$(PROTOBUF_LIB) -lprotobuf -L$(BOOST_LIB) -lboost_system -lboost_filesystem -lboost_date_time -L../../bin -lshare -llogger -lsimplexml -lnetcore -lcache -lmysqlclient
CXXFLAGS = $(CFLAGS) $(INC)
SRCS = $(wildcard src/*.cpp src/cache/*.cpp src/checkname/*.cpp src/database/*.cpp src/gate/*.cpp src/net/*.cpp) 
OBJS = $(SRCS:.cpp=.o)

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) $(LIB)   -o  $(TARGET)
	cp -f ./$(TARGET) ./output
	
clean : 
	rm -f ./src/*.o ./src/cache/*.o ./src/checkname/*.o ./src/database/*.o ./src/gate/*.o ./src/net/*.o
	rm -f src/MiniDumper.h
	rm -f src/MiniDumper.cpp
	rm -f ./$(TARGET)
	rm -f ./output/$(TARGET)
prepare:
	cp -f ../../../common/material-cpp/entity/player.pb.cc ./src/player.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_player.pb.cc ./src/msg_player.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_error.pb.cc ./src/msg_error.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_gate.pb.cc ./src/msg_gate.pb.cpp
	cp -f ../common/material-cpp/protocol/msg_cache.pb.cc ./src/msg_cache.pb.cpp
	cp -f ../common/material-cpp/protocol/msg_guid.pb.cc ./src/msg_guid.pb.cpp
