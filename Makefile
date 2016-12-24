
all: test 

test.c: test/test.c 
	gcc -Wall -Isrc -o bin/test test/test.c 

test: test.c
	@if ./bin/test ; then \
		rm -f ./bin/test; \
		echo "TESTS PASSED"; \
	else \
		rm -f ./bin/test; \
		echo "TESTS FAILED"; exit 1; \
	fi; \

example: bin/example.c 
	gcc -Wall -Isrc -o bin/example bin/example.c 

clean:
	rm -f bin/example bin/test

.PHONY: all test clean
