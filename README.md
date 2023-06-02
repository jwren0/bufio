# BufIO
BufIO is a small C library which implements buffered I/O.

> **Note**
>
> This library was implemented for educational purposes.
>
> C library implementations should use buffers for I/O by default, but the<br>
> buffer size may be smaller than the default used in this library (8KiB).
>
> See [this example](/examples/setvbuf.c) on modifying the default buffer size without using this library.


# Quick start
To install the library, run the following commands:
```
$ git clone https://github.com/jwren0/bufio.git
$ cd bufio/
# make install
```

# Make targets

## Building
```
$ make build
```

## Testing
```
$ make test
```

## Installing
```
# make install
```

## Uninstalling
```
# make uninstall
```
