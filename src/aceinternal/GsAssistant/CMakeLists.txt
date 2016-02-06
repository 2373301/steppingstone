cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(../../src/share/inc ../../src/netcore/inc ../../src/simplexml/inc ../../src/logger/inc ../../../common/inc ../../src/common/cachesys/inc ../../src/common/material-cpp ../../../common/material-cpp $ENV{ACE_INC} $ENV{MYSQLPP_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC})
link_directories(../../bin $ENV{ACE_LIB} $ENV{BOOST_LIB} $ENV{PROTOBUF_LIB} $ENV{MYSQLPP_LIB})

aux_source_directory(./src SRC_FILE)

add_executable(gsassistant ${SRC_FILE})
target_link_libraries(gsassistant ACE)
target_link_libraries(gsassistant pthread)

target_link_libraries(gsassistant boost_system)
target_link_libraries(gsassistant boost_filesystem)
target_link_libraries(gsassistant boost_date_time)

target_link_libraries(gsassistant share)
target_link_libraries(gsassistant logger)
target_link_libraries(gsassistant netcore)
target_link_libraries(gsassistant simplexml)
target_link_libraries(gsassistant protobuf)