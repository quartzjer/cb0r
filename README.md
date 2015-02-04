# cb0r - Minimal Zero-Footprint [CBOR](http://cbor.io) Decoder in C

> note - this code is not functioning yet

A one-pass minimal overhead walk of the raw bytes, using each one as an index into a jump table to determine state transitions.

* public domain, single cb0r.c file with one function (based on [js0n](https://github.com/quartzjer/js0n))
* easy to use, just returns a given value location in the CBOR bytes
* no dependencies, embedded friendly
* requires no memory/malloc or copying, uses only what is passed in
* optimized for high speed scanning/routing of small chunks of data, stops scanning upon match
* safely errors on anything bad

For other CBOR implementations check [http://cbor.io/impls.html](http://cbor.io/impls.html), and in C there is one from [RIOT-OS](https://github.com/RIOT-OS/RIOT/tree/master/sys/cbor) and [upwhere](https://github.com/upwhere/ccbor).

## Usage

> wip

