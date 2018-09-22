/*
 * @author Jayden Chan <jaydencn7@gmail.com>
 * @date October 7, 2018
 * @version 1
 *
 * UVic SENG 265, Fall 2018, A1
 *
 * This will contain a solution to sengfmt. In order to complete the
 * task of formatting a file, it must open and read the file (hint:
 * using fopen() and fgets() method) and format the text content base on the
 * commands in the file. The program should output the formated content
 * to the command line screen by default (hint: using printf() method).
 *
 * Supported commands include:
 * ?width width :  Each line following the command will be formatted such
 *                 that there is never more than width characters in each line
 * ?mrgn left   :  Each line following the command will be indented left spaces
 *                 from the left-hand margin.
 * ?fmt on/off  :  This is used to turn formatting on and off.
 */

/*
 * NOTE: This code is formatted according to the Linux Kernel Coding Style
 * guide, available here:
 *
 * https://www.kernel.org/doc/html/v4.10/process/coding-style.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool file_exists(const char* file_name);

int main(int argc, char* argv[]) 
{
        if (argc < 2) {
                fprintf(stderr, "ERROR: Not enough arguments. You must specify a file.\n");
                exit(1);
        }

        if (!file_exists(argv[1])) {
                fprintf(stderr, "ERROR: Specified file does not exist.\n");
                exit(1);
        }

        exit(0);
}

bool file_exists(const char* file_name)
{
        FILE* file = fopen(file_name, "r");

        if (file != NULL) {
                fclose(file);
                return true;
        }

        return false;
}
