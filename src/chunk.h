#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"
#include <stdint.h>

typedef enum {
    OP_CONSTANT = 0,
    OP_CONSTANT_LONG = 1,
    OP_RETURN = 2,
} OpCode;

typedef struct {
    int capacity;
    int count;
    uint8_t *items;
} OpCodeArray;

typedef struct {
    int offset;
    int line;
} LinesStart;

typedef struct {
    int capacity;
    int count;
    LinesStart *items;
} LinesArray;

typedef struct {
    OpCodeArray codes;
    LinesArray lines;
    ValueArray constants;
} Chunk;

/*
 * preconditions:
 *  - ch != NULL
 *  - if code = NULL then capacity and count = 0, vice versa
 *  - capacity >= count
 */
void chunk_check(const Chunk *ch);

/*
 * preconditions:
 *  - ch is valid
 * postconditions:
 *  chunk's codes' count & capacity = 0 and items = NULL
 *  chunk's lines' count & capacity = 0 and items = NULL
 *  chunk's constants' count & capacity = 0 and items = NULL
 */
void chunk_init(Chunk *ch);

/*
 * preconditions:
 *  - ch is valid
 * postconditions:
 *  - ch.codes.items at codes.count = byte
 *  - ch.codes.count increases by one
 *  - if ch.lines.items' line at ch.lines.count != line
 *    or ch.lines.count == 0
 *      - a new line is added
 *  - else
 *      - no action
 */
void chunk_write(Chunk *ch, uint8_t byte, int line);

/*
 * precondition:
 *  - chunk is valid
 *
 * postconditions:
 *  chunk's codes' count & capacity = 0 and items = NULL
 *  chunk's lines' count & capacity = 0 and items = NULL
 *  chunk's constants' count & capacity = 0 and items = NULL
 */
void chunk_free(Chunk *ch);

/*
 * precondition:
 *  - chunk is valid
 *  - offset >= 0 and < chunk's codes' count
 * postconditions:
 *  - no sideeffect
 */
int chunk_get_line_by_offset(const Chunk *ch, int offset);

/*
 * precondition:
 *  - ch is valid
 *
 * postconditions:
 *  - if chunk's contansts' count >= 255
 *      - ch's codes' count increases by 4
 *  - else
 *      - ch's codes' count increases by 2
 */
void chunk_write_constant(Chunk *ch, Value v, int line);

#endif
