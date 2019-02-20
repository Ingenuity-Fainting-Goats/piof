#include "php.h"
#include "piof_config.h" 


typedef struct{
    pipeline_message pipeline_status;
    char *source_string;    
    char *filename TSRMLS_CC;
} eval_hook_structure;
typedef eval_hook_structure evalHookStructure;
//
zend_op_array* hook(zval *source_string, char *filename TSRMLS_DC);
zend_op_array *(*old_compile_string)(zval *source_string, char *filename TSRMLS_DC);

