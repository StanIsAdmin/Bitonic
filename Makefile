all:
	mpicxx -std=c++11 bitonic.cpp -o bitonic

test:
	bash ./jobfile

clean:
	@- rm -f ./bitonic
