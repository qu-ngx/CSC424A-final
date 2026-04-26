PREFIX  ?= /usr/local
BINDIR  ?= $(PREFIX)/bin

CC       ?= cc
CBENCH   := cbench/cmap_bench
KEYS     ?= 10000
CFLAGS   := -std=c11 -O3 -Wall -Wextra \
	-DBENCH_KEY_SPACE=$(KEYS) \
	-DC_FILL_ROUNDS=$(or $(C_FILL_ROUNDS),50) \
	-DC_LOOKUP_ROUNDS=$(or $(C_LOOKUP_ROUNDS),5000000) \
	-DC_MIXED_ROUNDS=$(or $(C_MIXED_ROUNDS),5000000)

.PHONY: test run-example bench bench-c bench-vs-c clean install uninstall

test:
	go test ./...

run-example:
	go run ./examples

bench:
	go test -bench=. -benchmem -count=1 -benchtime=100ms -run=^$$ ./simplemap

$(CBENCH): cbench/bench.c cbench/map.c cbench/map.h
	$(CC) $(CFLAGS) -o $@ cbench/bench.c cbench/map.c

bench-c: $(CBENCH)
	./$(CBENCH)

bench-vs-c: $(CBENCH)
	@echo "--- C (cbench) ---"
	./$(CBENCH)
	@echo ""
	@echo "--- Go (./simplemap) ---"
	$(MAKE) bench

clean:
	go clean -cache
	rm -f example $(CBENCH)

install: $(CBENCH)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(CBENCH) $(DESTDIR)$(BINDIR)/cmap-bench
	go build -o $(DESTDIR)$(BINDIR)/simple-map-demo ./examples

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/cmap-bench $(DESTDIR)$(BINDIR)/simple-map-demo