#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stddef.h>

typedef enum itpStatus {
    STATUS_EOF, /* program execution stopped normally */
    STATUS_OK, /* program execution ongoing */
    STATUS_MISMATCHED_BRACE, /* ] found w/o matching  [ */
    STATUS_STACK_OVERFLOW, /* too much loop nesting */
    STATUS_TAPE_OVERFLOW, /* Unused. */
} itpStatus;

/* Denotes how the interpreter should behave when it encoounters an EOF
 * in standard input. */
typedef enum itpEOFMode {
    EOF_ZERO, /* cell is set to zero */
    EOF_MINUS_ONE, /* I guess set cell to 255 (-1 when cast to uint8_t)? */
    EOF_UNCHANGED /* cell is left unchanged */
} itpEOFMode;

/* Initialises the tape and the stack.
 * Does NOT initialise the program itself.
 * You must do that with itpLoadProgram. */
void itpInit(size_t tapeLength, size_t stackSize, itpEOFMode eofMode);

/* Loads the program from a user-provided string.
 * The string MUST be null-terminated.
 * The interpreter will take ownership of the string,
 * and will automatically free it after calling itpQuit. */
void itpLoadProgram(char* program);

/* The interpreter will execute one (1) instruction from the program.
 * It will then return the interpreter status after doing so. */
itpStatus itpStep();

/* The interpreter will execute the entire program.
 * It will then return the interpreter status after doing so.
 * If everything goes okay, it will return STATUS_EOF . */
itpStatus itpRun();

/* You don't need to call this if you use itpRun .
 * That function will call itpQuit automatically. */
void itpQuit();

#endif
