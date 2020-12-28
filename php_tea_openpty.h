
/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @license MIT
 */

#ifndef __PHP_TEA_OPENPTY_H
#define __PHP_TEA_OPENPTY_H

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <php.h>

PHP_INI_BEGIN()
PHP_INI_END()

ZEND_BEGIN_MODULE_GLOBALS(tea_openpty)
ZEND_END_MODULE_GLOBALS(tea_openpty)

ZEND_EXTERN_MODULE_GLOBALS(tea_openpty)

#define FRESH_TEA_ASMG(v) ZEND_MODULE_GLOBALS_ACCCESSOR(tea_openpty, v)

extern zend_module_entry tea_openpty_module_entry;

#define phpext_tea_openpty_ptr (&tea_openpty_module_entry)

#endif
