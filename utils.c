#include "common.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_valid_int_input(const char *prompt, int min, int max) {
    int input;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &input) == 1) {
            clear_input_buffer();
            if (input >= min && input <= max) {
                return input;
            }
        } else {
            clear_input_buffer();
        }
        printf("请输入%d-%d之间的有效数字！\n", min, max);
    }
}

void get_string_input(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void system_pause() {
    printf("\n按回车键继续...");
    clear_input_buffer();
    getchar();
}