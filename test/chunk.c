#include "chunk.h"
#include "memory.h"
#include "sb_memory.h"
#include <assert.h>
#include <stdio.h>

void test_chunk_write() {
    Chunk ch;
    chunk_init(&ch);
    int old_count = ch.codes.count;

    chunk_write(&ch, (uint8_t)2, 1);
    assert(ch.codes.capacity == 8);
    assert(ch.codes.count == old_count + 1);
    assert(ch.codes.items[old_count] == 2);

    chunk_free(&ch);
    chunk_init(&ch);
    for (uint8_t i = 0; i < 9; i++) {
        chunk_write(&ch, i, i);
    }
    chunk_write(&ch, 0, 9);
    assert(ch.codes.count == 10);
    assert(ch.codes.capacity == 16);
    assert(ch.lines.count == 10);
    chunk_free(&ch);
    sb_memory_destroy();
}

void test_chunk_write_300_constants() {
    Chunk ch;
    chunk_init(&ch);

    for (int i = 0; i < 300; i++) {
        chunk_write_constant(&ch, (Value)i, i);
    }
    assert(ch.constants.items[299] == 299);

    chunk_free(&ch);
    sb_memory_destroy();
}

void test_chunk_free() {
    Chunk ch;
    chunk_init(&ch);
    chunk_write_constant(&ch, (Value)1, 1);
    assert(ch.codes.items != NULL);
    assert(ch.codes.capacity == 8);
    assert(ch.codes.count == 2);
    assert(ch.constants.count == 1);

    chunk_free(&ch);

    assert(ch.codes.items == NULL);
    assert(ch.codes.capacity == 0);
    assert(ch.codes.count == 0);
    assert(ch.constants.count == 0);
    sb_memory_destroy();
}

void test_chunk_get_line_by_offset() {
    Chunk ch;
    chunk_init(&ch);
    chunk_write(&ch, 1, 1);
    chunk_write(&ch, 2, 1);
    chunk_write(&ch, 3, 1);
    chunk_write(&ch, 4, 2);
    chunk_write(&ch, 5, 2);
    chunk_write(&ch, 6, 3);
    chunk_write(&ch, 7, 3);

    assert(ch.codes.count == 7);
    assert(ch.lines.count == 3);

    int line = chunk_get_line_by_offset(&ch, 0);
    assert(line == 1);

    line = chunk_get_line_by_offset(&ch, 1);
    assert(line == 1);

    line = chunk_get_line_by_offset(&ch, 2);
    assert(line == 1);

    line = chunk_get_line_by_offset(&ch, 3);
    assert(line == 2);

    line = chunk_get_line_by_offset(&ch, 4);
    assert(line == 2);

    line = chunk_get_line_by_offset(&ch, 5);
    assert(line == 3);

    line = chunk_get_line_by_offset(&ch, 6);
    assert(line == 3);

    chunk_free(&ch);
    sb_memory_destroy();
}

int main() {
    test_chunk_write();
    test_chunk_free();
    test_chunk_get_line_by_offset();
    test_chunk_write_300_constants();
    printf("Chunk test is successful.\n");
}
