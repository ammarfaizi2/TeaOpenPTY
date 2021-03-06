
/**
 * @author Ammar Faizi <ammarfaizi2@gmail.com> https://www.facebook.com/ammarfaizi2
 * @license MIT
 */

#include "TeaOpenPTY.h"
#include "tea_openpty.h"

zend_class_entry *ce_TeaOpenPTY__TeaOpenPTY;

#define cc_ce ce_TeaOpenPTY__TeaOpenPTY

typedef union {
  tea_openpty_t *data;
  char          *p_data;
  tea_openpty_t **pp_data;
  char          arr_ptr[sizeof(tea_openpty_t *)];
} tea_opty_u;

/**
 * Constructor.
 *
 * @param string $app
 */
static
PHP_METHOD(TeaOpenPTY__TeaOpenPTY, __construct)
{
  char       *app;
  size_t     len;
  zval       *_this;
  tea_opty_u opty;

  ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_STRING(app, len)
  ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

  _this     = getThis();
  opty.data = emalloc(sizeof(tea_openpty_t));
  memset(opty.data, 0, sizeof(tea_openpty_t));

  zend_update_property_stringl(
    cc_ce, _this, ZEND_STRL("app"),
    app, len TSRMLS_CC);

  zend_update_property_stringl(
    cc_ce, _this, ZEND_STRL("opty"),
    opty.arr_ptr, sizeof(opty.arr_ptr) TSRMLS_CC);
}


/**
 * Destructor.
 *
 * @param string $app
 */
static
PHP_METHOD(TeaOpenPTY__TeaOpenPTY, __destruct)
{
  zval       rv;
  zval       *_this;
  zval       *zopty;
  tea_opty_u opty;

  _this = getThis();
  zopty = zend_read_property(cc_ce, _this, ZEND_STRL("opty"), 1, &rv TSRMLS_CC);

  if (zopty != NULL) {
    opty.p_data = Z_STRVAL_P(zopty);
    efree(*(opty.pp_data));
  }
}


/**
 * @param string ...$argv
 * @return int
 */
static
PHP_METHOD(TeaOpenPTY__TeaOpenPTY, exec)
{
  int           argc;
  zval          rv;
  zval          *_this;
  zval          *zopty;
  zval          *args;
  zval          *app;
  tea_opty_u    opty;
  tea_openpty_t *data;

  ZEND_PARSE_PARAMETERS_START(1, -1)
    Z_PARAM_VARIADIC('*', args, argc)
  ZEND_PARSE_PARAMETERS_END();

  _this = getThis();
  app   = zend_read_property(cc_ce, _this, ZEND_STRL("app"), 1, &rv TSRMLS_CC);
  zopty = zend_read_property(cc_ce, _this, ZEND_STRL("opty"), 1, &rv TSRMLS_CC);

  opty.p_data = Z_STRVAL_P(zopty);
  opty.data   = *(opty.pp_data);
  data        = opty.data;
  data->app   = Z_STRVAL_P(app);
  data->argv  = emalloc(sizeof(char *) * (argc + 1));

  for (int i = 0; i < argc; i++) {
    data->argv[i] = Z_STRVAL_P((args + i));
  }
  data->argv[argc] = NULL;

  int ret = tea_openpty(data);
  efree(data->argv);

  RETURN_LONG(ret);
}


/**
 * @return ?string
 */
static
PHP_METHOD(TeaOpenPTY__TeaOpenPTY, error)
{
  zval          rv;
  zval          *_this;
  zval          *zopty;
  tea_opty_u    opty;
  tea_openpty_t *data;

  _this = getThis();
  zopty = zend_read_property(cc_ce, _this, ZEND_STRL("opty"), 1, &rv TSRMLS_CC);

  opty.p_data = Z_STRVAL_P(zopty);
  opty.data   = *(opty.pp_data);
  data        = opty.data;

  if (data->error[0] == 0) {
    return;
  }

  RETURN_STRING(data->error);
}


zend_function_entry methods_TeaOpenPTY__TeaOpenPTY[] = {
  PHP_ME(TeaOpenPTY__TeaOpenPTY, __construct, NULL, ZEND_ACC_CTOR | ZEND_ACC_PUBLIC)
  PHP_ME(TeaOpenPTY__TeaOpenPTY, exec, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(TeaOpenPTY__TeaOpenPTY, error, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(TeaOpenPTY__TeaOpenPTY, __destruct, NULL, ZEND_ACC_DTOR | ZEND_ACC_PUBLIC)
  PHP_FE_END
};
