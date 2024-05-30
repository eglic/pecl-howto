#include "base.h"

ZEND_MODULE_SHUTDOWN_D(HowTo) {

    zend_unregister_ini_entries_ex(module_number, type);
	return SUCCESS;
}