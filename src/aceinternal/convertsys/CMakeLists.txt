cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(/usr/include/mysql ./inc ./protocal_factory ./protocal ./src ./src ./src/serialize ../../common/cachesys/inc ../../../../common/inc ./tinyxml ../../share/inc ../../netcore/inc ../../logger/inc ../../simplexml/inc ../../../../common/material-cpp ../../common/material-cpp ../../common/cachesys/inc $ENV{ACE_INC} $ENV{MYSQLPP_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC})
link_directories(../../../bin $ENV{ACE_LIB} $ENV{BOOST_LIB} $ENV{PROTOBUF_LIB} $ENV{MYSQLPP_LIB})

aux_source_directory(../../common/material-cpp/entity ENTITY_SRC)

configure_file(../../common/material-cpp/protocol/msg_binary.pb.cc ./src/msg_binary.pb.cpp)
configure_file(../service/protocal/cache_auto_assistant.h ./src)
configure_file(../service/protocal/cache_auto_assistant.cpp ./src)
configure_file(../service/protocal_factory/factory.h ./src)
configure_file(../service/protocal_factory/factory.cpp ./src)
configure_file(../service/src/serialize/Serialize.h ./src)
configure_file(../service/src/serialize/Serialize.cpp ./src)
configure_file(../service/src/serialize/SerializeFactory.h ./src)
configure_file(../service/src/serialize/SerializeFactory.cpp ./src)
configure_file(../service/src/serialize/sqlite3.h ./src)
configure_file(../service/src/serialize/sqlite3.c ./src)
configure_file(../service/src/serialize/SQLiteSerialize.h ./src)
configure_file(../service/src/serialize/SQLiteSerialize.cpp ./src)
configure_file(../service/inc/cached.h ./src)

aux_source_directory(./src SRC_FILE)

add_executable(convertsys ${SRC_FILE} ${ENTITY_SRC})

target_link_libraries(convertsys ACE)
target_link_libraries(convertsys pthread)
target_link_libraries(convertsys mysqlpp)
target_link_libraries(convertsys mysqlclient)
target_link_libraries(convertsys protobuf)

target_link_libraries(convertsys boost_system)
target_link_libraries(convertsys boost_filesystem)
target_link_libraries(convertsys boost_date_time)

target_link_libraries(convertsys share)
target_link_libraries(convertsys logger)
target_link_libraries(convertsys netcore)
target_link_libraries(convertsys simplexml)
