#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "memory.h"
#include "sb_int.h"
#include "sb_mem_freelist.h"
#include "value.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define SIZE 17

int main() {

    uint8_t a[] = {OP_RETURN};

    Chunk ch;
    chunk_init(&ch);
    for (size_t i = 0; i < sizeof(a) / sizeof(uint8_t); i++) {
        chunk_write(&ch, a[i], 1);
    }

    for (int i = 2; i < 3; i++) {
        chunk_write_constant(&ch, (Value)i, i);
    }

    debug_disassemble_chunk(&ch, "My chunk");

    chunk_free(&ch);
    sb_memory_destroy();
    return 0;
}
