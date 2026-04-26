#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "map.h"

#ifndef BENCH_KEY_SPACE
#define BENCH_KEY_SPACE 10000
#endif

#ifndef C_FILL_ROUNDS
#define C_FILL_ROUNDS 50
#endif

#ifndef C_LOOKUP_ROUNDS
#define C_LOOKUP_ROUNDS 5000000
#endif

#ifndef C_MIXED_ROUNDS
#define C_MIXED_ROUNDS 5000000
#endif

static char **g_keys;
static int g_key_n = BENCH_KEY_SPACE;

static int
keys_init(void)
{
	g_keys = calloc((size_t)g_key_n, sizeof(char *));
	if (!g_keys) {
		return -1;
	}
	char buf[32];
	for (int i = 0; i < g_key_n; i++) {
		snprintf(buf, sizeof buf, "%d", i);
		g_keys[i] = strdup(buf);
		if (!g_keys[i]) {
			return -1;
		}
	}
	return 0;
}

static void
keys_free(void)
{
	if (!g_keys) {
		return;
	}
	for (int i = 0; i < g_key_n; i++) {
		free(g_keys[i]);
	}
	free(g_keys);
	g_keys = NULL;
}

static double
now_sec(void)
{
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
		return 0;
	}
	return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
}

static void
bench_fill(void)
{
	const int rounds = C_FILL_ROUNDS;
	double t0 = now_sec();
	for (int r = 0; r < rounds; r++) {
		map_t *m = map_new(0);
		if (!m) {
			fprintf(stderr, "map_new failed\n");
			return;
		}
		for (int i = 0; i < g_key_n; i++) {
			if (map_set(m, g_keys[i], (void *)(intptr_t)r) != 0) {
				fprintf(stderr, "map_set failed\n");
				map_free(m);
				return;
			}
		}
		map_free(m);
	}
	double dt = now_sec() - t0;
	double ns = (dt / (double)rounds) * 1e9;
	printf("BenchmarkCMap_Fill          \t%d\t%.0f ns/op\t(full map %d keys)\n",
	    rounds, ns, g_key_n);
}

static void
bench_lookup(void)
{
	map_t *m = map_new((unsigned int)g_key_n);
	if (!m) {
		fprintf(stderr, "map_new failed\n");
		return;
	}
	for (int i = 0; i < g_key_n; i++) {
		if (map_set(m, g_keys[i], g_keys[i]) != 0) {
			fprintf(stderr, "map_set failed\n");
			map_free(m);
			return;
		}
	}

	volatile void *sink = NULL;
	const int n = C_LOOKUP_ROUNDS;
	double t0 = now_sec();
	for (int i = 0; i < n; i++) {
		sink = map_get(m, g_keys[i % g_key_n]);
	}
	double dt = now_sec() - t0;
	double ns = (dt / (double)n) * 1e9;
	printf("BenchmarkCMap_Lookup        \t%d\t%.2f ns/op\n", n, ns);
	(void)sink;
	map_free(m);
}

static void
bench_mixed(void)
{
	map_t *m = map_new((unsigned int)g_key_n);
	if (!m) {
		fprintf(stderr, "map_new failed\n");
		return;
	}
	for (int i = 0; i < g_key_n; i++) {
		if (map_set(m, g_keys[i], (void *)(intptr_t)0) != 0) {
			fprintf(stderr, "map_set failed\n");
			map_free(m);
			return;
		}
	}

	volatile void *sink = NULL;
	const int n = C_MIXED_ROUNDS;
	double t0 = now_sec();
	for (int i = 0; i < n; i++) {
		char *k = g_keys[i % g_key_n];
		if ((i & 1) == 0) {
			if (map_set(m, k, (void *)(intptr_t)i) != 0) {
				fprintf(stderr, "map_set failed\n");
				map_free(m);
				return;
			}
		} else {
			sink = map_get(m, k);
		}
	}
	double dt = now_sec() - t0;
	double ns = (dt / (double)n) * 1e9;
	printf("BenchmarkCMap_Mixed         \t%d\t%.2f ns/op\n", n, ns);
	(void)sink;
	map_free(m);
}

int
main(void)
{
	if (keys_init() != 0) {
		fprintf(stderr, "keys_init failed\n");
		return 1;
	}

	printf("goos: (C)\n");
	printf("goarch: (native)\n");
	printf("pkg: cbench (algorithm synced to simple-map-original)\n");

	bench_fill();
	bench_lookup();
	bench_mixed();

	keys_free();
	return 0;
}
