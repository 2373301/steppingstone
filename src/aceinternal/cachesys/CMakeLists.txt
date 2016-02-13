cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(/usr/include/mysql ./inc ./protocal_factory ./protocal ./src ./src/serialize ../../common/cachesys/inc ../../../../common/inc ./tinyxml ../../share/inc ../../netcore/inc ../../logger/inc ../../simplexml/inc ../../../../common/material-cpp ../../common/material-cpp ../../common/cachesys/inc $ENV{ACE_INC} $ENV{MYSQLPP_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC})
link_directories(../../../bin $ENV{ACE_LIB} $ENV{BOOST_LIB} $ENV{PROTOBUF_LIB} $ENV{MYSQLPP_LIB})

#add_library(sqlite STATIC src/serialize/sqlite3.c)

aux_source_directory(../../common/material-cpp/entity ENTITY_SRC)

configure_file(../../common/material-cpp/protocol/msg_binary.pb.cc ./protocal/msg_binary.pb.cpp)
configure_file(../../common/material-cpp/protocol/msg_cache.pb.cc ./protocal/msg_cache.pb.cpp)

aux_source_directory(./protocal SRC_PROTOCAL)
aux_source_directory(./src SRC_FILE)
aux_source_directory(./protocal_factory SRC_FACTORY)
aux_source_directory(./src/serialize SRC_SERIALIZE)

add_executable(cachesys ${SRC_PROTOCAL} ${SRC_FILE} ${SRC_FACTORY} ${SRC_SERIALIZE} ${ENTITY_SRC})
target_link_libraries(cachesys ACE)
target_link_libraries(cachesys pthread)
target_link_libraries(cachesys mysqlpp)
target_link_libraries(cachesys mysqlclient)
target_link_libraries(cachesys protobuf)

target_link_libraries(cachesys boost_system)
target_link_libraries(cachesys boost_filesystem)
target_link_libraries(cachesys boost_date_time)

target_link_libraries(cachesys share)
target_link_libraries(cachesys logger)
target_link_libraries(cachesys netcore)
target_link_libraries(cachesys simplexml)

#target_link_libraries(cachesys sqlite)
