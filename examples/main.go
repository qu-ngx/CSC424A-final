package main

import (
	"fmt"
	"simple-map-go-ver/simplemap"
)

func main() {
	m := simplemap.New(100)
	m.Set("animals", "[\"dog\", \"cat\", \"horse\"]")

	i := 100
	m.Set("integer", &i)

	ll := int64(100)
	m.Set("long_long", &ll)

	if v, ok := m.Get("animals"); ok {
		fmt.Println(v)
	}

	if p, ok := m.Get("integer"); ok {
		fmt.Println(*(p.(*int)))
	}

	if p, ok := m.Get("long_long"); ok {
		fmt.Println(*(p.(*int64)))
	}

	m.Delete("integer")
	if _, ok := m.Get("integer"); !ok {
		fmt.Println("map: key not found")
	}

	fmt.Println("cap:", m.Cap())
	fmt.Println("len:", m.Len())
}
