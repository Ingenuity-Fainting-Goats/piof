///  * gcc -shared -o eval.so -fPIC hook.c -g -Wall -I -I/usr/local/include/php -I/usr/local/include/php/main -I/usr/local/include/php/Zend -I/usr/local/include/php/TSRM -I/usr/local/include/php/Zend -I/usr/local/include/php/ext -I/usr/local/include/php/ext/date/lib -I/usr/include/php/20170718 -I/usr/include/php/20170718/main -I/usr/include/php/20170718/TSRM -I/usr/include/php/20170718/Zend -I/usr/include/php/20170718/ext -I/usr/include/php/20170718/ext/date/lib -I ../../


#include "hook.h"
#include <syslog.h>


zend_op_array* hook(zval *source_string, char *filename TSRMLS_DC){
    openlog("piof - eval - hook", LOG_PID, LOG_LOCAL0);
    evalHookStructure running_structure;
    running_structure.pipeline_status.status = 0;
    running_structure.filename = filename;
    running_structure.source_string = Z_STRVAL_P(source_string);
    syslog(LOG_INFO | LOG_LOCAL0, "Parameter Source String: %s",running_structure.source_string);
    syslog(LOG_INFO | LOG_LOCAL0, "Parameter Filename: %s",running_structure.filename);

    closelog();
	return old_compile_string(source_string, filename TSRMLS_CC);
}
