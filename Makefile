all: bitonic bitonicbonus

bitonic:
	mpiCC -std=c++11 -D RANDOM_LIST=false -D SORT_FIRST=true -O3 bitonic.cpp -o bitonic

bitonicbonus:
	mpiCC -std=c++11 -D RANDOM_LIST=true -D SORT_FIRST=true -O3 bitonic.cpp -o bitonicbonus

all_local: bitonic_local bitonicbonus_local

bitonic_local:
	mpicxx -std=c++11 -D RANDOM_LIST=false -D SORT_FIRST=false -O3 bitonic.cpp -o bitonic

bitonicbonus_local:
	mpicxx -std=c++11 -D RANDOM_LIST=true -D SORT_FIRST=true -O3 bitonic.cpp -o bitonicbonus

test_all: test test_bonus

test:
	bash ./jobfile bitonic

test_bonus:
	bash ./jobfile bitonicbonus

clean:
	@- rm -f ./bitonic
	@- rm -f ./bitonicbonus