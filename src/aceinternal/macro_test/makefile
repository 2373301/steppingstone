#######
C++ := g++
CCFLAG := -g -c -o
CCINCLUDE := -I$(ACE_ROOT)
ACE_LIB := -lACE

SRC_CPP := test.cpp


test : $(SRC_CPP)
	$(C++)  -g $(CCINCLUDE) -o $@ $^ $(ACE_LIB)

.PHONY : clean
clean:
	rm -f *.o test 
	
	
