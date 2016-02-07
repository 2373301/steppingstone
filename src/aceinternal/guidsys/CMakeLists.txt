cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(./src ./inc ../common ../../../common/inc ../common/material-cpp ../share/inc ../logger/inc ../simplexml/inc ../netcore/inc $ENV{ACE_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC})

link_directories(../../bin $ENV{ACE_LIB} $ENV{BOOST_LIB} $ENV{PROTOBUF_LIB})

configure_file(../common/material-cpp/protocol/msg_guid.pb.cc ./src/msg_guid.pb.cpp)
	
aux_source_directory(./src SRC_FILE)

add_executable(guidsys ${SRC_FILE})

target_link_libraries(guidsys ACE)
target_link_libraries(guidsys pthread)
target_link_libraries(guidsys protobuf)
target_link_libraries(guidsys boost_system)
target_link_libraries(guidsys boost_filesystem)
target_link_libraries(guidsys boost_date_time)
target_link_libraries(guidsys simplexml)
target_link_libraries(guidsys netcore)
target_link_libraries(guidsys share)
target_link_libraries(guidsys logger)
