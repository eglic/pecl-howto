#include "base.h"
#include <zend_enum.h>

/*
        实现一个函数 zval_empty
        跟  php 的 empty() 函数相比的区别：
        1. 将 '0' 			判定为 false : 非空的
        2. 将 "" \t\r\n  "	判定为 true : 是空的
*/

extern zend_class_entry *zend_standard_class_def;

// 实际函数声明是
// void zif_zval_empty (zend_execute_data *execute_data, zval *return_value)

/**
 * @brief 判断一个PHP变量是否为空
 * @param  mixed $val 任意变量
 */
PHP_FUNCTION(ht_empty) {

    //指针类型变量，务必给 NULL 默认值
    //否则后面调试根本找不到问题
    zval *val = NULL;

    // 开始从 execute_data 取参数 , 0:最少0个参数, 1:最多1个参数
    ZEND_PARSE_PARAMETERS_START(0, 1)
    // 从这个开始后面就是可选参数
    Z_PARAM_OPTIONAL
    Z_PARAM_ZVAL(val)
    ZEND_PARSE_PARAMETERS_END();

    //没有传参的情况
    if (val == NULL)
        RETURN_TRUE;

    // [1] 两种间接类型先处理
    if (Z_TYPE_P(val) == IS_INDIRECT)
        val = Z_INDIRECT_P(val);
    else if (Z_TYPE_P(val) == IS_REFERENCE)
        val = Z_REFVAL_P(val);

    // [2] 传的是未初始化成员或者 null
    if (Z_TYPE_P(val) == IS_UNDEF || Z_TYPE_P(val) == IS_NULL)
        RETURN_TRUE;

    // [3] 将 false 判定为空
    if (Z_TYPE_P(val) == IS_FALSE)
        RETURN_TRUE;

    // [4] 将 true 判断为非空
    if (Z_TYPE_P(val) == IS_TRUE)
        RETURN_FALSE;

    // [5] 将 int 型的非零值判定为非空，零判断为空
    if (Z_TYPE_P(val) == IS_LONG) {
        if (Z_LVAL_P(val) == 0)
            RETURN_TRUE;
        RETURN_FALSE;
    }

    // [6] 将很小的 double 判定为空
    if (Z_TYPE_P(val) == IS_DOUBLE) {
        // double类型与 0.0 难以比较
        //与 0.0 的差值小于 10亿分之1
        if (abs(Z_DVAL_P(val) - (double)0.0) < (double)1e-10)
            RETURN_TRUE;
        RETURN_FALSE;
    }

    // [7] string 类型判定
    //长度 < 1 空
    //全部是 空白字符串 空
    if (Z_TYPE_P(val) == IS_STRING) {
        if (Z_STRLEN_P(val) < 1)
            RETURN_TRUE;
        for (size_t i = 0; i < Z_STRLEN_P(val); i++) {
            //出非空白字符就判断为非空
            //暂时只支持 ASCII 字符集
            // UTF8 有点难度
            if (!NSTR_IS_ASCII_SPACE(Z_STRVAL_P(val)[i]))
                RETURN_FALSE;
        }
        RETURN_TRUE;
    }

    // [8] 两种内部类型不好判断，草草了之
    if (Z_TYPE_P(val) == IS_PTR)
        RETURN_FALSE;
    else if (Z_TYPE_P(val) == IS_RESOURCE)
        RETURN_FALSE;

    // [9] array 类型判定
    if (Z_TYPE_P(val) == IS_ARRAY) {
        //数组有个特殊的值，PHP代码里敲一对 [] ，底层就是这个
        if (Z_ARR_P(val) == &zend_empty_array)
            RETURN_TRUE;
        //无论是哪种类型的数组， zend_array_count 都可信
        if (zend_array_count(Z_ARR_P(val)) < 1)
            RETURN_TRUE;
        RETURN_FALSE;
    }

    // [10] object 类型判断
    // 对一个刚刚 new 出来的 stdClass ，还没设置任何成员的 判断为空
    // 对枚举类型，且后端是 int ，且值等于0的判断为空
    if (Z_TYPE_P(val) == IS_OBJECT) {
        if (Z_OBJCE_P(val) == zend_standard_class_def) {
            // stdClass 是支持动态属性的，与其他类不一样
            if (Z_OBJ_P(val)->properties == NULL)
                RETURN_TRUE;
            if (Z_OBJ_P(val)->properties == &zend_empty_array)
                RETURN_TRUE;
            if (zend_array_count(Z_OBJ_P(val)->properties) < 1)
                RETURN_TRUE;
            RETURN_FALSE;
        }
        if ((Z_OBJCE_P(val)->ce_flags & ZEND_ACC_ENUM) == ZEND_ACC_ENUM &&
            Z_OBJCE_P(val)->enum_backing_type == IS_LONG) {
            zval *v = zend_enum_fetch_case_value(Z_OBJ_P(val));
            if (v != NULL && Z_TYPE_P(v) == IS_LONG && Z_LVAL_P(v) == 0)
                RETURN_TRUE;
            RETURN_FALSE;
        }
        //其它的不管了
        RETURN_FALSE;
    }

    // [11] 其它没见过的，都当非空
    RETURN_FALSE;
}