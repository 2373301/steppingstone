#make file
##
##
C++:=g++
GCC:=gcc
CCFLAG:=-g 
CCINCLUDE:=-I$(ACE_ROOT)
SOFLAG:=-g -shared -fPIC
LDFLAG := -Wl,-rpath,. -L. -lACE -lpthread 

vpath %.cpp .
vpath %.h .
vpath %.so .

LIBS:=libthis_dll.so  libthat_dll.so  
	


BINS:=dynamic



all:$(LIBS) $(BINS)


libthis_dll.so: this_dll.cpp
	$(C++) $(SOFLAG) -o $@ $^ $(CCINCLUDE)

libthat_dll.so: that_dll.cpp 
	$(C++) $(SOFLAG) -o $@ $^ $(CCINCLUDE) 

dynamic:dynamic.cpp
	$(C++) $(CCFLAG) $(LDFLAG) -o $@ $^	


.PHONY : clean
clean:
	rm -f *.so 
	rm -f dynamic

