# cb0r - Minimal Zero-Footprint [CBOR](http://cbor.io) Decoder in C

> NOTE - this code is not functioning yet, consider using [cn-cbor](https://github.com/cabo/cn-cbor) until I find more time/demand to get this working

A one-pass minimal overhead walk of the raw bytes using a fast jump table to determine state transitions.

* public domain, single cb0r.c file with one function
* easy to use, just returns a given type, value, and offset locations in the bytes
* no dependencies, embedded friendly
* requires no memory/malloc or copying, uses only what is passed in
* safely stops on anything bad

For other CBOR implementations check [http://cbor.io/impls.html](http://cbor.io/impls.html).

## Usage

`uint8_t *cb0r(uint8_t *start, uint8_t *stop, uint32_t skip, cb0r_t result)`

* processes one item by default into result (if provided)
* returns end pointer to byte following the item value (may be == stop for complete, > stop for item value length overflow)
* can very efficiently skip any number of leading items of any type
* see [header](src/cb0r.h) for result structure and types
* indefinite length types will be resolved/counted before returning
