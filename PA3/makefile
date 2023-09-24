CC = g++
OPTFLAGS = -O3

all	: bin/cb 
#optimized version:
bin/cb	:src/cb.cpp
		$(CC) $(OPTFLAGS) $< -o bin/cb

#DEBUG Version:
#dbg 	:bin/cb_dbg

#bin/cb_dbg	:$(CC) -g -D_DEBUG_ON_ -o bin/cb_dbg

clean:
	rm -rf *.o bin/*