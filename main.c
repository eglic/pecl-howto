#include "project.h"
#include "core/zend.h"
#include "life/base.h"

// [1] 声明全局变量
project_global_storage project_globals;

// [2] 一些讨厌的难看的函数参数声明放到单独文件里面
//     单纯是为了干净，因此在 config.m4 里面没有出现
//     切勿单独编译它
#include "args.c"

// [3] php.ini 内与本扩展相关的条目配置，也是极其难看
//     为了干净，扔到单独文件里面，眼不见为净
//     切勿单独编译它
#include "inis.c"

// [4] 注册全局的，暴露给PHP的函数
//    下面的 clang-format off 千万别删，会很丑！！！
// clang-format off
static const zend_function_entry HowTo_function_entrties[] = {
    ZEND_FE_END
};

// [5] 声明对其它扩展的依赖，影响 HowTo 扩展自己的加载顺序
//     老版本的PHP里面， extension = HowTo.so 需要写到依赖后面
//     新版本的PHP貌似会自动管理加载顺序
static const zend_module_dep HowTo_depends[] = {
    ZEND_MOD_REQUIRED("standard")
    ZEND_MOD_REQUIRED("spl")
    ZEND_MOD_REQUIRED("date")
    ZEND_MOD_REQUIRED("hash")
    ZEND_MOD_REQUIRED("json")
    ZEND_MOD_REQUIRED("curl")
    ZEND_MOD_REQUIRED("gd")
    ZEND_MOD_REQUIRED("sqlite3")
    ZEND_MOD_END
};
// clang-format on

// [5] 定义扩展的本体
zend_module_entry HowTo_module_entry = {
    STANDARD_MODULE_HEADER_EX,
    ini_entries,                     /* ini */
    HowTo_depends,                   /* depends */
    PROJECT_NAME_STR,                /* name */
    HowTo_function_entrties,         /* APIS */
    ZEND_MODULE_STARTUP_N(HowTo),    /* MINIT */
    ZEND_MODULE_SHUTDOWN_N(HowTo),   /* MSHUTDOWN */
    ZEND_MODULE_ACTIVATE_N(HowTo),   /* RINIT */
    ZEND_MODULE_DEACTIVATE_N(HowTo), /* RSHUTDOWN */
    ZEND_MODULE_INFO_N(HowTo),       /* phpinfo */
    PROJECT_VERSION_STR,             /* version */
    sizeof(project_global_storage),  /* global size */
    &project_globals,                /* global data */
    zm_globals_ctor_HowTo,           /* global ctor */
    zm_globals_dtor_HowTo,           /* global dtor  */
    NULL,
    STANDARD_MODULE_PROPERTIES_EX};

// [6] 扩展必须的导出函数
ZEND_GET_MODULE(HowTo);

// [7] 模块启动函数，这个强烈建议放到 main.c 里面
//     因为 ini_entries 这个是 static 定义在本文件里面的
//     拿到别的地方访问不到 ini 的配置项
ZEND_MODULE_STARTUP_D(HowTo) {

  return zm_real_startup_HowTo(ini_entries, INIT_FUNC_ARGS_PASSTHRU);
}
