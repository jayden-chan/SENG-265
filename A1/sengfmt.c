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

#define MAX_BUF_LEN 40000
#define MAX_LINE_LEN 132

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
static void trim(char *input);
static bool file_exists(const char *file_name);
static void print_buffer(char *buffer, const bool debug);

static inline void write(char **dest, char **source);
static inline void write_spaces(char **output, int spaces);
static inline void write_char(char **output, const char ch);

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
        char output[MAX_BUF_LEN];

        fmt(argv[1], output, &s);
        print_buffer(output, false);

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
                        char buffer[MAX_LINE_LEN];
                        strncpy(buffer, line, sizeof(buffer));

                        char *word = strtok(buffer, " ");
                        handle_qm(word, s);
                        continue;
                }

                if (line[0] == '\n') {
                        output--;
                        write_char(&output, '\n');
                        write_char(&output, '\n');
                        curr_width = 0;
                        continue;
                }

                if (s->fmt) {
                        char *word = strtok(line, " ");

                        while (word != NULL) {
                                trim(word);

                                if (curr_width + s->mrgn + (int) strlen(word) > s->width) {
                                        output--;
                                        write_char(&output, '\n');
                                        curr_width = 0;
                                }

                                if (curr_width == 0)
                                        write_spaces(&output, s->mrgn);

                                char *wrd_ptr = word;
                                for (int i = 0; i < (int) strlen(word); i++)
                                        write(&output, &wrd_ptr);

                                write_char(&output, ' ');
                                curr_width += (int) strlen(word) + 1;

                                word = strtok(NULL, " ");
                        }
                } else {
                        char *ln_ptr = line;

                        while (*ln_ptr != '\0')
                                write(&output, &ln_ptr);
                }
        }

        trim(output);
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
        trim(word);

        if (!strcmp(word, "?width")) {
                word = strtok(NULL, " ");
                s->width = atoi(word);
                s->fmt = true;
                s->mrgn = 0;
                word = strtok(NULL, " ");
        } else if (s->fmt && !strcmp(word, "?mrgn")) {
                word = strtok(NULL, " ");
                s->mrgn = atoi(word);
                word = strtok(NULL, " ");
        } else if (!strcmp(word, "?fmt")) {
                word = strtok(NULL, " ");
                trim(word);
                s->fmt = strcmp(word, "off");
                word = strtok(NULL, " ");
        }
}

/**
 * trim removes the trailing whitespace from
 * a given input buffer
 *
 * @param input The buffer to trim
 */
static void trim(char *input)
{
        while (*input != '\0')
                input++;

        input--;

        while (isspace(*input))
                input--;

        input++;
        *input = '\0';
}

/**
 * write copies the character at the source pointer into
 * the location of the destination pointer, incrementing
 * both pointers after.
 *
 * @param dest   The destination location
 * @param source The source location
 */
static inline void write(char **dest, char **source)
{
        *(*dest)++ = *(*source)++;
}

/**
 * write_spaces writes the specified number of
 * spaces into the provided output buffer
 *
 * @param output The output buffer
 * @param spaces The number of spaces
 */
static inline void write_spaces(char **output, int spaces)
{
        for (int i = 0; i < spaces; i++)
                write_char(output, ' ');
}

/**
 * write_char writes the provided character
 * into the provided output buffer and increments
 * the output pointer by one
 *
 * @param output The output buffer
 * @param ch     The character to write
 */
static inline void write_char(char **output, const char ch)
{
        *(*output)++ = ch;
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
static void print_buffer(char *buffer, const bool debug)
{
        if (debug)
                printf("--- BEGIN BUFFER DUMP ---\n");

        /* Advance the pointer through the buffer until it
         * reaches the null terminator, printing each character
         * as we go
         */
        while (*buffer != '\0')
                printf("%c", *buffer++);

        /* Add a newline at the end */
        if (debug)
                printf("[0]");

        printf("\n");

        if (debug)
                printf("--- END BUFFER DUMP ---\n");
}
