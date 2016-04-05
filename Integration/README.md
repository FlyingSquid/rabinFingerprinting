#Integration

This directory holds an example of integrating Rabin Fingerprinting with a remote proxy.

## The idea


                      |rc| <-blocks-- |rs|
                       |               ^
                       v               |
    |-------| <-resp- |--|           |--| <-resp-|-----| <-
    |Browser|         |p1|           |p2|        |Proxy|
    |-------| -req->  |--|---req-->  |--| -req-> |-----| ->

Here, `rc` and `rs` represent the Rabin Client and Rabin Server respectively.

- The request from the browser is forwarded by p1 to p2 to the proxy. The proxy's response is written to p2. This is written to the rabin server, which transfer information to the rabin client. Here, it is re-assembled and written to p1 which forwards i to the browser.

- p1 is ideally implemented on the same machine as the Browser and p2 on the same machine as the proxy.

- The connection between `rc` and `rs` is permanently open for now. This may have to be improved.



## A Quick Note

The proxy used in this example deals only with `HTTP GET` requests. A more
feature-rich proxy can be used instead of the `proxy` binary.

## Setup

Compile using the makefile. Now run binaries in this order:

- Run `proxy` with a port number
- Run `p2` with a port to be open at, `proxy`'s host name, `proxy`'s port number and a port number for the rabin connection.
- Run `p1` with a port number to be open at, `p2`s host name, `p2`'s port number and `p2`'s rabin port number.
- Point you browser at `p1` (hostname and port number) to use the system.

##Current issue

- Issues with blocking read calls from `proxy` to `p2`.


