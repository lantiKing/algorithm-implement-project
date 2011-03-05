CC = gcc -O2

##OpenCVがインストールされてるとき
#OPENCV = -DOPEN_CV `pkg-config --cflags opencv` `pkg-config --libs opencv`
##OpenCVがインストールされてないとき
OPENCV = 

all:
	$(CC) Huffman.c $(OPENCV) -o Huffman

clean:
	rm Huffman
