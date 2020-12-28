
/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @license MIT
 */

#include <php.h>

#include "tea_openpty.h"
#include "php_tea_openpty.h"
#include "classes/TeaOpenPTY/TeaOpenPTY.h"

ZEND_GET_MODULE(tea_openpty);
ZEND_DECLARE_MODULE_GLOBALS(tea_openpty);

/**
 * MINIT
 */
static
PHP_MINIT_FUNCTION(tea_openpty)
{
  zend_class_entry ce;

  INIT_CLASS_ENTRY(ce, "TeaOpenPTY\\TeaOpenPTY", methods_TeaOpenPTY__TeaOpenPTY);
  ce_TeaOpenPTY__TeaOpenPTY = zend_register_internal_class(&ce TSRMLS_CC);

  REGISTER_INI_ENTRIES();
  return SUCCESS;
}

/**
 * SHUTDOWN
 */
static
PHP_MSHUTDOWN_FUNCTION(tea_openpty)
{
  UNREGISTER_INI_ENTRIES();
  return SUCCESS;
}

/**
 * GLOBAL INIT.
 */
static
PHP_GINIT_FUNCTION(tea_openpty)
{
#if defined(COMPILE_DL_ASTKIT) && defined(ZTS)
  ZEND_TSRMLS_CACHE_UPDATE();
#endif
}

zend_module_entry tea_openpty_module_entry = {
  STANDARD_MODULE_HEADER,
  "tea_openpty",
  NULL, /* functions */
  PHP_MINIT(tea_openpty),
  PHP_MSHUTDOWN(tea_openpty),
  NULL, /* RINIT */
  NULL, /* RSHUTDOWN */
  NULL, /* MINFO */
  "0.1",
  PHP_MODULE_GLOBALS(tea_openpty),
  PHP_GINIT(tea_openpty),
  NULL, /* GSHUTDOWN */
  NULL, /* RPOSTSHUTDOWN */
  STANDARD_MODULE_PROPERTIES_EX
};
