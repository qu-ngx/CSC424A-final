# simple-map-go-ver

## Part 1: C to Go transition

This project started from a C hash map and changed to Go while sharing the same behavior in Go.
The core design stayed the same (string keys, bucket array, linked-list collision handling, resize when full), but implementation details changed because of Go's different behaviors.

## How to test the Go version

Run all Go tests in the project:

```bash
make test
```

Or run only the map package tests directly:

```bash
go test ./simplemap -v
```

All tests should `PASS` and an `ok` should show for the package checker.

## Part 2: How to run benchmarks

```bash
make bench-vs-c
```

You can also run them separately:

```bash
make bench-c
make bench
```

## How to interpret the results from benchmarks

There are 3 workloads:
- `Fill`: insert many keys into an empty map.
- `Lookup`: repeated `Get` on a pre-filled map.
- `Mixed`: alternating `Set` and `Get`.

Main metrics:
- `ns/op`: nanoseconds per operation. **Lower is faster**.
- `B/op` (Go only): bytes allocated per operation. **Lower is better**.
- `allocs/op` (Go only): allocations per operation. **Lower is better**.

Benchnark Reading Example:
- If `Lookup` in C is `40 ns/op` and Go is `60 ns/op`, then C lookup is faster in that test.
- If Go `Fill` shows high `B/op` and `allocs/op`, that means Fill spends more time/overhead in allocation.