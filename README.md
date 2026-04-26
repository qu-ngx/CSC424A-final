# simple-map-go-ver

## Part 1: C to Go transition

This project started from a C hash map with separate chaining and recreated the same behavior in Go.
The core design stayed the same (string keys, bucket array, linked-list collision handling, resize when full), but implementation details changed:
- C used manual memory management and `void*` values.
- Go uses garbage collection and `any` values with type assertions when needed.
- API concepts were mapped directly (`map_set/map_get/map_del` -> `Set/Get/Delete`) to keep behavior faithful.


## How to test the Go version

Run all Go tests in the project:

```bash
make test
```

Or run only the map package tests directly:

```bash
go test ./simplemap -v
```

If tests pass, you should see `PASS` and an `ok` line for the package.

## Part 2: How to run benchmarks

From this folder, run:

```bash
make bench-vs-c
```

This prints:
- C benchmarks first (`cbench`)
- Go benchmarks second (`simplemap`)

You can also run them separately:

```bash
make bench-c
make bench
```


## How to interpret the results

There are 3 workloads:
- `Fill`: insert many keys into an empty map.
- `Lookup`: repeated `Get` on a pre-filled map.
- `Mixed`: alternating `Set` and `Get`.

Main metrics:
- `ns/op`: nanoseconds per operation. **Lower is faster**.
- `B/op` (Go only): bytes allocated per operation. **Lower is better**.
- `allocs/op` (Go only): allocations per operation. **Lower is better**.

### Example reading

If `Lookup` in C is `40 ns/op` and Go is `60 ns/op`, then C lookup is faster in that test.

If Go `Fill` shows high `B/op` and `allocs/op`, that means Fill spends more time/overhead in allocation.

## Notes for fair comparison

- Compare the same workload (`Fill` vs `Fill`, etc.).
- Run multiple times and look for consistent trends.
- Keep machine conditions similar (no heavy background tasks).
