cmake_minimum_required(VERSION 2.8)

include_directories(./inc ./src ../../typedef/inc ../../netstream/inc ../../share/inc ../../logger/inc ../../scene/inc ../../gamelogic/inc ../../common/cachesys/inc ../../common/material-cpp ../../../../common/inc ../../cache/inc ../../../../common/material-cpp ../../terminal/inc ../../simplexml/inc $ENV{ACE_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC})
aux_source_directory(./src SRC_FILE)
add_library(ManageShort SHARED ${SRC_FILE})