import sys
import os
from distutils.file_util import *

def convertFileName(file_name, output_path):
    print file_name, output_path
    fn = file_name
    pos = file_name.rfind('\\')
    if (pos >= 0):
        fn = file_name[pos + 1:]
    pos = fn.rfind('.')
    if pos >= 0:
        return output_path + fn[:pos] + '.xml'
    else:
        return output_path + fn + '.xml'

def getNextEnum(lines, pos):
    begin_pos = -1
    end_pos = -1
    for i in range(pos, len(lines)):
        if lines[i].find('enum') >= 0:
            begin_pos = i
        elif lines[i].find('}') >= 0:
            end_pos = i
            break
    return end_pos, lines[begin_pos : end_pos + 1]

def convertValue(c_value):
    pos = c_value.find('0x')
    if pos >= 0:
        return str(int(c_value[pos + 2 : ], 16))
    else:
        return c_value.strip()
    
def convertNote(note_value):
    begin_pos = note_value.find('/')
    if begin_pos >= 0:
        end_pos = begin_pos
        for i in range(begin_pos, len(note_value)):
            if note_value[i] == '/':
                end_pos = i
            else:
                break
        return note_value[:begin_pos] + '// ' + note_value[end_pos + 1 :]
    else:
        if len(note_value.strip()) > 0:
            return '//    ' + note_value
        else:
            return note_value

def convertOneLine(line, current_index):
    is_note = False
    if len(line) == 0:
        is_note = True
    elif line[0] == '/':
        is_note = True
    pos = line.find(',')
    param_name = ''
    param_value = str(current_index)
    param_note = ''
    equal_pos = line.find('=')
    if not is_note:
        if equal_pos >= 0:
            param_name = line[:equal_pos]
            if pos >= 0:
                param_value = line[equal_pos + 1 : pos]
            else:
                param_value = line[equal_pos + 1 : ]
        elif pos >= 0:
            param_name = line[:pos]
            param_value = str(current_index + 1)
            param_note = line[pos + 1 : ]
        else:
            param_name = line
            param_value = str(current_index + 1)

    else:
        param_note = line
    param_name = param_name.strip()
    param_value = param_value.strip()
    param_value = convertValue(param_value)
    param_note = convertNote(param_note.strip())

    return int(param_value), param_name, param_value, param_note
        
def convertEnumToFile(file_handle, lines):
    if len(lines) <= 0:
        return 
    
    pos = lines[0].find('enum')
    current_index = 0
    param_names = []
    param_values = []
    param_notes = []

    for i in range(0, len(lines)):
        if 0 == i:
            enum_name = lines[0][pos + 5:]
#            file_handle.write('\n//#########################    ' + enum_name.strip() + '    ###########################\n')
        else:
            if lines[i].find('{') >= 0:
                continue
            if lines[i].find('}') >= 0:
                break
            current_index, name, value, note = convertOneLine(lines[i].strip(), current_index)
#            if (len(name) > 0):
#               name = name
            name.lower();
            param_names.append(name)
            param_values.append(value)
#            param_notes.append(note)
    name_max_len = 0
    for i in range(0, len(param_names)):
        name_max_len = max(name_max_len, len(param_names[i]))

    write_cpp_inc_f = open('protocol_dec.h', 'w+')
    
    file_handle.write('<?xml version="1.0" encoding="utf-8" ?>\n')
    file_handle.write('<root>\n')
    name_max_len = name_max_len + 4
    #print 'param_names ', param_names
    for i in range(0, len(param_names) - 1):
        #print 'conternt ', i
        if len(param_names[i]) == 0:
            pass
        elif int(param_values[i]) > 0: 
			if param_names[i] == 'CMSG_VALIATION_ACCOUNT':
				pname = 'cmsg_validate_account'
				file_handle.write('\t<message opcode=\'' + param_values[i].lower() + '\' name=\'' + pname + '\' />\n')
				write_cpp_inc_f.write('typed::protocol::' + pname + (30 - len(pname)) * ' ' + '\t' + pname + '_;\n')
			else:
				file_handle.write('\t<message opcode=\'' + param_values[i].lower() + '\' name=\'' + param_names[i].lower() + '\' />\n')
				write_cpp_inc_f.write('typed::protocol::' + param_names[i].lower() + (30 - len(param_names[i])) * ' ' + '\t' + param_names[i].lower() + '_;\n')
				
    file_handle.write('</root>')

    write_cpp_inc_f.close()
    
def convertEnum(file_name, output_path):
    try:
        f_read = open(file_name, 'r')
        write_file_name = convertFileName(file_name, output_path)

        f_write = open(write_file_name, 'w+')

#        f_write.write('#! /usr/bin/env python\n')
#        f_write.write('#coding=utf-8\n')

        lines = f_read.readlines()
        enum_pos = -1
        while True:
            enum_pos, enum_lines = getNextEnum(lines, enum_pos + 1)
            if enum_pos >= 0:
                convertEnumToFile(f_write, enum_lines)
            else:
                break
        f_read.close()
        f_write.close()
        
        return write_file_name
    except IOError, e:
        print 'the file does not exist ', file_name, e
        return None
    
if __name__ == '__main__':
    if len(sys.argv) < 3: 
        print 'please input output path, opcode file as argv'
    else:
        file_list = sys.argv
        del file_list[0]
        output_path = file_list[0]
        del file_list[0]
        
        for f in file_list:
            print '******start to convert file:', f
            convert_file = convertEnum(f, output_path)
            print '******end to convert file: ', f
            if convert_file != None:
#                if os.path.exists(output_path + os.path.basename(convert_file)):
#                    os.remove(output_path + os.path.basename(convert_file))
#                move_file(convert_file, output_path)
                pass
            print ''
    
