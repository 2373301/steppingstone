cmake_minimum_required(VERSION 2.8)

include_directories(../../../common/material-cpp ../common/material-cpp ../common/cachesys/inc $ENV{PROTOBUF_INC})
aux_source_directory(../common/material-cpp/entity ENTITY_SRC)
aux_source_directory(../../../common/material-cpp/protocol PROTOCOL_SRC)
aux_source_directory(../common/material-cpp/protocol INNER_PROTOCOL_SRC)
add_library(protocol SHARED ${ENTITY_SRC} ${PROTOCOL_SRC} ${INNER_PROTOCOL_SRC})