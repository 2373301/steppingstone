cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(./src/client ./src/gameserver ./src/qq ./src ./inc ../hiso/shared/inc ../../dep/easyxml/inc ../../dep/protobuf/src ../common ../common/gate/template/cpp/protocol ../common/login/template/cpp/protocol ../../../common/inc ../../../common/material-cpp ../../../common/template/cpp/protocol ./src/roleserver ./src/net ./src/login ./src/stat ../../../common/material-cpp ../share/inc ../netstream/inc ../logger/inc ../simplexml/inc  ../common/material-cpp/protocol ../common/material-cpp $ENV{ACE_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC} $ENV{INC_OPENAPI} $ENV{INC_JSON} $ENV{INC_OAUTH} $ENV{INC_CURL} $ENV{INC_OPENSSL})

link_directories(../../bin $ENV{ACE_LIB} $ENV{BOOST_LIB} $ENV{PROTOBUF_LIB} $ENV{LIB_OPENAPI} $ENV{LIB_JSON} $ENV{LIB_OAUTH} $ENV{LIB_CURL} $ENV{LIB_OPENSSL})

configure_file(../../../common/material-cpp/protocol/msg_login.pb.cc ./src/msg_login.pb.cpp)
configure_file(../../../common/material-cpp/protocol/msg_player.pb.cc ./src/msg_player.pb.cpp)
configure_file(../../../common/material-cpp/protocol/msg_gate.pb.cc ./src/msg_gate.pb.cpp)
configure_file(../../../common/material-cpp/protocol/msg_error.pb.cc ./src/msg_error.pb.cpp)
configure_file(../../../common/material-cpp/protocol/msg_member.pb.cc ./src/msg_member.pb.cpp)
configure_file(../common/material-cpp/protocol/msg_query.pb.cc ./src/msg_query.pb.cpp)
	
aux_source_directory(./src SRC_FILE)
aux_source_directory(./src/stat SRC_STAT)
aux_source_directory(./src/gameserver SRC_GAME)
aux_source_directory(./src/login SRC_LOGIN)
aux_source_directory(./src/client SRC_CLIENT)
aux_source_directory(./src/roleserver SRC_ROLE)
aux_source_directory(./src/net SRC_NET)
aux_source_directory(./src/qq SRC_QQ)

add_executable(gatesys ${SRC_STAT} ${SRC_FILE} ${SRC_GAME} ${SRC_LOGIN} ${SRC_CLIENT} ${SRC_ROLE} ${SRC_NET} ${SRC_QQ})


target_link_libraries(gatesys libopenapi.a libjson.a liboauth.a libcurl.a libssl.a libcrypto.a)


target_link_libraries(gatesys ACE)
target_link_libraries(gatesys pthread)
target_link_libraries(gatesys protobuf)
target_link_libraries(gatesys boost_system)
target_link_libraries(gatesys boost_filesystem)
target_link_libraries(gatesys boost_date_time)
target_link_libraries(gatesys share)
target_link_libraries(gatesys simplexml)
target_link_libraries(gatesys logger)
target_link_libraries(gatesys netstream)
