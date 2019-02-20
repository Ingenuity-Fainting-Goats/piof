#include "piof_globals.h"
#include "php.h"
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> 
#include <dirent.h> 
#include <string.h>
#include <syslog.h>


#define HANDLERS_POSITIONS "/opt/phprasp/php-extension/hooks_build/"
#define HANDLER_EXTENSION ".so"
#define HOOK_POSTFIX "_hook"


int file_exists(const char *filename)
{
	FILE *file;
	if (file = fopen(filename, "r"))
	{
		fclose(file);
		return 1;
	}
	return 0;
}

void piof_copy_args(zend_execute_data *execute_data, zval **args, int *ret_num_args)
{
	int i, num_args = ZEND_CALL_NUM_ARGS(execute_data), has_scope = 0;
	zval *arguments = emalloc((num_args + 1) * sizeof(zval));
	*args = arguments;

	if (getThis() != NULL)
	{
		has_scope = 1;
		ZVAL_COPY(&arguments[0], getThis());
	}

	for (i = 0; i < num_args; i++)
	{
		ZVAL_COPY(&arguments[i + has_scope], ZEND_CALL_VAR_NUM(execute_data, i));
	}
	*ret_num_args = num_args + has_scope;
}

char *piof_get_ini_value(char *k)
{
	char *v;
	zval *zv;

	v = zend_ini_string(k, strlen(k), 0);

	if (v)
		return v;

	zv = cfg_get_entry(k, strlen(k));

	if (zv)
	{
		return Z_STRVAL_P(zv);
	}

	return v;
}

zend_string *get_function_name(zend_string *function_name)
{
	zend_string *result;
	if (!function_name)
	{
		return NULL;
	}

	return zend_string_copy(function_name);
}


