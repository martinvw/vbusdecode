# Source files to either reference or create
SOURCES = vbusdecode.cpp
# The executable file that will be created at the end
EXEC = target/vbusdecode
# The flags to use for compilation
FLAGS =
# The code compiler to use for compilation
CC = g++

all: clean compile test install

clean:
	rm -rf target *.o vbusdecode

compile:
	mkdir -p target
	$(CC) $(SOURCES) $(FLAGS) -o $(EXEC)

test: test1 test_negative

install:

test1:
	cat examples/raw.log | ./target/vbusdecode -s 0x4221 -c 1 0,15,0.1,f 2,15,0.1 4,15,0.1 6,15,0.1 8,7,1 9,7,1 20,16,1,p 22,16,1000,p 24,16,1000000 12,16,0,t 10,1,0,y 10,1,1,l > target/output.txt
	diff test/output.txt target/output.txt

test_negative:
	cat examples/raw2.log | ./target/vbusdecode -s 0x7751 -c 1 0,15,0.1 > target/output2.txt
	diff test/output2.txt target/output2.txt
