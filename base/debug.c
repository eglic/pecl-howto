#include "base.h"

/**
 * @brief 向控制台输出调试信息
 * @param func
 * @param level
 * @param fmt
 * @param
 */
void nlog_console_echo(const char *func, log_level_t level, const char *fmt, ...) {
    switch (level) {
    case LL_DEBUG: //绿色
        printf("\033[0;32m[DEBUG]\033[0m");
        break;
    case LL_INFO: //黑色
        printf("\033[0;0m[INFO]\033[0m");
        break;
    case LL_DONE: //蓝色
        printf("\033[0;34m[DONE]\033[0m");
        break;
    case LL_WARN: //黄色
        printf("\033[0;33m[WARN]\033[0m");
        break;
    case LL_ERROR: //红色
        printf("\033[0;31m[ERROR]\033[0m");
        break;
    case LL_FATAL: //红底白字
        printf("\033[0;37;41m[FATAL]\033[0m");
        break;
    default:
        return;
    }
    if (func == NULL)
        printf(",UNKNOWN,");
    else
        printf(",%s,", func);

    va_list vp;
    va_start(vp, fmt);
    vprintf(fmt, vp);
    va_end(vp);
    printf("\n");
}