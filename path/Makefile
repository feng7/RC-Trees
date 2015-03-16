######################################################################
## Makefile
##
## Umut A. Acar
######################################################################

CC = g++
OPTFLAGS = -O4 -finline-functions
COPTIONS = -Wno-deprecated -Wno-unused-result -std=c++11
RCFLAGS = -DNDEBUG -DNVERBOSE 
LDOPTIONS = 
INCLUDES = -I. -I../rcTrees
CFLAGS = $(COPTIONS) $(OPTFLAGS) $(INCLUDES)
LIBS = -L.
LD = $(CC) $(LIBS) $(LDOPTIONS) 

APOBJS = Data.o Application.o

all: default

default:  Evolution
.c.o: 
	$(CC) $(CFLAGS) $(RCFLAGS) -c  $*.c

Evolution: Evolution.o libRCTrees
	$(LD) -o Evolution Evolution.o -lRCTrees

Example: Example.o libRCTrees
	$(LD) -o Example Example.o -lRCTrees

libRCTrees: $(APOBJS) 
	make rcobjs -f ../rcTrees/Makefile -C.   
	ar r libRCTrees.a $(APOBJS)      

clean:
	rm -f *.o *.a Example Evolution
