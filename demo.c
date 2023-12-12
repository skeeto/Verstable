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
        *arena += -size & 15;
    }
}

#define NAME int_set
#define KEY_TY int
#define MALLOC_FN arena_alloc
#define FREE_FN arena_free
#include "verstable.h"

int main(void)
{
    int cap = 1<<28;
    char *arena = (char *)malloc(cap) + cap;

    {
        char *scratch = arena;
        int_set set;
        vt_init(&set);
        set.alloc_arg = &scratch;
        int n = 50;

        for (int i = 0; i < n; i++) {
            vt_insert(&set, i);
        }

        for (int i = 0; i < n; i += 2) {
            vt_erase(&set, i);
        }

        int_set clone;
        vt_init_clone(&clone, &set);
        for (int_set_itr i = vt_first(&clone); !vt_is_end(i); i = vt_next(i)) {
            printf("%d\n", i.data->key);
        }
    }
}
