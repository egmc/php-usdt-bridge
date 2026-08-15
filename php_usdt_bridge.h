#ifndef PHP_USDT_BRIDGE_H
#define PHP_USDT_BRIDGE_H

extern zend_module_entry usdt_bridge_module_entry;
#define phpext_usdt_bridge_ptr &usdt_bridge_module_entry

#define PHP_USDT_BRIDGE_VERSION "1.0.0"

PHP_MINFO_FUNCTION(usdt_bridge);
PHP_FUNCTION(call_usdt_bridge);

#endif /* PHP_USDT_BRIDGE_H */
