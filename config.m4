dnl $Id$
dnl config.m4 for extension kore

PHP_ARG_ENABLE(kore, whether to enable kore support,
[  --enable-kore           Enable kore support])

if test "$PHP_KORE" != "no"; then
  PHP_NEW_EXTENSION(kore, kore.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
