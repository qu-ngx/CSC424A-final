#include <stdlib.h>
#include <string.h>

#include "map.h"

#define CAPACITY_MULTIPLIER 2

static void
chain_free(struct node *n)
{
	while (n) {
		struct node *next = n->next;
		free(n->key);
		free(n);
		n = next;
	}
}

map_t *
map_new(const unsigned int size)
{
	map_t *m = malloc(sizeof(map_t));
	if (!m) {
		return NULL;
	}
	memset(m, 0, sizeof(map_t));
	if (size == 0) {
		m->cap = DEFAULT_SIZE;
	} else {
		m->cap = (int)size;
	}
	m->len = 0;
	m->list = calloc((size_t)m->cap, sizeof(struct node *));
	if (!m->list) {
		free(m);
		return NULL;
	}
	return m;
}

void
map_free(map_t *m)
{
	if (!m) {
		return;
	}
	if (m->list) {
		for (int i = 0; i < m->cap; i++) {
			chain_free(m->list[i]);
		}
		free(m->list);
	}
	free(m);
}

static int
hash(map_t *m, char *key)
{
	int sum = 0;
	for (int i = 0; key[i] != '\0'; i++) {
		sum += (unsigned char)key[i];
	}
	return sum % m->cap;
}

void *
map_get(map_t *m, char *key)
{
	int pos = hash(m, key);
	for (struct node *temp = m->list[pos]; temp; temp = temp->next) {
		if (strcmp(temp->key, key) == 0) {
			return temp->val;
		}
	}
	return NULL;
}

static int
map_resize(map_t *m, int new_cap)
{
	map_t *nm = map_new((unsigned int)new_cap);
	if (!nm) {
		return -1;
	}

	for (int i = 0; i < m->cap; i++) {
		for (struct node *temp = m->list[i]; temp; temp = temp->next) {
			if (map_set(nm, temp->key, temp->val) != 0) {
				map_free(nm);
				return -1;
			}
		}
	}

	for (int i = 0; i < m->cap; i++) {
		chain_free(m->list[i]);
	}
	free(m->list);

	m->list = nm->list;
	m->cap = nm->cap;
	m->len = nm->len;
	free(nm);
	return 0;
}

int
map_set(map_t *m, char *key, void *val)
{
	if (m->len == m->cap) {
		if (map_resize(m, m->cap * CAPACITY_MULTIPLIER) == -1) {
			return -1;
		}
	}
	int pos = hash(m, key);
	struct node *list = m->list[pos];
	for (struct node *temp = list; temp; temp = temp->next) {
		if (strcmp(temp->key, key) == 0) {
			temp->val = val;
			return 0;
		}
	}
	struct node *newn = malloc(sizeof(struct node));
	if (!newn) {
		return -1;
	}
	memset(newn, 0, sizeof(struct node));
	newn->key = strdup(key);
	if (!newn->key) {
		free(newn);
		return -1;
	}
	newn->val = val;
	newn->next = list;
	m->list[pos] = newn;
	m->len++;
	return 0;
}

void
map_del(map_t *m, char *key)
{
	int pos = hash(m, key);
	struct node **n = &m->list[pos];
	while (*n) {
		if (strcmp((*n)->key, key) == 0) {
			struct node *dead = *n;
			*n = dead->next;
			free(dead->key);
			free(dead);
			m->len--;
			return;
		}
		n = &(*n)->next;
	}
}
