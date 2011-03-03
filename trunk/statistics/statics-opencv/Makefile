
EXE=main

UTIL_DIR=.

SOURCE=$(EXE).cpp $(UTIL_DIR)/cv_statics.cpp

LIBRARY=`pkg-config --libs opencv_noqt`
INCLUDE=`pkg-config --cflags opencv_noqt` -I$(UTIL_DIR)
FRAMEWORK=#-framework OpenCV

DEFINE=#-DDEBUG

CXX=g++ #-arch `arch`

all:
	$(CXX) $(SOURCE) $(DEFINE) $(LIBRARY) $(INCLUDE) $(FRAMEWORK) -o $(EXE)

clean:
	rm -rf $(EXE)
