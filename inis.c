#pragma once
#include "core/zend.h"
#include "life/base.h"
#include <zend_ini.h>

// clang-format off
ZEND_INI_BEGIN()
    PROJECT_INI_ENTRY( "enable"           , "0"           ,  OnUpdateBool             , enable )
ZEND_INI_END()
// clang-format on
