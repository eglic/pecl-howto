#include "base.h"
#include <zend_enum.h>

/**
 * @brief 返回 zval 的内部类型名称
 * @attention 为了这点醋，现包的饺子
 * @attention 用这个在 test 里面打印好看一点
 * @attention 否则 null 和 false 会出现空白
 * @param
 */
PHP_FUNCTION(ht_ztype) {
    zval *val = NULL;
    ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_ZVAL(val)
    ZEND_PARSE_PARAMETERS_END();

    if (val == NULL) {
        //跟 null 区分开
        RETURN_STRING("nil");
    }
    RETURN_STRING(zend_zval_type_name(val));
}