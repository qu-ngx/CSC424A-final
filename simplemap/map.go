package simplemap

const (
	// DefaultSize is the bucket count when New is called with size 0.
	DefaultSize = 16
	capacityMultiplier = 2
)

type node struct {
	key  string
	val  any
	next *node
}

// Map is a string-keyed hash map with separate chaining. Values are stored as any
// (comparable to the original void* usage).
type Map struct {
	len     int
	cap     int
	buckets []*node
}

// New allocates a map. If size is 0, capacity defaults to DefaultSize.
func New(size int) *Map {
	cap := size
	if cap == 0 {
		cap = DefaultSize
	}
	return &Map{
		cap:     cap,
		buckets: make([]*node, cap),
	}
}

// Hash function for the map.
func hash(key string, cap int) int {
	sum := 0
	for i := 0; i < len(key); i++ {
		sum += int(key[i])
	}
	if cap == 0 {
		return 0
	}
	return sum % cap
}

// Len returns the number of key-value pairs.
func (m *Map) Len() int {
	return m.len
}

// Cap returns the current number of buckets.
func (m *Map) Cap() int {
	return m.cap
}

// Get returns the value for key and whether it was found.
func (m *Map) Get(key string) (any, bool) {
	pos := hash(key, m.cap)
	for n := m.buckets[pos]; n != nil; n = n.next {
		if n.key == key {
			return n.val, true
		}
	}
	return nil, false
}

func (m *Map) resize(newCap int) {
	nm := New(newCap)
	for i := 0; i < m.cap; i++ {
		for n := m.buckets[i]; n != nil; n = n.next {
			nm.setInternal(n.key, n.val, false)
		}
	}
	m.cap = nm.cap
	m.len = nm.len
	m.buckets = nm.buckets
}

// setInternal inserts or updates. If allowResize is false, used during rehash only.
func (m *Map) setInternal(key string, val any, allowResize bool) {
	if allowResize && m.len == m.cap {
		m.resize(m.cap * capacityMultiplier)
	}
	pos := hash(key, m.cap)
	head := m.buckets[pos]
	for n := head; n != nil; n = n.next {
		if n.key == key {
			n.val = val
			return
		}
	}
	m.buckets[pos] = &node{key: key, val: val, next: head}
	m.len++
}

// Set stores val under key. It matches the C behavior of growing when len equals
// capacity before inserting a new key.
func (m *Map) Set(key string, val any) {
	m.setInternal(key, val, true)
}

// Delete removes key. It is a no-op if the key is absent.
func (m *Map) Delete(key string) {
	pos := hash(key, m.cap)
	cur := &m.buckets[pos]
	for *cur != nil {
		if (*cur).key == key {
			*cur = (*cur).next
			m.len--
			return
		}
		cur = &(*cur).next
	}
}
