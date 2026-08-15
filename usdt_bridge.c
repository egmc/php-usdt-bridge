#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_usdt_bridge.h"

#include <sys/sdt.h>

ZEND_BEGIN_ARG_INFO_EX(arginfo_call_usdt_bridge, 0, 0, 1)
    ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ void call_usdt_bridge(string $message)
 * Fires the SystemTap USDT probe usdt_bridge:call with the given message,
 * the currently executing script's filename and line number. */
PHP_FUNCTION(call_usdt_bridge)
{
    char *message;
    size_t message_len;
    const char *filename;
    uint32_t lineno;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(message, message_len)
    ZEND_PARSE_PARAMETERS_END();

    filename = zend_get_executed_filename();
    if (!filename) {
        filename = "";
    }
    lineno = zend_get_executed_lineno();

    STAP_PROBE3(usdt_bridge, call, message, filename, lineno);
}
/* }}} */

PHP_MINFO_FUNCTION(usdt_bridge)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "usdt_bridge support", "enabled");
    php_info_print_table_row(2, "Version", PHP_USDT_BRIDGE_VERSION);
    php_info_print_table_row(2, "Probe", "usdt_bridge:call");
    php_info_print_table_end();
}

static const zend_function_entry usdt_bridge_functions[] = {
    PHP_FE(call_usdt_bridge, arginfo_call_usdt_bridge)
    PHP_FE_END
};

zend_module_entry usdt_bridge_module_entry = {
    STANDARD_MODULE_HEADER,
    "usdt_bridge",
    usdt_bridge_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    PHP_MINFO(usdt_bridge),
    PHP_USDT_BRIDGE_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_USDT_BRIDGE
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(usdt_bridge)
#endif
