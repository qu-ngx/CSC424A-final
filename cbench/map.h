#ifndef _MAP_H
#define _MAP_H

#define DEFAULT_SIZE 16
#define ERR_SMALLER_THAN_DEFAULT 2

struct node {
	char *key;
	void *val;
	struct node *next;
};

typedef struct {
	int len;
	int cap;
	struct node **list;
} map_t;

map_t *map_new(const unsigned int size);
void map_free(map_t *m);
void *map_get(map_t *m, char *key);
int map_set(map_t *m, char *key, void *val);
void map_del(map_t *m, char *key);

#endif /* _MAP_H */
