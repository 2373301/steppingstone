cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(/usr/include/mysql ./ ./inc ./src ./src/cache ./src/checkname ./src/database ./src/gate ./src/net ../common/cachesys/inc ../../../common/inc ../share/inc ../cache/inc ../netcore/inc ../logger/inc ../simplexml/inc ../../../common/material-cpp ../common/material-cpp/entity ../../../common/material-cpp/protocol ../common/material-cpp ../common/cachesys/inc $ENV{ACE_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC} $ENV{MYSQLPP_INC})

link_directories(../../bin $ENV{ACE_LIB} $ENV{BOOST_LIB} $ENV{PROTOBUF_LIB} $ENV{MYSQLPP_LIB})

configure_file(../common/material-cpp/entity/player.pb.cc ./src/player.pb.cpp)
configure_file(../../../common/material-cpp/protocol/msg_player.pb.cc ./src/msg_player.pb.cpp)
configure_file(../../../common/material-cpp/protocol/msg_error.pb.cc ./src/msg_error.pb.cpp)
configure_file(../../../common/material-cpp/protocol/msg_gate.pb.cc ./src/msg_gate.pb.cpp)
configure_file(../../../common/material-cpp/protocol/msg_char.pb.cc ./src/msg_char.pb.cpp)
configure_file(../common/material-cpp/protocol/msg_cache.pb.cc ./src/msg_cache.pb.cpp)
configure_file(../common/material-cpp/protocol/msg_guid.pb.cc ./src/msg_guid.pb.cpp)

aux_source_directory(./src SRC_FILE)
aux_source_directory(./src/cache SRC_CACHE)
aux_source_directory(./src/checkname SRC_CHECK)
aux_source_directory(./src/database SRC_DB)
aux_source_directory(./src/gate SRC_GATE)
aux_source_directory(./src/net SRC_NET)

add_executable(charsys ${SRC_FILE} ${SRC_CACHE} ${SRC_CHECK} ${SRC_DB} ${SRC_GATE} ${SRC_NET})

target_link_libraries(charsys ACE)
target_link_libraries(charsys pthread)
target_link_libraries(charsys protobuf)
target_link_libraries(charsys boost_system)
target_link_libraries(charsys boost_filesystem)
target_link_libraries(charsys boost_date_time)
target_link_libraries(charsys simplexml)
target_link_libraries(charsys netcore)
target_link_libraries(charsys share)
target_link_libraries(charsys logger)
target_link_libraries(charsys mysqlpp)
target_link_libraries(charsys mysqlclient)