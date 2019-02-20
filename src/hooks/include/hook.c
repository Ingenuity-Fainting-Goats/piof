///  * gcc -shared -o include.so -fPIC hook.c -g -Wall -I -I/usr/local/include/php -I/usr/local/include/php/main -I/usr/local/include/php/Zend -I/usr/local/include/php/TSRM -I/usr/local/include/php/Zend -I/usr/local/include/php/ext -I/usr/local/include/php/ext/date/lib -I/usr/include/php/20170718 -I/usr/include/php/20170718/main -I/usr/include/php/20170718/TSRM -I/usr/include/php/20170718/Zend -I/usr/include/php/20170718/ext -I/usr/include/php/20170718/ext/date/lib -I ../../


#include "hook.h"
#include <syslog.h>


zend_op_array* hook(zend_file_handle *file_handle, int type){
    openlog("piof - include - hook", LOG_PID, LOG_LOCAL0);
    includeHookStructure running_structure;
    running_structure.pipeline_status.status = 0;
    running_structure.file_handle = file_handle;
    syslog(LOG_INFO | LOG_LOCAL0, "Parameter %s",running_structure.file_handle->filename);
    closelog();
    return old_compile_file(file_handle, type);
}
