# simple-map-go-ver

## How to run benchmarks

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
