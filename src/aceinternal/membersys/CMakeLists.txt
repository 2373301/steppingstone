cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(/usr/include/mysql ./ ./inc ./src ./src/cache ./src/checkname ./src/database ./src/gate ./src/net ../common/cachesys/inc ../../../common/inc ../share/inc ../cache/inc ../netcore/inc ../logger/inc ../simplexml/inc ../../../common/material-cpp ../common/material-cpp/entity ../../../common/material-cpp/protocol ../common/material-cpp ../common/cachesys/inc $ENV{ACE_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC} $ENV{MYSQLPP_INC})

link_directories(../../bin $ENV{ACE_LIB} $ENV{BOOST_LIB} $ENV{PROTOBUF_LIB} $ENV{MYSQLPP_LIB})

configure_file(../../../common/material-cpp/protocol/msg_member.pb.cc ./src/msg_member.pb.cpp)

aux_source_directory(./src SRC_FILE)

add_executable(membersys ${SRC_FILE})

target_link_libraries(membersys ACE)
target_link_libraries(membersys pthread)
target_link_libraries(membersys protobuf)
target_link_libraries(membersys boost_system)
target_link_libraries(membersys boost_filesystem)
target_link_libraries(membersys boost_date_time)
target_link_libraries(membersys simplexml)
target_link_libraries(membersys share)
target_link_libraries(membersys logger)
target_link_libraries(membersys mysqlpp)
target_link_libraries(membersys mysqlclient)