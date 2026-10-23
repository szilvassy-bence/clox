#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void debug_disassemble_chunk(Chunk *ch, const char *name);
int debug_disassemble_instruction(Chunk *ch, int offset);

#endif
