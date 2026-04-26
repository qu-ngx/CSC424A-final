package simplemap

import (
	"strconv"
	"testing"
)

func TestNew(t *testing.T) {
	m := New(100)
	if m == nil {
		t.Fatal("expected non-nil map")
	}
}

func TestDefaultCap(t *testing.T) {
	m := New(0)
	if m.Cap() != DefaultSize {
		t.Fatalf("cap: got %d want %d", m.Cap(), DefaultSize)
	}
}

func TestLen(t *testing.T) {
	m := New(0)
	m.Set("animals", "[\"dog\", \"cat\", \"horse\"]")
	if m.Len() != 1 {
		t.Fatalf("len: got %d want 1", m.Len())
	}
}

func TestSet(t *testing.T) {
	m := New(0)
	m.Set("doctor", "Quinn")
	v, ok := m.Get("doctor")
	if !ok || v != "Quinn" {
		t.Fatalf("get: ok=%v v=%v", ok, v)
	}
}

func TestDel(t *testing.T) {
	m := New(0)
	m.Set("key", "value")
	if v, ok := m.Get("key"); !ok || v != "value" {
		t.Fatalf("pre del: ok=%v v=%v", ok, v)
	}
	m.Delete("key")
	if _, ok := m.Get("key"); ok {
		t.Fatal("expected key missing after delete")
	}
}

func TestUpdate(t *testing.T) {
	m := New(0)
	testVal := 10
	m.Set("int", &testVal)
	p, ok := m.Get("int")
	if !ok {
		t.Fatal("missing int")
	}
	if *(p.(*int)) != 10 {
		t.Fatalf("got %d", *(p.(*int)))
	}
	newVal := testVal + 1
	m.Set("int", &newVal)
	p, ok = m.Get("int")
	if !ok {
		t.Fatal("missing int after update")
	}
	if *(p.(*int)) != 11 {
		t.Fatalf("got %d", *(p.(*int)))
	}
}

func TestGetSetInt(t *testing.T) {
	m := New(0)
	testVal := 10
	m.Set("int", &testVal)
	p, ok := m.Get("int")
	if !ok {
		t.Fatal("missing")
	}
	if *(p.(*int)) != 10 {
		t.Fatalf("got %d", *(p.(*int)))
	}
}

func TestGetSetDouble(t *testing.T) {
	m := New(0)
	testVal := 10.0
	m.Set("double", &testVal)
	p, ok := m.Get("double")
	if !ok {
		t.Fatal("missing")
	}
	if *(p.(*float64)) != 10 {
		t.Fatalf("got %v", *(p.(*float64)))
	}
}

func TestResize(t *testing.T) {
	m := New(0)
	for i := 1; i <= 16; i++ {
		m.Set(strconv.Itoa(i), "1")
	}
	m.Set("17", "111")
	m.Set("18", "1")
	if m.Cap() != 32 {
		t.Fatalf("cap: got %d want 32", m.Cap())
	}
	if m.Len() != 18 {
		t.Fatalf("len: got %d want 18", m.Len())
	}
	for i := 1; i <= 18; i++ {
		key := strconv.Itoa(i)
		v, ok := m.Get(key)
		if !ok || v == nil {
			t.Fatalf("missing key %q", key)
		}
	}
}
