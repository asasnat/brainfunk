#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

/*typedef size_t ProgCounter;*/

static char* g_program = NULL;
static char* g_progPointer;

static uint8_t* g_tape = NULL;
static uint8_t* g_tapePointer;
static size_t   g_tapeLength = 0;

static char** g_stack = NULL;
static char** g_stackPointer;
static size_t g_stackSize = 0;

static itpStatus g_status;

void (*g_eofHandler) () = NULL;

static void itpEOFHandler_Zero()
{
    *g_tapePointer = 0;
}

static void itpEOFHandler_MinusOne()
{
    *g_tapePointer = 255;
}

static void itpEOFHandler_Unchanged()
{
    /* lol */
}

/* Helper function. When called, it will skip the current loop.
 * Called when a [ instruction is encountered
 * and the current tape value is zero (0). */
static void SkipToMatchingClosingBrace()
{
    unsigned int depth = 1;

    assert(*g_progPointer == '[' && "SkipToMatchingClosingBrace mustn't be "
          "called on an instruction other than '['");

    while (depth > 0) {
        ++g_progPointer;
        if (*g_progPointer == '\0') {
            g_status = STATUS_EOF;
            break;
        }

        switch (*g_progPointer) {
        case '[':
            ++depth;
            break;

        case ']':
            --depth;
            break;

        default:
            break;
        }
    }
}

void itpInit(size_t tapeLength, size_t stackSize, itpEOFMode eofMode)
{
    if (g_tape != NULL) free(g_tape);
    if (g_stack != NULL) free(g_stack);

    g_tape = malloc(tapeLength * sizeof(uint8_t));
    g_tapePointer = g_tape;
    memset(g_tape, 0, tapeLength * sizeof(uint8_t));

    /* The stack pointer starts one value before the actual stack,
     * because the stack push instruction first increments the stack pointer,
     * THEN assigns the value at the new stack pointer position. */
    g_stack = malloc(stackSize * sizeof(uint8_t*));
    g_stackPointer = g_stack - 1;
    memset(g_stack, 0, stackSize * sizeof(uint8_t*));

    g_tapeLength = tapeLength;
    g_stackSize  = stackSize;

    g_status = STATUS_OK;

    switch (eofMode) {
    case EOF_ZERO:
        g_eofHandler = itpEOFHandler_Zero;
        break;

    case EOF_MINUS_ONE:
        g_eofHandler = itpEOFHandler_MinusOne;
        break;

    case EOF_UNCHANGED:
        g_eofHandler = itpEOFHandler_Unchanged;
        break;

    default:
        break;
    }

    if (g_program != NULL) g_progPointer = g_program;
}

void itpLoadProgram(char* program)
{
    g_program = program;

    g_progPointer = g_program;
}

itpStatus itpStep()
{
    if (g_status != STATUS_OK) return g_status;
    char instruction = *g_progPointer;

    switch (instruction) {
    case '+':
        ++(*g_tapePointer);
        break;

    case '-':
        --(*g_tapePointer);
        break;

    case '<':
        if (g_tapePointer > g_tape) --g_tapePointer;
        break;

    case '>':
        if (g_tapePointer < g_tapeLength + g_tape - 1) ++g_tapePointer;
        break;

    case '.':
        putchar(*g_tapePointer);
        break;

    case ',': {
            char input = getchar();

            if (input == EOF) {
                g_eofHandler();
            } else *g_tapePointer = input;
        }
        break;

    case '[':
        if (*g_tapePointer == 0) SkipToMatchingClosingBrace();
        else {
            ++g_stackPointer;
            if (g_stackPointer >= g_stack + g_stackSize) {
                g_status = STATUS_STACK_OVERFLOW;
                break;
            }

            *g_stackPointer = g_progPointer;
        }
        break;

    case ']':
        if (g_stackPointer < g_stack) g_status = STATUS_MISMATCHED_BRACE;
        else {
            if (*g_tapePointer == 0) --g_stackPointer;
            else g_progPointer = *g_stackPointer;
        }
        break;

    case '\0':
        g_status = STATUS_EOF;
        break;
    }

    ++g_progPointer;

    return STATUS_OK;
}

itpStatus itpRun()
{
    while (g_status == STATUS_OK) itpStep();

    printf("\n"); /* in case the program does not output a newline at the end.
                   * Helps keep the terminal look normal. */

    itpQuit();

    return g_status;
}

void itpQuit()
{
    free(g_program);
    free(g_stack);
    free(g_tape);
}
