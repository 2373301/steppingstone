python convert_opcode_to_name.py .\inc\ .\inc\opcode.h
move /Y .\protocol_dec.h .\inc\ 
copy /Y .\inc\opcode.xml ..\server\bin\tools\opcode.xml
pause