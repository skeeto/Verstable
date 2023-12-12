#include <stdio.h>

static void *arena_alloc(size_t size, void *arg)
{
    char **arena = arg;
    size += -size & 15;
    return *arena -= size;
}

static void arena_free(void *p, size_t size, void *arg)
{
    char **arena = arg;
    if (p == *arena) {
        size += -size & 15;
        *arena += size;
    }
}

#define NAME str_set
#define KEY_TY char *
#define MALLOC_FN arena_alloc
#define FREE_FN arena_free
#include "verstable.h"

int main(void)
{
    int cap = 1<<28;
    char *arena = (char *)malloc(cap) + cap;

    {
        char *scratch = arena;
        str_set set;
        vt_init(&set);
        set.alloc_arg = &scratch;
        int n = 50;

        for (int i = 0; i < n; i++) {
            char *key = arena_alloc(16, &scratch);
            sprintf(key, "%d", i);
            vt_insert(&set, key);
        }

        for (int i = 0; i < n; i += 2) {
            char key[16];
            sprintf(key, "%d", i);
            vt_erase(&set, key);
        }

        str_set clone;
        vt_init_clone(&clone, &set);
        for (str_set_itr i = vt_first(&clone); !vt_is_end(i); i = vt_next(i)) {
            printf("%s\n", i.data->key);
        }
    }
}
