#pragma once
#include "../project.h"
#include "../core/zend.h"

typedef struct {
  bool enable;
} project_global_storage;

extern project_global_storage project_globals;
#define _HTG(v) project_globals.v

// 看，这就是 clang-foramt 格式后的丑样
// 凑合看吧
#define PROJECT_INI_ENTRY(name, default_value, on_modify, property_name)       \
  ZEND_INI_ENTRY2(PROJECT_INI_PREFIX name, default_value, ZEND_INI_SYSTEM,     \
                  on_modify,                                                   \
                  (void *)XtOffsetOf(project_global_storage, property_name),   \
                  (void *)&project_globals)

// 声明 main 和 life/*.c 需要的函数
ZEND_MODULE_STARTUP_D(HowTo);
ZEND_MODULE_ACTIVATE_D(HowTo);
ZEND_MODULE_DEACTIVATE_D(HowTo);
ZEND_MODULE_SHUTDOWN_D(HowTo);
ZEND_MODULE_INFO_D(HowTo);

//这两个不用 Zend 自己的宏，可以减少编译时的一个警告
void zm_globals_ctor_HowTo(void *raw);
void zm_globals_dtor_HowTo(void *raw);

//下面这个是 ZEND_MODULE_STARTUP_D(HowTo) 的影分身
//  分开原因看 main.c
zend_result zm_real_startup_HowTo(const zend_ini_entry_def *ini_entry,
                                  INIT_FUNC_ARGS);

#define ZEND_TYPE_REGISTER_N(tn) __reg_howto_##tn
#define ZEND_TYPE_REGISTER_D(tn) void __reg_howto_##tn(int module_number)
#define ZEND_TYPE_REGISTER(tn) __reg_howto_##tn(module_number)

void setup_type_base(int module_number);
void setup_type_enum(int module_number);
