###############################################################################
## Makefile for HISO CACHE service by  Jacky Wang

CC = g++
CFLAGS = -w -Wno-deprecated -g
TARGET = cache
SQLITE_LIB = libsqlite.a

INC = -I/usr/include/mysql -I./inc -I./protocal_factory -I./protocal -I./src -I./src/serialize -I../../common/cachesys/inc -I../../../../common/inc -I./tinyxml -I../../share/inc -I../../netcore/inc -I../../logger/inc -I../../simplexml/inc -I../../../../common/material-cpp -I../../common/material-cpp -I$(ACE_INC) -I$(MYSQLPP_INC) -I$(BOOST_INC) -I$(PROTOBUF_INC)
LIB = -L$(ACE_LIB) -lACE -L$(BOOST_LIB) -lpthread -L$(MYSQLPP_LIB) -lmysqlpp -lmysqlclient -L$(PROTOBUF_LIB) -lprotobuf -L$(BOOST_LIB) -lboost_system -lboost_filesystem -lboost_date_time -L../../../bin -lshare -llogger -lsimplexml -lnetcore -L./ -lsqlite
CXXFLAGS = $(CFLAGS) $(INC)
SRCS = $(wildcard protocal/*.cpp src/*.cpp protocal_factory/*.cpp src/serialize/*.cpp) 
OBJS = $(SRCS:.cpp=.o)

$(TARGET) : $(OBJS) $(SQLITE_LIB)
	$(CC) $(OBJS) $(LIB)   -o  $(TARGET)
	cp -f ./$(TARGET) ../output
	
$(SQLITE_LIB):
	gcc -c src/serialize/sqlite3.c
	ar cqs $(SQLITE_LIB) sqlite3.o
	
clean : 
	rm -f ./src/*.o ./src/*~ ./src/core.* ./protocal/*.o ./protocal/*~ ./protocal/core.*
	rm -f ./$(TARGET)
	rm -f ../output/$(TARGET)
prepare:
	cp -f ../../../../common/material-cpp/entity/player.pb.cc ./src/player.pb.cpp
	cp -f ../../../../common/material-cpp/entity/form.pb.cc ./src/form.pb.cpp
	cp -f ../../common/material-cpp/protocol/msg_binary.pb.cc ./protocal/msg_binary.pb.cpp
	cp -f ../../common/material-cpp/protocol/msg_cache.pb.cc ./protocal/msg_cache.pb.cpp
