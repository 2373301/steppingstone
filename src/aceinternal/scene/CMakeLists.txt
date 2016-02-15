cmake_minimum_required(VERSION 2.8)

include_directories(./inc ./src ./tinyxml ../share/inc ../netstream/inc ./ ../logger/inc ../../../common/inc ../../../common/material-cpp ../common/cachesys/inc ../cache/inc ../simplexml/inc ../terminal/inc ../gamelogic/inc ../gamelogic/src/resource ../common/material-cpp ../common/cachesys/inc $ENV{ACE_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC})
aux_source_directory(./src SRC_FILE)
add_library(scene SHARED ${SRC_FILE})