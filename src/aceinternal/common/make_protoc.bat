
protoc.exe --proto_path=material --cpp_out=material-cpp material/protocol/msg_guid.proto
protoc.exe --proto_path=material --cpp_out=material-cpp material/protocol/msg_cache.proto
protoc.exe --proto_path=material --cpp_out=material-cpp material/protocol/msg_binary.proto


protoc.exe --proto_path=material --cpp_out=material-cpp material/entity/test.proto
protoc.exe --proto_path=material --cpp_out=material-cpp material/entity/player.proto



pause
