#include "php.h"
#include "piof_config.h"
#include "piof_globals.h"
#include "Zend/zend_ini.h"
#include "zend_exceptions.h"
#include <unistd.h>
#include <syslog.h>
#include <SAPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> 
#include <dirent.h> 
#include <string.h>

#define HANDLERS_POSITIONS "/opt/piof/hooks/build/"
#define HANDLER_EXTENSION ".so"

void (*original_zend_execute_ex)(zend_execute_data *execute_data);
void (*original_zend_execute_internal)(zend_execute_data *execute_data, zval *return_value);
zend_op_array *(*old_compile_string)(zval *source_string, char *filename TSRMLS_DC);
zend_op_array *(*old_compile_file)(zend_file_handle *file_handle, int type);
void piof_zend_execute_ex(zend_execute_data *execute_data TSRMLS_DC);
void piof_zend_execute_internal(INTERNAL_FUNCTION_PARAMETERS);

zend_class_entry *piof_exception;
void piof_init_exception(TSRMLS_D)
{
	zend_class_entry e;
	INIT_CLASS_ENTRY(e, "piofException", NULL);
	piof_exception = zend_register_internal_class_ex(&e, zend_exception_get_default());
}

ZEND_DECLARE_MODULE_GLOBALS(piof)
zend_module_entry piof_module_entry = {
	STANDARD_MODULE_HEADER,
	PIOF_NAME,
	NULL,				/* All exposed functions, only to test POC, will get removed later */
	PHP_MINIT(piof), /* On module startup */
	PHP_MSHUTDOWN(piof),
	PHP_RINIT(piof),
	PHP_RSHUTDOWN(piof),
	NULL, /* Module info, used in phpinfo(); */
	PIOF_VERSION,
	STANDARD_MODULE_PROPERTIES};

ZEND_GET_MODULE(piof)

void piof_zend_execute_ex(zend_execute_data *execute_data)
{
	original_zend_execute_ex(execute_data);
	return;
}

void resume_execute_internal(INTERNAL_FUNCTION_PARAMETERS)
{
	if (original_zend_execute_internal)
	{
		original_zend_execute_internal(INTERNAL_FUNCTION_PARAM_PASSTHRU);
	}
	else
	{
		execute_data->func->internal_function.handler(INTERNAL_FUNCTION_PARAM_PASSTHRU);
	}
}

void piof_zend_execute_internal(INTERNAL_FUNCTION_PARAMETERS)
{
	zend_string *function_name = get_function_name(
		EG(current_execute_data)->func->common.function_name);
	zval *piof_pre_hook;
	zval *piof_validator;

	zend_string *validator_name;

	if (function_name == NULL)
	{
		resume_execute_internal(INTERNAL_FUNCTION_PARAM_PASSTHRU);
		return;
	}

	piof_pre_hook = zend_hash_find(PIOF_G(piof_hooked_functions), function_name);

	if (piof_pre_hook == NULL)
	{
		resume_execute_internal(INTERNAL_FUNCTION_PARAM_PASSTHRU);
		zend_string_release(function_name);
	}
	else
	{
		t_hook *hook_ptr = (t_hook *)(Z_LVAL(*piof_pre_hook));
		PipelineMessage result = hook_ptr(INTERNAL_FUNCTION_PARAM_PASSTHRU, resume_execute_internal);
		if (result.status < 0)
		{
			zend_throw_exception(piof_exception, "piof.c Custom Exception zend_throw_exception", 0L);
		}
	}
}

bool piof_hook_function(const char *method_name, t_hook *hook)
{
	openlog("piof - piof_hook_function", LOG_PID, LOG_LOCAL0);
	zend_function *function;
	zend_string *f_name = zend_string_init(method_name, strlen(method_name), 0);
	if (!hook)
	{
		syslog(LOG_DEBUG | LOG_LOCAL0, "[ERROR] Unable to apply hook, no hook address provided");
		return false;
	}
	if ((function = zend_hash_str_find_ptr(CG(function_table), method_name, strlen(method_name))) != NULL)
	{
		if (hook != NULL)
		{
			zval zv;
			ZVAL_LONG(&zv, (long)*hook);
			zend_hash_update(PIOF_G(piof_hooked_functions), f_name, &zv);
			syslog(LOG_DEBUG | LOG_LOCAL0, "Hooked function %s with handler %p", method_name, *hook);
		}
		closelog();
		return true;
	}
	else
	{
		syslog(LOG_DEBUG | LOG_LOCAL0, "[ERROR] Unable to locate function '%s' in global function table", method_name);
		closelog();
		return false;
	}
}


