
all: test_mystring test_slist isbalanced countwords test_bits test_mysort

test_mystring: mystring.c test_mystring.c
	gcc -g -o test_mystring mystring.c test_mystring.c

test_slist: test_slist.c slist.c
	gcc -g -o test_slist test_slist.c slist.c

isbalanced: isbalanced.c
	gcc -g -o isbalanced isbalanced.c

countwords: countwords.c
	gcc -g -o countwords countwords.c -std=gnu99

test_bits: test_bits.c bits.c
	gcc -std=gnu99 -g -o test_bits test_bits.c bits.c

test_mysort: test_mysort.c mysort.c
	gcc -std=gnu99 -g -o test_mysort test_mysort.c mysort.c
	
org:
	cp test_mystring test_mystring.org
	cp test_slist test_slist.org
	cp isbalanced isbalanced.org
	cp countwords countwords.org

clean:
	rm -f test_mystring isbalanced test_slist countwords *.out

