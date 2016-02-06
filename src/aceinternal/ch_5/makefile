#make file
##
##
C++:=g++
GCC:=gcc
CCFLAG:=-g 
CCINCLUDE:=-I$(ACE_ROOT)
SOFLAG:=-g -shared -fPIC
LDFLAG := -Wl,-rpath,. -L. -lACE -lpthread  -lstdc++ 

vpath %.cpp .
vpath %.h .
vpath %.so .


BINS:=server



all:$(BINS)


server:server.cpp
	$(C++) $(CCFLAG) $(LDFLAG) -o $@ $^	

.PHONY : clean
clean:
	rm -f *.so 
	rm server 	

