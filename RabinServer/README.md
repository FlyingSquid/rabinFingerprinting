# RabinServer


This is a class that implements a set of functions that allow for `Value Based Web Caching` to be implemented.
It utilizes rabin fingerprinting and TCP.


## Protocol

- A `block_desc` is first sent
- A stream of bytes of the provided size is then given


## TODO

- Need to change block's implementation from `vector` to `map` or some other
structure that can deal with sparse hashes. Basically I've implemented a really dumb hash table, time to use a real one. The keys should just be block numbers. This is a simplification of the WBVC paper.

- Need to figure out how requests will work



