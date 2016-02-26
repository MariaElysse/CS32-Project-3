CC =clang++    # On lnxsrv07 or 09, replace this with /usr/local/cs/bin/g++
LIBS = -lglut -lGL -lGLU
RPATH =

# On lnxsrv07 or 09, uncommment the following line
#RPATH = -Wl,--rpath=/usr/local/cs/lib64

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

.PHONY: default all clean


all: FrackMan

%.o: %.cpp $(HEADERS)
	$(CC) -c -std=c++11 -Wall -Wextra -Wno-sign-compare -Werror=return-type  $< -o $@

FrackMan: $(OBJECTS) 
	$(CC) $(RPATH) $(OBJECTS) $(LIBS) -o $@

clean:
	rm -f *.o
	rm -f FrackMan
