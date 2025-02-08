/**
  ******************************************************************************
  * @file           : LOG_Task.c
  * @author         : The Fool
  * @brief          : 特殊图案生成网站：http://patorjk.com/
  *                   彩色字符打印：https://blog.csdn.net/qq_40399012/article/details/84195092
  * @attention      : None
  * @date           : 2025/2/5
  ******************************************************************************
  */

#include "LOG_Task.h"
#include "main.h"
#include "version_git.h"

static char *magic_mand[] = {
        "███╗   ███╗ █████╗  ██████╗ ██╗ ██████╗    ██╗    ██╗ █████╗ ███╗   ██╗██████╗ ",
        "████╗ ████║██╔══██╗██╔════╝ ██║██╔════╝    ██║    ██║██╔══██╗████╗  ██║██╔══██╗",
        "██╔████╔██║███████║██║  ███╗██║██║         ██║ █╗ ██║███████║██╔██╗ ██║██║  ██║",
        "██║╚██╔╝██║██╔══██║██║   ██║██║██║         ██║███╗██║██╔══██║██║╚██╗██║██║  ██║",
        "██║ ╚═╝ ██║██║  ██║╚██████╔╝██║╚██████╗    ╚███╔███╔╝██║  ██║██║ ╚████║██████╔╝",
        "╚═╝     ╚═╝╚═╝  ╚═╝ ╚═════╝ ╚═╝ ╚═════╝     ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ",
        "                                                                               "
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
    int rows = sizeof(magic_mand) / sizeof(magic_mand[0]); // 计算数组的行数
    for (int i = 0; i < rows; i++) {
        printf("\033[40;34;1m%s \033[0m\r\n", magic_mand[i]); // 打印每一行
    }
    printf("Author:  krud\r\n");
    printf("Git -v:  %s\r\n", GIT_DESCRIPTION);
    printf("Build:   Feb 05 2025\r\n");
}
