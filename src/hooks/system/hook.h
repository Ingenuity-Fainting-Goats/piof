#include "php.h"
#include "piof_config.h" 
/**
 * system_hook_struture
 **/
typedef struct{
    pipeline_message pipeline_status;
    zend_string *parameter;
} system_hook_structure;
typedef system_hook_structure systemHookStructure;
//
PipelineMessage hook(INTERNAL_FUNCTION_PARAMETERS, t_executor_internal executor);