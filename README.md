# Rabin Fingerprinting


This repository holds a library that implements a set of functions that allow for partial file caching across a network. It utilizes a djb2-based fingerprinting system and TCP.


## Protocol

The custom protocol implemented is described in the paper [Value Based Web Caching](http://www.cs.berkeley.edu/~brewer/papers/vbwc.pdf).

This implementation is hidden from the end user. `block_desc` structs are used to implement custom headers, and files are sent as character blocks.


## Use

API's for both server and client-side implementations are provided in `rabinclient.h` and `rabinserver.h` respectively.






