################################################################################
## Makefile for HISO CACHE service by  Jacky Wang

CC = g++
CFLAGS = -fPIC -w -Wno-deprecated -g
INC = -I$(ACE_INC) -I$(BOOST_INC) -I$(PROTOBUF_LIB) -I./inc -I./src -I../share/inc

LIB = -L$(ACE_LIB) -lACE -lpthread -L$(PROTOBUF_LIB) -lprotobuf -L$(BOOST_LIB) -lboost_system -lboost_filesystem -lboost_date_time -L../../bin -lshare
CXXFLAGS = $(CFLAGS) $(INC)
TARGET = liblogger.so
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

$(TARGET) : $(OBJS)
	$(CC) -fPIC -shared $(OBJS) $(LIB) -o $(TARGET)
	cp -f $(TARGET) ../../bin
clean :
	rm -f ./$(TARGET)
	rm -f ../../bin/$(TARGET)
	rm -f ./src/*~ $(OBJS)
