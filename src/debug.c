#include "debug.h"
#include "chunk.h"
#include "memory.h"
#include "value.h"
#include <stdio.h>

void debug_disassemble_chunk(Chunk *ch, const char *name) {
    printf("== %s ==\n", name);
    for (int offset = 0; offset < ch->codes.count;) {
        offset = debug_disassemble_instruction(ch, offset);
    }
}

static int simple_instruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constant_instruction(const char *name, Chunk *ch, int offset) {
    int constant_offset = DA_AT(ch->codes, offset + 1);
    printf("opcode: %-16s | constant offset: %04d | ", name, constant_offset);
    value_print(DA_AT(ch->constants, constant_offset));
    printf("\n");
    return offset + 2;
}

static int long_constant_instruction(const char *name, Chunk *ch, int offset) {
    int constant_offset_highest_8_bits = DA_AT(ch->codes, offset + 1);
    int constant_offset_middle_8_bits = DA_AT(ch->codes, offset + 2);
    int constant_offset_lowest_8_bits = DA_AT(ch->codes, offset + 3);
    int constant_offset = (int)(constant_offset_highest_8_bits << 16) +
                          (int)(constant_offset_middle_8_bits << 8) +
                          (int)constant_offset_lowest_8_bits;
    printf("opcode: %-16s | constant offset: %04d | ", name, constant_offset);
    value_print(DA_AT(ch->constants, constant_offset));
    printf("\n");
    return offset + 4;
}

int debug_disassemble_instruction(Chunk *ch, int offset) {
    printf("offset: %04d | ", offset);

    int line = chunk_get_line_by_offset(ch, offset);
    printf("line: %04d | ", line);

    uint8_t instruction = DA_AT(ch->codes, offset);
    switch (instruction) {
    case OP_CONSTANT:
        return constant_instruction("OP_CONSTANT", ch, offset);
    case OP_CONSTANT_LONG:
        return long_constant_instruction("OP_CONSTANT_LONG", ch, offset);
    case OP_RETURN:
        return simple_instruction("OP_RETURN", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}
