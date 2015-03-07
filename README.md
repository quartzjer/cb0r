# cb0r - Minimal Zero-Footprint [CBOR](http://cbor.io) Decoder in C

> NOTE - this code is not functioning yet, consider using [cn-cbor](https://github.com/cabo/cn-cbor) until I find more time/demand to get this working

A one-pass minimal overhead walk of the raw bytes, using each one as an index into a jump table to determine state transitions.

* public domain, single cb0r.c file with one function
* easy to use, just returns a given type, value, and offset locations in the bytes
* no dependencies, embedded friendly
* requires no memory/malloc or copying, uses only what is passed in
* safely stops on anything bad

For other CBOR implementations check [http://cbor.io/impls.html](http://cbor.io/impls.html).

## Usage

`size_t cb0r(uint8_t *raw, size_t len, uint8_t *type, uint64_t *val, size_t *start)`

* always returns length of whole value decoded, add to raw for offset to next value
* see [header](src/cb0r.h) for types returned
* most types set val, may need to cast for some
* variable types set start to the beginning of the data and val to the length
* indefinite length types will be resolved/counted before returning


```c
uint8_t *cb0r(raw, len, &type, &val, &start);
// type is int, uint, byte, char, array, map, simple, float, break
// val is actual value for int/uint/float, len for byte/char/array/map and sets start
// simple type set value in size, bool/null/undef
// returns next item in raw stream
```
