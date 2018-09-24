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
 * It is pretty messy right now because we are not allowed to use malloc
 * or header files.. Will improve a lot after the second C assignment.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_BUF_LEN 40000
#define FLAG_SIZE_MRGN 6
#define FLAG_SIZE_WIDTH 7

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
static void fmt(char *input, char *output, Settings *s);
static int space_diff(char *input);
static void pre_parse(char *input, Settings *s);
static void trim(char *input);
static bool load_file(const char *file_name, char buffer[]);
static bool file_exists(const char *file_name);
static void print_buffer(char *buffer);
static int count_digits(int n);

static inline bool parse_bool(char *input);
static inline int parse_int(char *input);
static inline void write(char **dest, char **source);

/* Entry point for the program */
int main(int argc, char *argv[])
{
        if (argc < 2) {
                fprintf(stderr, "Not enough arguments. You must specify a file.\n");
                exit(1);
        }

        if (!file_exists(argv[1])) {
                fprintf(stderr, "Specified file does not exist.\n");
                exit(1);
        }

        char input[MAX_BUF_LEN];
        char output[MAX_BUF_LEN];

        if (!load_file(argv[1], input)) {
                fprintf(stderr, "ERROR: File reading failed.\n");
                exit(1);
        }

        Settings s;
        print_buffer(input);

        pre_parse(input, &s);

        fmt(input, output, &s);

        trim(output);
        print_buffer(output);

        exit(0);
}

/**
 * fmt formats the provided input buffer and outputs
 * the result in the output buffer
 *
 * @param input  The input buffer
 * @param output The output buffer
 * @param s      The settings to use
 */
static void fmt(char *input, char *output, Settings *s)
{
        int curr_width = 0;
        while (*input != '\0') {
                if (s->fmt && isspace(*input)) {
                        while (isspace(*input)) {
                                input++;
                        }
                        input--;
                        *input = ' ';

                        if (curr_width + space_diff(input) + s->mrgn >= s->width) {
                                *input = '\n';
                                curr_width = 0;
                        } else {
                                curr_width++;
                        }

                        write(&output, &input);
                } else if (*input == '?') {
                        input++;
                        char wrd[10];
                        char *wrd_ptr = wrd;

                        while (isalpha(*input)) {
                                write(&wrd_ptr, &input);
                        }
                        *wrd_ptr = '\0';

                        input++;
                        if (!strcmp(wrd, "width")) {
                                s->width = parse_int(input);
                                s->fmt = true;
                                input += count_digits(s->width);
                        } else if (s->fmt && !strcmp(wrd, "mrgn")) {
                                s->mrgn = parse_int(input);
                        } else if (!strcmp(wrd, "fmt")) {
                                s->fmt = parse_bool(input);
                        }

                        while (isspace(*input)) {
                                input++;
                        }
                } else {
                        write(&output, &input);
                        curr_width++;
                }
        }
}

/**
 * space_diff calculates the number of characters
 * until the next space in the input buffer
 *
 * @param input The input buffer
 * @return      The number of chars
 */
static int space_diff(char *input)
{
        int ret = 0;
        while (!isspace(*input++)) {
                ret++;
        }

        return ret;
}

/**
 * pre_parse checks for the presence of the ?width
 * tag in the provided input buffer and sets the
 * provided settings accordingly.
 *
 * @param input The input buffer
 * @param s     The settings
 */
static void pre_parse(char *input, Settings *s)
{
        char *width_loc = strstr(input, "?width ");
        if (width_loc == NULL) {
                s->width = (size_t) NULL;
                s->mrgn = 0;
                s->fmt = false;
        } else {
                s->width = parse_int(width_loc + FLAG_SIZE_WIDTH);
                s->fmt = true;
        }
}

/**
 * parse_int takes a pointer to an integer in a string
 * and returns its value
 *
 * @param input The pointer
 * @return      The value of the integer
 */
static inline int parse_int(char *input)
{
        char num[10];
        char *num_ptr = num;
        char *inp_ptr = input;

        while (isdigit(*inp_ptr)) {
                *num_ptr++ = *inp_ptr++;
        }
        *num_ptr = '\0';

        return atoi(num);
}

/**
 * parse_bool takes a pointer to a word in a string
 * and returns its value
 *
 * @param input The pointer
 * @return      The value of the boolean
 */
static inline bool parse_bool(char *input)
{
        char val[10];
        char *val_ptr = val;
        char *inp_ptr = input;

        while (isalpha(*inp_ptr)) {
                *val_ptr++ = *inp_ptr++;
        }
        *val_ptr = '\0';

        return strcmp(val, "on") == 0;
}

/**
 * trim removes the trailing whitespace from
 * a given input buffer
 *
 * @param buffer The buffer to trim
 */
static void trim(char *input)
{
        while (*input != '\0') {
                input++;
        }
        input--;

        while (isspace(*input)) {
                input--;
        }
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
 * load_file reads the specified file and loads the characters
 * into the supplied buffer array. This function is adapted from:
 * https://stackoverflow.com/a/2029227/9980744
 *
 * @param file_name The path of the file to read
 * @param buffer    The buffer to load the file into
 * @return          True if the file was read successfully, false otherwise
 */
static bool load_file(const char *file_name, char *buffer)
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
 * into stdout. Currently only used for debugging.
 *
 * @param buffer The buffer to print
 */
static void print_buffer(char *buffer)
{
        printf("--- BEGIN BUFFER DUMP ---\n");

        /* Advance the pointer through the buffer until it
         * reaches the null terminator, printing each character
         * as we go
         */
        while (*buffer != '\0') {
                printf("%c", *buffer++);
        }

        /* Add a newline at the end */
        printf("[0]\n");
        printf("--- END BUFFER DUMP ---\n");
}

/**
 * count_digits counts the number of digits
 * in an integer
 *
 * @param n The number
 * @return  The number of digits in n
 */
static int count_digits(int n)
{
    if (n < 0) n = (n == INT_MIN) ? INT_MAX : -n;
    if (n > 999999999) return 10;
    if (n > 99999999) return 9;
    if (n > 9999999) return 8;
    if (n > 999999) return 7;
    if (n > 99999) return 6;
    if (n > 9999) return 5;
    if (n > 999) return 4;
    if (n > 99) return 3;
    if (n > 9) return 2;
    return 1;
}
