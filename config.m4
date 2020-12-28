
dnl config.m4

PHP_ARG_ENABLE(tea_openpty, for tea_openpty support, [  --enable-tea-openpty            Include tea_openpty support])

PHP_ADD_INCLUDE(.)

PHP_NEW_EXTENSION(tea_openpty, tea_openpty.c php_tea_openpty.c classes/TeaOpenPTY/TeaOpenPTY.c, $ext_shared)

PHP_ADD_LIBRARY(util, 1, TEA_OPENPTY_SHARED_LIBADD)
PHP_SUBST(TEA_OPENPTY_SHARED_LIBADD)
