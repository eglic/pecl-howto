#include "base.h"

zend_result zm_real_startup_HowTo(const zend_ini_entry_def *ini_entry, INIT_FUNC_ARGS) {
    //注册全局变量
    // CONST_PERSISTENT 标志是超全局持久，
    // php-fpm 模式下直到进程结束，多次请求会使用同一个值
    // 同时要注意别跟其他扩展的全局常量名字冲突
    REGISTER_STRING_CONSTANT("__HT_VERSION__", PROJECT_VERSION_STR, CONST_CS | CONST_PERSISTENT);

    //在 HT 命名空间下注册个常量
    REGISTER_STRING_CONSTANT("HT\\__VERSION__", PROJECT_VERSION_STR, CONST_CS | CONST_PERSISTENT);

    return SUCCESS;
}