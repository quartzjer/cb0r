
all: test 

test.c: test/test.c 
	gcc -Wall -Isrc -o bin/test test/test.c src/cb0r.c

test: test.c
	@if ./bin/test ; then \
		echo "TESTS PASSED"; \
	else \
		echo "TESTS FAILED"; exit 1; \
	fi; \

example: bin/example.c 
	gcc -Wall -Isrc -o bin/example bin/example.c 

clean:
	rm -f bin/example bin/test

.PHONY: all test clean
