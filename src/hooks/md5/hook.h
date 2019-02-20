/**
 * @file hook.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-11
 * 
 * @copyright Copyright (c) 2019
 * 
 * gcc -shared -o md5.so -fPIC hook.c -g -Wall -I. -I/usr/local/include/php -I/usr/local/include/php/main -I/usr/local/include/php/Zend-I/usr/local/include/php/TSRM -I/usr/local/include/php/Zend -I/usr/local/include/php/ext -I/usr/local/include/php/ext/date/lib -I/usr/include/php/20170718 -I/usr/include/php/20170718/main -I/usr/include/php/20170718/TSRM -I/usr/include/php/20170718/Zend -I/usr/include/php/20170718/ext -I/usr/include/php/20170718/ext/date/lib -I ../../
 */

#include "php.h"
#include "piof_config.h" 
/**
 * md5_hook_struture
 **/
typedef struct{
    pipeline_message pipeline_status;
    zend_string *parameter;
} md5_hook_structure;
typedef md5_hook_structure md5HookStructure;
//
PipelineMessage hook(INTERNAL_FUNCTION_PARAMETERS, t_executor_internal executor);