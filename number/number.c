#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>

void play();
void clear_input_buffer();
bool my_strtoimax(char *s, int *guess);
char *trim_whitespace(char *s);
char *trim_leading_ws(char *s);
void trim_trailing_ws(char *s);
bool prompt_play_again();
bool is_yes(char *s);
bool is_no(char *s);

char buf[256];

int main() {
    clrscr();

    while (1) {
        play();

        if (!prompt_play_again()) {
            return 0;
        }
    }
}

void play() {
    int ret, secret_number, guess;
    unsigned wait_time;

    // Gather "randomness".
    printf("\n");
    printf("Guess the number! Press any key to begin\n"); // exactly 40 chars
    wait_time = 0;
    clear_input_buffer();
    while (1) {
        if (kbhit()) break;
        wait_time += 1;
    }
    cgetc(); // consume the pending keypress

    secret_number = wait_time % 100 + 1;

    printf("\n");
    printf("I'm thinking of a number\nbetween 1 and 100...\n");

    while (1) {
        printf("\n");
        printf("What is your guess? ");
        ret = scanf("%255s", buf);
        if (ret == EOF) {
            return;
        } else if (ret == 0) {
            perror("scanf");
            exit(EXIT_FAILURE);
        }

        if (!my_strtoimax(buf, &guess)) {
            printf("That's not a number.\n");
        } else if (!(1 <= guess && guess <= 100)) {
            printf("Try guessing a number between 1 and 100.\n");
        } else if (guess == secret_number) {
            printf("Yes! The number was %d\n", secret_number);
            return;
        } else if (guess < secret_number) {
            printf("Nope. %d is too low.\n", guess);
        } else if (guess > secret_number) {
            printf("Nope. %d is too high.\n", guess);
        }
    }
}

// Consume any pending keypresses in the keyboard input buffer.
void clear_input_buffer() {
    while (kbhit()) {
        cgetc();
    }
}

bool my_strtoimax(char *s, int *guess) {
    char *end;
    s = trim_whitespace(s);
    *guess = strtoimax(s, &end, 10);
    return end - s == strlen(s);
}

// Write a null byte after the last non-whitespace character.
// Return a pointer to the first non-whitespace character.
//
// (Be careful to free `s` and not the returned pointer.)
char *trim_whitespace(char *s) {
    trim_trailing_ws(s);
    return trim_leading_ws(s);
}

// Helper for `trim_whitespace`.
char *trim_leading_ws(char *s) {
    char *c;
    for (c = s; *c; c++) {
        if (!isspace(*c)) {
            return c;
        }
    }
    return c;
}

// Helper for `trim_whitespace`.
void trim_trailing_ws(char *s) {
    char *c;
    for (c = s + strlen(s) - 1; c >= s; c--) {
        if (!isspace(*c)) {
            *(c + 1) = '\0';
            return;
        }
    }
    *s = '\0'; // s was all whitespace.
}

bool prompt_play_again() {
    int ret;

    printf("\n");
    while (1) {
        printf("Play again? (y/n) ");
        ret = scanf("%255s", buf);
        if (ret == EOF) {
            return false;
        } else if (ret == 0) {
            perror("scanf");
            exit(EXIT_FAILURE);
        }

        if (is_yes(buf)) {
            return true;
        } else if (is_no(buf)) {
            return false;
        }
    }
}

// Ignore leading whitespace. Check if `s` starts with y/Y.
bool is_yes(char *s) {
    char *c;
    for (c = s; *c; c++) {
        if (*c == 'y' || *c == 'Y') {
            return true;
        } else if (!isspace(*c)) {
            return false;
        }
    }
    return false;
}

// Ignore leading whitespace. Check if `s` starts with n/N.
bool is_no(char *s) {
    char *c;
    for (c = s; *c; c++) {
        if (*c == 'n' || *c == 'N') {
            return true;
        } else if (!isspace(*c)) {
            return false;
        }
    }
    return false;
}
