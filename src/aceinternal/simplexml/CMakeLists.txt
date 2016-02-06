cmake_minimum_required(VERSION 2.8)

include_directories(./inc ./src ./tinyxml ../share/inc $ENV{ACE_INC} $ENV{BOOST_INC} $ENV{PROTOBUF_LIB})
aux_source_directory(./src SRC_FILE)
aux_source_directory(./tinyxml TINYXML_FILE)
add_library(simplexml SHARED ${SRC_FILE} ${TINYXML_FILE})