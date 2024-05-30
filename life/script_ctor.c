#include "base.h"

ZEND_MODULE_ACTIVATE_D(HowTo) {
    //注册脚本作用域的常量（宏）
    REGISTER_LONG_CONSTANT("__NOW__", time(NULL), CONST_CS);
    return SUCCESS;
}