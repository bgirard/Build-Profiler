SRC = src/forktrace.cc

all: bin/libmtrace.dylib bin/simple_test

bin/libmtrace.dylib: $(SRC)
	$(CC) -framework QuartzCore -arch i386 -arch x86_64 -flat_namespace -dynamiclib $(SRC) -o bin/libmtrace.dylib

bin/simple_test: test/simple_test.cc
	$(CC) test/simple_test.cc -o bin/simple_test
