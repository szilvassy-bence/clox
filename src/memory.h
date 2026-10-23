#ifndef clox_memory_h
#define clox_memory_h
#include "common.h"

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity)*2)

#define DA_CHECK(da)                                                           \
    do {                                                                       \
        assert((da) != NULL);                                                  \
        assert(((da)->items == NULL) ==                                        \
               ((da)->count == 0 && (da)->capacity == 0));                     \
        assert(((da)->items == NULL) || (da)->capacity >= (da)->count);        \
    } while (0)

#define DA_INIT(da)                                                            \
    do {                                                                       \
        assert((da) != NULL);                                                  \
        (da)->capacity = 0;                                                    \
        (da)->count = 0;                                                       \
        (da)->items = NULL;                                                    \
    } while (0)

#define DA_APPEND(da, item)                                                    \
    do {                                                                       \
        if ((da)->count >= (da)->capacity) {                                   \
            int new_cap_ = GROW_CAPACITY((da)->capacity);                      \
            (da)->items =                                                      \
                reallocate((da)->items, sizeof *(da)->items * new_cap_);       \
            (da)->capacity = new_cap_;                                         \
        }                                                                      \
        (da)->items[(da)->count++] = (item);                                   \
    } while (0)

#define DA_FREE(da)                                                            \
    do {                                                                       \
        reallocate((da)->items, 0);                                            \
        DA_INIT(da);                                                           \
    } while (0)

#define DA_AT(da, idx)                                                         \
    (assert((idx) >= 0 && (idx) < (da).count), (da).items[(idx)])

void *reallocate(void *pointer, size_t new_size);
#endif
