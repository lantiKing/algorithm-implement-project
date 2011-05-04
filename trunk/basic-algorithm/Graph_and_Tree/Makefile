CC=gcc


SRC = Binary-Tree Breadth-first-Search 

EXE = $(SRC:%=%.exe)

%.exe:
	@echo "---------"
	gcc -o $@ $(basename $@).c

all:$(EXE)

clean:
	rm *.exe
