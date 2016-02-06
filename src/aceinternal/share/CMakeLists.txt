cmake_minimum_required(VERSION 2.8)

include_directories(./inc ./src ../logger/inc $ENV{ACE_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_INC})
aux_source_directory(./src SRC_FILE)
add_library(share SHARED ${SRC_FILE})
