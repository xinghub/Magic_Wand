/**
  ******************************************************************************
  * @file           : LOG_Task.c
  * @author         : The Fool
  * @brief          : None
  * @attention      : None
  * @date           : 2025/2/5
  ******************************************************************************
  */

#include "LOG_Task.h"
#include "main.h"
#include "version_git.h"
static char *magic_mand[] = {
        // M
        "|\\    /|",
        "| \\  / |",
        "|  \\/  |",
        "|      |",
        "|      |",
        "|      |",
        "|      |",

        // A
        "   /\\   ",
        "  /  \\  ",
        " /____\\ ",
        "|      |",
        "|      |",
        "|      |",
        "|      |",

        // G
        "  _____  ",
        " /     \\",
        "|       ",
        "|   ____",
        "|      |",
        " \\     |",
        "  \\____/",

        // I
        "_______",
        "    |   ",
        "    |   ",
        "    |   ",
        "    |   ",
        "    |   ",
        "____|___",

        // C
        "  ______    ",
        " /      \\   ",
        "|       |   ",
        "|           ",
        "|           ",
        " \\      |   ",
        "  \\_____/   ",

        // M
        "|      |",
        "|      |",
        "|      |",
        "|      |",
        "|  /\\  |",
        "| /  \\ |",
        "|/    \\|",

        // A
        "   /\\   ",
        "  /  \\  ",
        " /____\\ ",
        "|      |",
        "|      |",
        "|      |",
        "|      |",

        // N (粗线条版本)
        "|\\     |",
        "| \\    |",
        "|  \\   |",
        "|   \\  |",
        "|    \\ |",
        "|     \\|",
        "|      |",

        // D
        "_______ ",
        "|      \\",
        "|       |",
        "|       |",
        "|       |",
        "|       |",
        "|______/",
};

/**
* @CreateTime
* @Author
* @brief
* @param val1
* @param val2
* @return
*/
void MZprint(void)
{
    // 横向打印逻辑（每7行合并打印）
    int letter_count = 10; // MAGIC MAND 共10个字母（含空格）
    int rows_per_letter = 7;

    for (int row = 0; row < rows_per_letter; row++) {
        for (int letter = 0; letter < letter_count; letter++) {
            int index = letter * rows_per_letter + row;
            if (index < sizeof(magic_mand) / sizeof(magic_mand[0])) {
                printf("\033[40;32;5m%s ", magic_mand[index]); // 字母间用2空格分隔
            }
        }
        printf("\033[0m\r\n");
    }
    printf("Auther:  krud\r\n");
    printf("Git -v:  %s\r\n", GIT_COMMIT_SHA);
    printf("Build:   Feb 05 2025\r\n");
}
