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

#define MAX_BUF_LEN 40000
#define WIDTH_CHARS 7
#define MRGN_CHARS 6

/*
 * The Settings struct stores the formatting flags
 * found in the supplied text file.
 */
typedef struct Settings {
        int width;
        int mrgn;
        bool fmt;
} Settings;

/* Function prototypes */
bool load_file(const char *file_name, char buffer[]);
bool file_exists(const char *file_name);
void print_buffer(char *buffer);

int main(int argc, char *argv[])
{
        if (argc < 2) {
                fprintf(stderr, "ERROR: Not enough arguments. You must specify a file.\n");
                exit(1);
        }

        if (!file_exists(argv[1])) {
                fprintf(stderr, "ERROR: Specified file does not exist.\n");
                exit(1);
        }

        char buffer[MAX_BUF_LEN];
        bool success = load_file(argv[1], buffer);
        if (!success) {
                fprintf(stderr, "ERROR: File reading failed.\n");
                exit(1);
        }

        print_buffer(buffer);

        exit(0);
}

/*
 * load_file reads the specified file and loads the characters
 * into the supplied buffer array.
 *
 * @param file_name The path of the file to read
 * @param buffer    The buffer to load the file into
 * @return          True if the file was read successfully, false otherwise
 */
bool load_file(const char *file_name, char *buffer)
{
        /* Load the file */
        FILE *fp = fopen(file_name, "r");

        /* If the file isn't NULL, open */
        if (fp != NULL) {
                size_t newLen = fread(buffer, sizeof(char), MAX_BUF_LEN, fp);

                /* If there is an error, close and return false */
                if (ferror(fp) != 0) {
                        fclose(fp);
                        return false;
                } else {
                        buffer[newLen++] = '\0'; /* Terminate string */
                }

                fclose(fp);
                return true;
        }

        /* If file is NULL return false, indicating failure */
        return false;
}

/*
 * file_exists checks to see whether the specified file exists
 *
 * @param file_name The path to the file
 * @return True if the file exists, false if it does not
 */
bool file_exists(const char *file_name)
{
        /* Try opening the file */
        FILE* file = fopen(file_name, "r");

        /* If the file isn't NULL it exists, return true */
        if (file != NULL) {
                fclose(file);
                return true;
        }

        /* File doesn't exist, return false */
        return false;
}

/*
 * print_buffer prints the contents of the supplied buffer
 * into stdout. Currently only used for debugging.
 *
 * @param buffer The buffer to print
 */
void print_buffer(char *buffer)
{
        char *ptr = buffer;

        /* Advance the pointer through the buffer until it
         * reaches the null terminator, printing each character
         * as we go
         */
        while (*ptr != '\0') {
                printf("%c", *ptr);
                ptr++;
        }

        /* Add a newline at the end */
        printf("[0]\n");
}
