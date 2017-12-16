all:
	mpicxx bitonic.cpp -o bitonic

test:
	bash ./jobfile

clean:
	@- rm -f ./bitonic
