#include "base.h"

void zm_globals_ctor_HowTo(void *raw) {
    //先情况全局变量存储
    memset(raw, 0, sizeof(project_global_storage));

    //变换指针
    project_global_storage *MG = (project_global_storage *)raw;

    //手动初始化
    MG->enable = true;
}