int load_shared_objects()
{
	openlog("piof - load_shared_objects", LOG_PID, LOG_LOCAL0);
	syslog(LOG_DEBUG | LOG_LOCAL0, "Loading hooking modules from %s", HANDLERS_POSITIONS);
    DIR *directory_ptr;
    struct dirent *dir;
    directory_ptr = opendir(HANDLERS_POSITIONS);
    if (directory_ptr) {
        while ((dir = readdir(directory_ptr)) != NULL) {
            char *file_extension = strrchr(dir->d_name, '.');
            if(strcmp(file_extension, HANDLER_EXTENSION)==0){
                void* handle = NULL;
                const char* error_message = NULL;
                char module_path[256];
				memset(module_path, '\0', sizeof(module_path));
                strncat(module_path,HANDLERS_POSITIONS,sizeof(module_path));
                strncat(module_path,dir->d_name,sizeof(module_path));
                handle = dlopen(module_path, RTLD_LAZY );
                if( !handle ){
					syslog(LOG_DEBUG | LOG_LOCAL0, "[ERROR] - dlopen() %s", dlerror() );
                    return 1;
                }

				// method router
				if (strcmp(dir->d_name, "include.so") == 0){
					t_hook_compiler *hook= dlsym(handle, "hook");
					if ((error_message = dlerror()) != NULL)  {
						syslog(LOG_DEBUG | LOG_LOCAL0, "[ERROR] - dlopen() %s", error_message );
						dlclose( handle );
						return 1;
					}
					old_compile_file = zend_compile_file;
					zend_compile_file = hook;
					syslog(LOG_DEBUG | LOG_LOCAL0, "Hooked function zend_compile_file with handler %p", *hook);
				} else if(strcmp(dir->d_name, "eval.so") == 0){
					t_hook_string_compiler *hook= dlsym(handle, "hook");
					if ((error_message = dlerror()) != NULL)  {
						syslog(LOG_DEBUG | LOG_LOCAL0, "[ERROR] - dlopen() %s", error_message );
						dlclose( handle );
						return 1;
					}
					old_compile_string = zend_compile_string;
					zend_compile_string = hook;
					syslog(LOG_DEBUG | LOG_LOCAL0, "Hooked function zend_compile_string with handler %p", *hook);
				}else {
					t_hook *hook = dlsym(handle, "hook");
					if ((error_message = dlerror()) != NULL)  {
						syslog(LOG_DEBUG | LOG_LOCAL0, "[ERROR] - dlopen() %s", error_message );
						dlclose( handle );
						return 1;
					}
					char *hook_name = malloc(strlen(dir->d_name));
					memset(hook_name, '\0', sizeof(hook_name));
					memcpy(hook_name, dir->d_name, strlen(dir->d_name)-3);
					hook_name[strlen(dir->d_name)-3] = '\0';
					piof_hook_function(hook_name, hook);
					free(hook_name);
				}
           }
        }
    closedir(directory_ptr);
    }
	closelog();
    return 0;
}

PHP_MINIT_FUNCTION(piof)
{
	openlog("piof - MINIT", LOG_PID, LOG_LOCAL0);
	ALLOC_HASHTABLE(PIOF_G(piof_hooked_functions));
	zend_hash_init(PIOF_G(piof_hooked_functions), 16, NULL, ZVAL_PTR_DTOR, 0);

	load_shared_objects();

	original_zend_execute_ex = zend_execute_ex;
	zend_execute_ex = piof_zend_execute_ex;
	syslog(LOG_DEBUG | LOG_LOCAL0, "Hooked zend_execute_ex %p", *piof_zend_execute_ex);

	original_zend_execute_internal = zend_execute_internal;
	zend_execute_internal = piof_zend_execute_internal;
	syslog(LOG_DEBUG | LOG_LOCAL0, "Hooked zend_execute_internal %p", *piof_zend_execute_internal);


	closelog();
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(piof)
{
	openlog("piof - MSHUTDOWN", LOG_PID, LOG_LOCAL0);
	closelog();
	return SUCCESS;
}

PHP_RINIT_FUNCTION(piof)
{
	openlog("piof - RINIT", LOG_PID, LOG_LOCAL0);
	closelog();
	return SUCCESS;

}

PHP_RSHUTDOWN_FUNCTION(piof)
{
	openlog("piof - RSHUTDOWN", LOG_PID, LOG_LOCAL0);
	syslog(LOG_DEBUG | LOG_LOCAL0, "Cleaning hooked functions... ");
	zend_execute_ex = original_zend_execute_ex;
	zend_execute_internal = original_zend_execute_internal;
	zend_compile_string = old_compile_string;
	zend_compile_file = old_compile_file;
	zend_hash_destroy(PIOF_G(piof_hooked_functions));
	FREE_HASHTABLE(PIOF_G(piof_hooked_functions));
	closelog();
	return SUCCESS;
}
