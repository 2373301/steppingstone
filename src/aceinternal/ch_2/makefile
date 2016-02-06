#make file
##
##
C++:=g++
GCC:=gcc
CCFLAG:=-g 
CCINCLUDE:=-I$(ACE_ROOT)
SOFLAG:=-g -shared -fPIC
LDFLAG := -Wl,-rpath,. -L. -lACE -lpthread  -lstdc++ -lacceptor -lhandle_data

vpath %.cpp .
vpath %.h .
vpath %.so .

LIBS:=libacceptor.so libhandle_data.so


BINS:=server



all:$(LIBS) $(BINS)


libacceptor.so: acceptor.cpp
	$(C++) $(SOFLAG) -o $@ $^ $(CCINCLUDE)

libhandle_data.so: handle_data.cpp 
	$(C++) $(SOFLAG) -o $@ $^ $(CCINCLUDE) 


server:server.cpp
	$(C++) $(CCFLAG) $(LDFLAG) -o $@ $^	


.PHONY : clean
clean:
	rm -f *.so 
	rm server

