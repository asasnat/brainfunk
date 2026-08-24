#include "interpreter.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Allocates a long enough string in char** program,
 * then loads into it the contents of file at filePath. */
static bool LoadProgramFromFile(const char* filePath, char** program)
{
    FILE* fstream = fopen(filePath, "rb");
    long fileLength;
    size_t out;

    if (fstream == NULL) {
        fprintf(stderr, "Failed to open file \"%s\"\n", filePath);
        return false;
    }

    fseek(fstream, 0L, SEEK_END);
    fileLength = ftell(fstream);
    rewind(fstream);

    *program = malloc(fileLength * sizeof(char));
    out = fread(*program, 1, fileLength, fstream);

    if (out < (size_t)fileLength) {
        fprintf(stderr, "Failed to read contents of file \"%s\"\n", filePath);
        free(*program);
        *program = NULL;
        return false;
    }

    fclose(fstream);

    return true;
}

int main(int argc, char** argv)
{
    size_t tapeLength = 32768;
    size_t stackSize = 32768;
    itpEOFMode eofMode = EOF_UNCHANGED;
    char* program = NULL;
    bool printHelp = false;

    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];

        if (arg[0] == '-') {
            switch (arg[1]) {
            case 'f': /* load file */
                ++i;
                if (i >= argc) {
                    fprintf(stderr, "No filename specified");
                    break;
                }

                if (program == NULL) {
                    LoadProgramFromFile(argv[i], &program);
                } else {
                    fprintf(stderr, "Program already loaded!\n");
                }

                break;

            case 'h': /* show help */
                printHelp = true;
                break;

            case 't': { /* specify tape length */
                    size_t newSize;

                    ++i;
                    if (i >= argc) {
                        fprintf(stderr, "No tape length specified\n");
                        break;
                    }

                    newSize = atoll(argv[i]);
                    if (newSize <= 0) {
                        fprintf(stderr, "Invalid tape length specified\n");
                        break;
                    }
                    tapeLength = newSize;
                }
                break;

            case 's': { /* specify stack size */
                    size_t newSize;

                    ++i;
                    if (i >= argc) {
                        fprintf(stderr, "No stack size specified\n");
                        break;
                    }

                    newSize = atoll(argv[i]);
                    if (newSize <= 0) {
                        fprintf(stderr, "Invalid stack size specified\n");
                        break;
                    }
                    stackSize = newSize;
                }
                break;

            case 'e': /* specify EOF mode */
                ++i;
                if (i >= argc) {
                    fprintf(stderr, "No EOF mode specified\n");
                    break;
                }

                if        (strcmp("zero",      argv[i]) == 0) {
                    eofMode = EOF_ZERO;
                } else if (strcmp("minusone",  argv[i]) == 0) {
                    eofMode = EOF_MINUS_ONE;
                } else if (strcmp("unchanged", argv[i]) == 0) {
                    eofMode = EOF_UNCHANGED;
                } else {
                    fprintf(stderr, "Invalid EOF mode\n");
                }
                break;

            default:
                goto invalid;
            }
        } else if (program == NULL) {
            if (!LoadProgramFromFile(arg, &program)) {
            invalid:
                fprintf(stderr, "Invalid parameter: %s\n", arg);
            }
        }
    }

    if (program == NULL || printHelp) {
        fprintf(stderr,
            "Brainfunk command line parameters:\n"
            "[filename]    Load program\n\n"
            "-h            Show this info\n\n"
            "-t [bytes]    Specifies the size of the tape,\n"
            "              in bytes. (default: 32768)\n\n"
            "-s [entries]  Specifies the length of the stack,\n"
            "              in entries. (default: 32768)\n\n"
            "-e [mode]     Specifies the EOF handling mode.\n"
            "              Possible values: zero, minusone,\n"
            "              unchanged (default: unchanged)\n\n"
        );

        return -1;
    }

    itpInit(tapeLength, stackSize, eofMode);
    itpLoadProgram(program);

    return itpRun();
}
