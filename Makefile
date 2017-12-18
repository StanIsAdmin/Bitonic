all:
	mpicxx -std=c++11 -O3 bitonic.cpp -o bitonic

test:
	bash ./jobfile

clean:
	@- rm -f ./bitonic
