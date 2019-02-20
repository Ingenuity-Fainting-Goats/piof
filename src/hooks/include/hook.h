#include "php.h"
#include "piof_config.h" 


typedef struct{
    pipeline_message pipeline_status;
    zend_file_handle *file_handle;
} include_hook_structure;
typedef include_hook_structure includeHookStructure;
//
zend_op_array* hook(zend_file_handle *file_handle, int type);
zend_op_array *(*old_compile_file)(zend_file_handle *file_handle, int type);

