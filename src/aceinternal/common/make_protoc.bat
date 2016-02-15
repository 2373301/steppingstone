for %i in (./material/protocol/*.proto) do protoc.exe --proto_path=material --cpp_out=material-cpp ./material/protocol/%i

for %j in (./material/entity/*.proto) do protoc.exe --proto_path=material --cpp_out=material-cpp ./material/entity/%j 

pause
