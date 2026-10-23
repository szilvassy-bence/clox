#include "chunk.h"
#include "memory.h"
#include "value.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define CONSTANT_LONG_MAX 0xFFFFFF

static int chunk_add_constant(Chunk *ch, Value v);

void chunk_check(const Chunk *ch) {
    DA_CHECK(&ch->codes);
    DA_CHECK(&ch->lines);
    DA_CHECK(&ch->constants);
}

void chunk_init(Chunk *ch) {
    assert(ch != NULL);
    DA_INIT(&ch->codes);
    DA_INIT(&ch->lines);
    DA_INIT(&ch->constants);
}

int chunk_get_last_line(const LinesArray line_arr) {
    if (line_arr.count == 0)
        return 0;
    return DA_AT(line_arr, line_arr.count - 1).line;
}

void chunk_write(Chunk *ch, uint8_t byte, int line) {
    chunk_check(ch);

    DA_APPEND(&ch->codes, byte);
    int offset = ch->codes.count - 1;
    LinesStart ls = {.offset = offset, .line = line};
    int last_line = chunk_get_last_line(ch->lines);
    if (ch->lines.count == 0 || last_line != line)
        DA_APPEND(&ch->lines, ls);
}

void chunk_free(Chunk *ch) {
    chunk_check(ch);
    DA_FREE(&ch->codes);
    DA_FREE(&ch->lines);
    DA_FREE(&ch->constants);
}

int chunk_get_line_by_offset(const Chunk *ch, int offset) {
    assert(offset >= 0 && offset < ch->codes.count);
    LinesStart last_line = *ch->lines.items;

    for (int i = 0; i < ch->lines.count; i++) {
        LinesStart entry_line = DA_AT(ch->lines, i);
        if (offset == entry_line.offset) {
            return entry_line.line;
        }
        if (entry_line.offset > offset) {
            return last_line.line;
        }
        last_line = entry_line;
    }
    return last_line.line;
}

void chunk_write_constant(Chunk *ch, Value v, int line) {
    chunk_check(ch);

    int constant_index = chunk_add_constant(ch, v);

    assert(constant_index <= CONSTANT_LONG_MAX);

    if (constant_index > 0xFF) {
        chunk_write(ch, OP_CONSTANT_LONG, line);
        uint32_t u32_constant_index =
            (uint32_t)(constant_index & CONSTANT_LONG_MAX);

        // int to 3 bytes
        // largest bytes
        uint8_t highest_8_bits = (uint8_t)(u32_constant_index >> 16 & 0xFF);
        uint8_t middle_8_bits = (uint8_t)(u32_constant_index >> 8 & 0xFF);
        uint8_t lowest_8_bits = (uint8_t)(u32_constant_index & 0xFF);
        chunk_write(ch, highest_8_bits, line);
        chunk_write(ch, middle_8_bits, line);
        chunk_write(ch, lowest_8_bits, line);
    } else {
        chunk_write(ch, OP_CONSTANT, line);
        chunk_write(ch, (uint8_t)constant_index, line);
    }
}

/*
 * preconditions:
 *  - ch is valid
 *
 * postconditions:
 *  - chunk's constants' count > 0
 *
 * return: index of the newly inserted constant
 */
int chunk_add_constant(Chunk *ch, Value v) {
    chunk_check(ch);
    DA_APPEND(&ch->constants, v);
    assert(ch->constants.count > 0);
    return ch->constants.count - 1;
}
