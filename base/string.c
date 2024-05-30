#include "base.h"

/**
 * @brief 原生字符串基操：大写
 * @attention ★ 不限长度
 * @attention ★ 不改变长度
 * @attention 仅限 ASCII 字符集
 * @param str 待处理字符串，不可传 const
 * @param len
 * @return bool 是否改变内容
 */
bool nstr_upper(char *str, size_t len) {
    if (str == NULL || len < 1)
        return false;
    size_t i  = 0;
    bool   ch = false;
    for (i = 0; i < len; i++) {
        if (*(str + i) >= 'a' && *(str + i) <= 'z') {
            ch = true;
            *(str + i) -= 0x20;
        }
    }
    return ch;
}

/**
 * @brief 原生字符串基操：小写
 * @attention ★ 不限长度
 * @attention ★ 不改变长度，只返回 result_code_t
 * @attention 仅限 ASCII 字符集
 * @param str 待处理字符串，不可传 const
 * @param len
 * @return
 */
bool nstr_lower(char *str, size_t len) {
    if (str == NULL || len < 1)
        return false;
    size_t i  = 0;
    bool   ch = false;
    for (i = 0; i < len; i++) {
        if (*(str + i) >= 'A' && *(str + i) <= 'Z') {
            ch = true;
            *(str + i) += 0x20;
        }
    }
    return ch;
}

/**
 * @brief 判断字符串是否空的
 * @attention ★ 不限长度
 * @param str
 * @param len
 * @return
 */
bool nstr_empty(const char *str, size_t len) {
    if (str == NULL)
        return true;
    if (len < 1)
        len = strlen(str);
    if (len < 1)
        return true;
    for (size_t i = 0; i < len; i++) {
        if (!NSTR_IS_ASCII_SPACE(str[i]))
            return false;
    }
    return true;
}

/**
 * @brief 判断字符串中是否包含指定字符
 * @param str
 * @param len
 * @param fnd
 * @return
 */
bool nstr_contains(const char *str, size_t len, char fnd) {
    if (str == NULL)
        return false;
    if (len < 1)
        len = strlen(str);
    if (len < 1)
        return false;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == fnd)
            return true;
    }
    return false;
}

/**
 * @brief 在字符串内进行单字符替换
 * @param str
 * @param len
 * @param f1
 * @param f2
 * @return
 */
bool nstr_replace(char *str, size_t len, char f1, char f2) {
    if (str == NULL)
        return false;
    if (len < 1)
        len = strlen(str);
    if (len < 1)
        return true;
    if (f1 == f2)
        return true;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == f1)
            str[i] = f2;
    }
    return true;
}

/**
 * @brief 原生字符串基操：修剪
 * @attention ★ 限制长度 2G
 * @attention ★ 返回新长度，若返回0长度一般都是有问题
 * @param str 待修剪字符，不可传 const
 * @param len 字符串长度，传 0 将会重新计算
 * @return uint32_t 修剪后的长度
 */
uint32_t nstr_trim(char *str, uint32_t len) {
    if (str == NULL)
        return 0;
    if (len == 0) {
        size_t nl = strlen(str);
        if (nl >= UINT32_MAX)
            return 0;
        len = nl;
    }

    if (len == 1) {
        if (NSTR_IS_ASCII_SPACE(*str)) {
            *str = 0;
            return 0;
        }
        return len;
    }

    long     p = -1;
    uint32_t i = 0;
    for (i = 0; i < len; i++) {
        if (NSTR_IS_ASCII_SPACE(str[i])) {
            continue;
        }
        p = i;
        break;
    }

    // 1. 全空
    if (p < 0) {
        memset(str, 0, len);
        return 0;
    }

    // 2. 前面有空白
    if (p > 0) {
        memcpy(str, str + p, len - p);
        len -= p;
        str[len] = '\0';
    }
    if (len < 1)
        return 0;
    for (i = len - 1; i > 0; i--) {
        if (str[i] == 0) {
            len--;
            continue;
        }
        if (NSTR_IS_ASCII_SPACE(str[i])) {
            str[i] = '\0';
            len--;
            continue;
        }
        break;
    }
    return (uint32_t)len;
}

	/**
 * @brief 对只读字符串进行修剪操作
 * @attention ★ 限制长度 2G  ， 且不修改原字符串
 * @attention ★ np = NULL 时，不返回新的头指针
 * @attention ★ nl = NULL 是，不返回新长度
 * @attention ★ 返回值表示原字符串是否需要修剪
 * @param str
 * @param len
 * @param np
 * @param nl
 * @return
 */
bool nstr_trim_ro(const char *str, uint32_t len, const char **np, uint32_t *nl) {
    if (str == NULL || (np == NULL && nl == NULL))
        return false;
    if (len == 0) {
        size_t tl = strlen(str);
        if (tl >= UINT32_MAX)
            return 0;
        len = (uint32_t)tl;
    }
    int64_t p = -1, q = -1;
    //正向查找
    for (uint32_t i = 0; i < len; i++) {
        if (NSTR_IS_ASCII_SPACE(str[i]))
            continue;
        p = i;
        break;
    }
    if (p < 0) { //全空，新指针直接指向最末端
        if (np != NULL)
            *np = str + len;
        if (nl != NULL)
            *nl = 0;
        return true;
    }
    //反向查找
    for (uint32_t i = len - 1; len >= (uint32_t)p; i--) {
        if (NSTR_IS_ASCII_SPACE(str[i]))
            continue;
        q = i;
        break;
    }
    if (q < 0) { //同样全空
        if (np != NULL)
            *np = str + len;
        if (nl != NULL)
            *nl = 0;
        return true;
    }
    if (p == q) { //就剩一个字符了
        if (np != NULL)
            *np = str + p;
        if (nl != NULL)
            *nl = 1;
        return true;
    }
    if (p == 0 && q == len - 1) { //无需裁剪
        if (np != NULL)
            *np = str;
        if (nl != NULL)
            *nl = len;
        return false;
    }
    if (np != NULL)
        *np = str + p;
    if (nl != NULL)
        *nl = q - p + 1;
    return true;
}

