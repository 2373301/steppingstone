###############################################################################
## Makefile for HISO CACHE service by  Jacky Wang

CC = g++
CFLAGS = -w -Wno-deprecated -g
TARGET = convert
SQLITE_LIB = libsqlite.a

INC = -I/usr/include/mysql -I./inc -I./src -I../../common/cachesys/inc -I../../../../common/inc -I../../share/inc -I../../logger/inc -I../../simplexml/inc -I../../../../common/material-cpp -I../../common/material-cpp -I$(ACE_INC) -I$(MYSQLPP_INC) -I$(BOOST_INC) -I$(PROTOBUF_INC)
LIB = -L$(ACE_LIB) -lACE -L$(BOOST_LIB) -lpthread -L$(MYSQLPP_LIB) -lmysqlpp -L$(PROTOBUF_LIB) -lprotobuf -L$(BOOST_LIB) -lboost_system -lboost_filesystem -lboost_date_time -L../../../bin -lshare -llogger -lsimplexml -L./ -lsqlite
CXXFLAGS = $(CFLAGS) $(INC)
SRCS = $(wildcard src/*.cpp) 
OBJS = $(SRCS:.cpp=.o)

$(TARGET) : $(OBJS) $(SQLITE_LIB)
	$(CC) $(OBJS) $(LIB)   -o  $(TARGET)
	cp -f $(TARGET) ../output
	
$(SQLITE_LIB):
	gcc -c src/sqlite3.c
	ar cqs $(SQLITE_LIB) sqlite3.o
	
clean : 
	rm -f $(TARGET)
	rm -f ./src/*.o ./src/*~ ./src/core.* ./protocal/*.o ./protocal/*~ ./protocal/core.*
	rm -rf ./src/MiniDumper.*
prepare:
	cp -f ../../../../common/material-cpp/entity/player.pb.cc ./src/player.pb.cpp
	cp -f ../../../../common/material-cpp/entity/form.pb.cc ./src/form.pb.cpp
	cp -f ../../common/material-cpp/protocol/msg_binary.pb.cc ./src/msg_binary.pb.cpp
	cp -f ../service/protocal/cache_auto_assistant.* ./src
	cp -f ../service/protocal_factory/factory.* ./src
	cp -f ../service/src/serialize/Serialize.* ./src
	cp -f ../service/src/serialize/SerializeFactory.* ./src
	cp -f ../service/src/serialize/sqlite3.* ./src
	cp -f ../service/src/serialize/SQLiteSerialize.* ./src
	cp -f ../service/inc/cached.h ./src