package simplemap

import (
	"strconv"
	"testing"
)

const benchKeySpace = 10_000

func benchKeys() []string {
	keys := make([]string, benchKeySpace)
	for i := range keys {
		keys[i] = strconv.Itoa(i)
	}
	return keys
}

// Fill: insert many keys from an empty map (alloc + hash + chaining + resize).
func BenchmarkSimpleMap_Fill(b *testing.B) {
	keys := benchKeys()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		m := New(0)
		for _, k := range keys {
			m.Set(k, i)
		}
	}
}

// Lookup: repeated gets on a full map.
func BenchmarkSimpleMap_Lookup(b *testing.B) {
	keys := benchKeys()
	m := New(len(keys))
	for _, k := range keys {
		m.Set(k, k)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_, _ = m.Get(keys[i%len(keys)])
	}
}

// Mixed: alternate set (update) and get.
func BenchmarkSimpleMap_Mixed(b *testing.B) {
	keys := benchKeys()
	m := New(len(keys))
	for _, k := range keys {
		m.Set(k, 0)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		k := keys[i%len(keys)]
		if i%2 == 0 {
			m.Set(k, i)
		} else {
			_, _ = m.Get(k)
		}
	}
}
