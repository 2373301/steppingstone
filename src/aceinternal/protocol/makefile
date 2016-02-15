################################################################################
## Makefile for apss cache by Jacky Wang
CC = g++
AR = ar
CFLAGS = -w -Wno-deprecated -g -fPIC
CXXFLAGS = $(CFLAGS) $(INC)
INC = -I../../../common/material-cpp -I../common/material-cpp -I$(PROTOBUF_INC)
LIB = -L$(PROTOBUF_LIB) -lprotobuf
TARGET = libprotocol.so
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

$(TARGET) : $(OBJS)
	$(CC) -fPIC -shared $(INC) -c $(SRCS)
	cp -f $(TARGET) ../../bin
clean :
	rm -rf ./src
	rm -rf ./*.o
	rm -f ./*.a
	rm -f ./src/MiniDumper.h
	rm -f ./src/MiniDumper.cpp
prepare:
	mkdir src
	cp -f ../../../common/material-cpp/entity/player.pb.cc ./src/player.pb.cpp
	cp -f ../../../common/material-cpp/entity/form.pb.cc ./src/form.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_char.pb.cc ./src/msg_char.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_enemy.pb.cc ./src/msg_enemy.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_error.pb.cc ./src/msg_error.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_form.pb.cc ./src/msg_form.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_gate.pb.cc ./src/msg_gate.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_login.pb.cc ./src/msg_login.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_player.pb.cc ./src/msg_player.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_pool.pb.cc ./src/msg_pool.pb.cpp
	cp -f ../../../common/material-cpp/protocol/msg_war.pb.cc ./src/msg_war.pb.cpp
	cp -f ../common/material-cpp/protocol/msg_guid.pb.cc ./src/msg_guid.pb.cpp
	cp -f ../common/material-cpp/protocol/msg_binary.pb.cc ./src/msg_binary.pb.cpp
	cp -f ../common/material-cpp/protocol/msg_cache.pb.cc ./src/msg_cache.pb.cpp
	cp -f ../common/material-cpp/protocol/msg_guid.pb.cc ./src/msg_guid.pb.cpp
	cp -f ../common/material-cpp/protocol/msg_plugin.pb.cc ./src/msg_plugin.pb.cpp
	