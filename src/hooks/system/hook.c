///  * gcc -shared -o system.so -fPIC hook.c -g -Wall -I -I/usr/local/include/php -I/usr/local/include/php/main -I/usr/local/include/php/Zend -I/usr/local/include/php/TSRM -I/usr/local/include/php/Zend -I/usr/local/include/php/ext -I/usr/local/include/php/ext/date/lib -I/usr/include/php/20170718 -I/usr/include/php/20170718/main -I/usr/include/php/20170718/TSRM -I/usr/include/php/20170718/Zend -I/usr/include/php/20170718/ext -I/usr/include/php/20170718/ext/date/lib -I ../../

#include "hook.h"
#include <syslog.h>

PipelineMessage hook(INTERNAL_FUNCTION_PARAMETERS, t_executor_internal executor)
{
    openlog("piof - system - hook", LOG_PID, LOG_LOCAL0);
    systemHookStructure running_structure;
    running_structure.pipeline_status.status = 0;
    ZEND_PARSE_PARAMETERS_START(1, 1);
    Z_PARAM_STR(running_structure.parameter);
    ZEND_PARSE_PARAMETERS_END();
    syslog(LOG_INFO | LOG_LOCAL0, "Arguments %d",ZEND_CALL_NUM_ARGS(execute_data));
    syslog(LOG_INFO | LOG_LOCAL0, "Parameter %s",ZSTR_VAL(running_structure.parameter));
    closelog();
    executor(INTERNAL_FUNCTION_PARAM_PASSTHRU);
    return running_structure.pipeline_status; 
}
