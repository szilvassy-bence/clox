#include "memory.h"
#include "sb_mem_freelist.h"
#include <stdlib.h>

void *reallocate(void *ptr, size_t new_size) {
    if (new_size == 0) {
        if (ptr != NULL) {
            sb_memory_free(ptr);
        }
        return NULL;
    }
    void *result = sb_memory_realloc(ptr, (uint32_t)new_size);
    if (result == NULL) {
        exit(1);
    }
    return result;
}
