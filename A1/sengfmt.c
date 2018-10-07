/*
 * @author Jayden Chan <jaydencn7@gmail.com>
 * @date October 7, 2018
 * @version 1.7.6
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
 * ?width width:  Each line following the command will be formatted such
 *                that there is never more than width characters in each line
 * ?mrgn left  :  Each line following the command will be indented left spaces
 *                from the left-hand margin.
 * ?fmt on/off :  This is used to turn formatting on and off.
 */

/*
 * NOTE: This code is formatted according to the Linux Kernel Coding Style
 * guide, available here:
 *
 * https://www.kernel.org/doc/html/v4.10/process/coding-style.html
 *
 * It is pretty messy right now because we are not allowed to use malloc
 * or header files.. Will improve a lot after the second C assignment.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_BUF_LEN 39600
#define MAX_LINE_LEN 132

#define WRITE(X, Y) *(*X)++ = *(*Y)++;
#define WRITE_CHAR(X, Y) *(*X)++ = Y;

#define WRITE_SPACES(X, Y) for (int i = 0; i < Y; i++)\
                                        WRITE_CHAR(X, ' ');
/**
 * The Settings struct stores the formatting flags
 * found in the supplied text file.
 */
typedef struct Settings {
        int width;
        int mrgn;
        bool fmt;
} Settings;

/* Function prototypes */
static void fmt(const char *file_name, char *output, Settings *s);
static void handle_qm(char *word, Settings *s);
static bool file_exists(const char *file_name);
static void print_buffer(char *buffer);

/* Entry point for the program */
int main(int argc, char *argv[])
{
        if (argc < 2) {
                fprintf(stderr, "Not enough arguments. You must specify a file.\n");
                return EXIT_FAILURE;
        }

        if (!file_exists(argv[1])) {
                fprintf(stderr, "Specified file does not exist.\n");
                return EXIT_FAILURE;
        }

        Settings s;
        char output[MAX_BUF_LEN+1];

        fmt(argv[1], output, &s);
        print_buffer(output);

        return EXIT_SUCCESS;
}

/**
 * fmt formats the provided input buffer and outputs
 * the result in the output buffer
 *
 * @param file_name The file to format
 * @param output    The output buffer
 * @param s         The settings to use
 */
static void fmt(const char *file_name, char *output, Settings *s)
{
        FILE* file = fopen(file_name, "r");
        char line[MAX_LINE_LEN+1];
        int curr_width = 0;

        while (fgets(line, MAX_LINE_LEN, file) != NULL) {
                if (line[0] == '?') {
                        char buffer[MAX_LINE_LEN+1];
                        strncpy(buffer, line, sizeof(buffer));

                        char *word = strtok(buffer, " \n");
                        handle_qm(word, s);
                        continue;
                }

                if (line[0] == '\n') {
                        output--;
                        WRITE_CHAR(&output, '\n');
                        WRITE_CHAR(&output, '\n');
                        curr_width = 0;
                        continue;
                }

                if (s->fmt) {
                        char *word = strtok(line, " \n");

                        while (word != NULL) {
                                if (curr_width + s->mrgn + (int) strlen(word) > s->width) {
                                        output--;
                                        WRITE_CHAR(&output, '\n');
                                        curr_width = 0;
                                }

                                if (curr_width == 0)
                                        WRITE_SPACES(&output, s->mrgn);

                                char *wrd_ptr = word;
                                for (int i = 0; i < (int) strlen(word); i++)
                                        WRITE(&output, &wrd_ptr);

                                WRITE_CHAR(&output, ' ');
                                curr_width += (int) strlen(word) + 1;

                                word = strtok(NULL, " \n");
                        }
                } else {
                        char *ln_ptr = line;
                        while (*ln_ptr != '\0')
                                WRITE(&output, &ln_ptr);
                }
        }

        *(output-1) = '\0';
        fclose(file);
}

/**
 * handle_qm takes in a word beginning with a
 * question mark and parses it. If it is a flag,
 * the appropriate action will be taken.
 * Otherwise nothing will happen.
 *
 * WARNING: This function will not work if there
 * isn't already something tokenized by strtoken
 * (strtoken is being passed NULL)
 *
 * @param word The word to parse
 * @param s    The settings to modify
 */
static void handle_qm(char *word, Settings *s)
{
        if (!strcmp(word, "?width")) {
                word = strtok(NULL, " \n");
                s->width = atoi(word);
                s->fmt = true;
                s->mrgn = 0;
                word = strtok(NULL, " \n");
        } else if (s->fmt && !strcmp(word, "?mrgn")) {
                word = strtok(NULL, " \n");
                s->mrgn = (int) strtol(word, NULL, 10);
                word = strtok(NULL, " \n");
        } else if (!strcmp(word, "?fmt")) {
                word = strtok(NULL, " \n");
                s->fmt = strcmp(word, "off");
                word = strtok(NULL, " \n");
        }
}

/**
 * file_exists checks to see whether the specified file exists
 *
 * @param file_name The path to the file
 * @return True if the file exists, false if it does not
 */
static bool file_exists(const char *file_name)
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

/**
 * print_buffer prints the contents of the supplied buffer
 * into stdout.
 *
 * @param buffer The buffer to print
 * @param debug  Whether or not to print debug info
 */
static void print_buffer(char *buffer)
{
#ifdef DEBUG
        printf("--- BEGIN BUFFER DUMP ---\n");
#endif

        /* Advance the pointer through the buffer until it
         * reaches the null terminator, printing each character
         * as we go
         */
        while (*buffer != '\0')
                printf("%c", *buffer++);

        /* Add a newline at the end */
#ifdef DEBUG
        printf("[0]");
#endif
        printf("\n");
#ifdef DEBUG
        printf("--- END BUFFER DUMP ---\n");
#endif
}
