#include "piof_config.h"
#include "php.h"
#include "Zend/zend_ini.h"
#include "zend_exceptions.h"

char *piof_get_ini_value(char *k);
zend_string *get_function_name(zend_string *function_name);
void piof_copy_args(zend_execute_data *execute_data, zval **args, int *ret_num_args);
