cmake_minimum_required(VERSION 2.8)

set(CMAKE_CURRENT_BINARY_DIR ../../bin)

set(CUDA_GENERATED_OUTPUT_DIR ./build)

include_directories(./inc ./src ../share/inc ../netstream/inc ../logger/inc ../../../common/inc ../../../common/material-cpp ./src/resource ../scene/inc ../common/material-cpp ../common/cachesys/inc ../cache/inc ../terminal/inc ../simplexml/inc $ENV{ACE_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC})
aux_source_directory(./src SRC_FILE)
aux_source_directory(./src/resource RESOURCE_FILE)
add_library(gamelogic SHARED ${SRC_FILE} ${RESOURCE_FILE})
