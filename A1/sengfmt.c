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
 *
 * This code does some extra error handling in addition to the main assignment, so it
 * might be a little longer than most other submissions. The code should be much
 * easier to adapt for the second part of the assignment though.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_BUF_LEN 40000

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
void fmt(char *input, char *output);
void apply_margin(char *input, char *output, const Settings s);
void parse_space(char **input, char **dest, int *curr_width, const Settings s);
void parse_flag(char *ret, char **input);
int parse_flag_int(char **input);
bool parse_flag_bool(char **input);
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

        char input[MAX_BUF_LEN];
        char output[MAX_BUF_LEN];
        bool success = load_file(argv[1], input);
        if (!success) {
                fprintf(stderr, "ERROR: File reading failed.\n");
                exit(1);
        }

        fmt(input, output);

        print_buffer(output);

        exit(0);
}

/*
 * fmt formats the text
 *
 * @param input  The input buffer
 * @param output The output buffer
 */
void fmt(char *input, char *output)
{
        Settings s = {1, 0, false};
        char *base_ptr = input;
        char *dest = input;

        int curr_width = 0;
        while (*input != '\0') {
                if (s.mrgn >= s.width) {
                        fprintf(stderr, "ERROR: Margin width equal to or greater than line width.\n");
                        exit(1);
                }

                if (isspace(*input) && s.fmt) {
                        parse_space(&input, &dest, &curr_width, s);
                        *dest++ = *input++;
                        curr_width++;
                } else if (*input == '?') {
                        char wrd[10];
                        parse_flag(wrd, &input);

                        if (!strcmp(wrd, "width")) {
                                s.width = parse_flag_int(&input);
                                s.fmt = true;
                        } else if (!strcmp(wrd, "mrgn")) {
                                s.mrgn = parse_flag_int(&input);
                        } else if (!strcmp(wrd, "fmt")) {
                                s.mrgn = parse_flag_bool(&input);
                        }
                } else {
                        *dest++ = *input++;
                        curr_width++;
                }
        }

        apply_margin(base_ptr, output, s);
}

void apply_margin(char *input, char *output, const Settings s)
{
        char *ovr_ptr = output;

        for (int i = 0; i < s.mrgn; i++) {
                *ovr_ptr++ = ' ';
        }

        while(*input != '\0') {
                if (*input == '\n') {
                        *ovr_ptr++ = *input++;
                        for (int i = 0; i < s.mrgn; i++) {
                                *ovr_ptr++ = ' ';
                        }
                } else {
                        *ovr_ptr++ = *input++;
                }

        }
}

/*
 * parse_space handles an occurrence of a space in the text buffer
 *
 * @param input      Pointer to the input buffer
 * @param dest       Pointer to the output buffer
 * @param curr_width Current line width
 * @param s          Settings
 */
void parse_space(char **input, char **dest, int *curr_width, const Settings s)
{
        while (isspace(*(*input)++));
        (*input)--;
        (*input)--;
        **input = ' ';

        char *tmp_ptr = *input;
        do {
                (*input)++;
        } while (!isspace(**input));

        if (*curr_width + (*input - tmp_ptr - 1) + s.mrgn >= s.width) {
                *(*dest)++ = '\n';
                tmp_ptr++;
                *curr_width = 0;
        }

        *input = tmp_ptr;
}

/*
 * parse_flag parses a flag when the question mark is
 * encountered
 *
 * @param ret The buffer to load the flag into
 * @param input Pointer to the input buffer
 */
void parse_flag(char *ret, char **input)
{
        (*input)++;
        char *wrd_ptr = ret;

        while (**input != ' ') {
                *wrd_ptr++ = *(*input)++;
        }

        *wrd_ptr = '\0';
        (*input)++;
}

/*
 * parse_flag_int parses the integer after the flag
 *
 * @param  input Pointer to the input buffer
 * @return The integer value of the flag
 */
int parse_flag_int(char **input)
{
        char num[10];
        char *num_ptr = num;

        while (isdigit(**input)) {
                *num_ptr++ = *(*input)++;
        }

        *num_ptr = '\0';
        (*input)++;

        return atoi(num);
}

/*
 * parse_flag_bool parses the bool after the flag
 *
 * @param  input Pointer to the input buffer
 * @return The boolean value of the flag
 */
bool parse_flag_bool(char **input)
{
        char val[10];
        char *val_ptr = val;
        while (isdigit(**input)) {
                *val_ptr++ = *(*input)++;
        }
        *val_ptr = '\0';
        (*input)++;

        return strcmp(val, "on") == 0;
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
        /* printf("--- BEGIN BUFFER DUMP ---\n"); */
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
        /* printf("[0]\n"); */
        /* printf("--- END BUFFER DUMP ---\n"); */
}
