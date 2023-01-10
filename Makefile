SHELL=/bin/sh
CC=g++
CFLAGS=-std=c++17
OS=$(shell uname)
ifeq ($(OS), Linux)
    CHECK_FLAGS=-lgtest -pthread -lrt -lm -lsubunit
else
    CHECK_FLAGS=-lgtest
endif
GCOVFLAGS=--coverage
HTML_CREATE=lcov --no-external -t a.out -o rep.info -c -d .; genhtml -o report rep.info
BUILD=mlp_build
HOME=MLP
TEST_COMPILE= $(CC) $(CFLAGS) test.cpp $(HOME)/Model/*.cpp $(CHECK_FLAGS)

all: install tests

install: uninstall 
	mkdir $(BUILD)
	cd $(HOME); qmake
	make -C ./$(HOME)
	cd $(HOME); mv moc_* ui_* $(HOME).app Makefile ../$(BUILD)
	cd $(HOME); mv *.o ../$(BUILD); rm .qmake.stash

uninstall:
	rm -rf $(BUILD)

start:
	./$(BUILD)/$(HOME).app/Contents/MacOS/$(HOME)

tests:
	$(TEST_COMPILE)
	./a.out
	make clean

dvi:
	makeinfo --html doc.texi
	open doc/index*

dist:
	make clean
	tar -czvf $(HOME).tar.gz Makefile $(HOME) test.cpp doc.texi 

linter: 
	cp ../materials/linters/.clang-format ./
	clang-format -n ./$(HOME)/*/*.h
	clang-format -n ./$(HOME)/*/*.cpp
	rm .clang-format

clean:
	rm -rf doc *.g* *.info *.out build* *.txt *.clang-format
