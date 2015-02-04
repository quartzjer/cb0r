// key = string to match or null
// klen = key length (or 0), or if null key then len is the array offset value
// cbor = raw bytes to decode
// clen = length of bytes
// vlen = where to store return value length
// returns pointer to value and sets len to value length, or 0 if not found
// any parse error will set vlen to the position of the error
uint8_t *cb0r(char *key, size_t klen, uint8_t *cbor, size_t clen, size_t *vlen)
