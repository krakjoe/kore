/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe                                                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/spl/spl_exceptions.h"
#include "php_kore.h"

#include "zend_exceptions.h"

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(func_get_named_args_info, 0, 0, IS_ARRAY, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(func_get_named_args_info, 0, 0, IS_ARRAY, NULL, 0)
#endif
	ZEND_ARG_TYPE_INFO(0, format, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto array func_get_named_args([callable format]) */
PHP_FUNCTION(func_get_named_args)
{
	zend_execute_data *frame  = EX(prev_execute_data);
	zval *arg                 = ZEND_CALL_ARG(frame, 1);
	zval *end                 = arg + ZEND_CALL_NUM_ARGS(frame);
	zend_arg_info *info       = frame->func->common.arg_info;
	zend_fcall_info fci       = empty_fcall_info;
	zend_fcall_info_cache fcc = empty_fcall_info_cache;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|f", &fci, &fcc) != SUCCESS) {
		return;
	}

	array_init(return_value);

	if (frame->func->type == ZEND_INTERNAL_FUNCTION) {
		return;
	}

	while (arg < end && info) {
		if (ZEND_NUM_ARGS()) {
			zval name, rv;

			ZVAL_STR(&name, info->name);

			fci.retval       = &rv;
			fci.params       = &name;
			fci.param_count  = 1;

			if (zend_call_function(&fci, &fcc) != SUCCESS) {
				zend_throw_exception_ex(spl_ce_RuntimeException, 0, 
					"failed to call format function for %s", ZSTR_VAL(info->name));
				return;
			}

			if (zend_hash_add(Z_ARRVAL_P(return_value), Z_STR(rv), arg)) {
				Z_TRY_ADDREF_P(arg);
			}

			zval_dtor(&rv);
		} else {
			if (zend_hash_add(Z_ARRVAL_P(return_value), info->name, arg)) {
				Z_TRY_ADDREF_P(arg);
			}
		}

#if PHP_VERSION_ID >= 80000
		if (ZEND_ARG_IS_VARIADIC(info)) {
#else
		if (info->is_variadic) {
#endif			
			break;
		}

		arg++;
		info++;
	}
}
/* }}} */

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(func_get_return_type_info, 0, 0, IS_STRING, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(func_get_return_type_info, 0, 0, IS_STRING, NULL, 1)
#endif
ZEND_END_ARG_INFO()

/* {{{ proto ?string func_get_return_type(void) */
PHP_FUNCTION(func_get_return_type)
{
	zend_execute_data *frame  = EX(prev_execute_data);
	zend_arg_info *info;

	if (!(frame->func->common.fn_flags & ZEND_ACC_HAS_RETURN_TYPE)) {
		return;
	}

	if (frame->func->type == ZEND_INTERNAL_FUNCTION) {
		return;
	}
	
	info = frame->func->common.arg_info - 1;
#if PHP_VERSION_ID >= 80000
	if (ZEND_TYPE_IS_ONLY_MASK(info->type)) {
		switch (ZEND_TYPE_PURE_MASK(info->type)) {
			
			case MAY_BE_STR:
				RETURN_STRING(zend_get_type_by_const(IS_STRING));
			case MAY_BE_LONG:
				RETURN_STRING(zend_get_type_by_const(IS_LONG));
			case MAY_BE_DOUBLE:
				RETURN_STRING(zend_get_type_by_const(IS_DOUBLE));
			case MAY_BE_ARRAY:
				RETURN_STRING(zend_get_type_by_const(IS_ARRAY));
			case MAY_BE_NULL:
				RETURN_STRING("void");
			
			default:
				return;
		}
		
	}
1
	RETURN_STR_COPY(ZEND_TYPE_NAME(info->type));
#elif PHP_VERSION_ID >= 70200
	if (!ZEND_TYPE_IS_CLASS(info->type)) {
		RETURN_STRING(zend_get_type_by_const(ZEND_TYPE_CODE(info->type)));
	}

	RETURN_STR(ZEND_TYPE_NAME(info->type));
#else
	if (info->type_hint != IS_OBJECT) {
		RETURN_STRING(zend_get_type_by_const(info->type_hint));
	}

	RETURN_STR(info->class_name);
#endif
} /* }}} */

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(object_dump_info, 0, 0, IS_ARRAY, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(object_dump_info, 0, 0, IS_ARRAY, NULL, 1)
#endif
	ZEND_ARG_TYPE_INFO(0, object, IS_OBJECT, 0)
ZEND_END_ARG_INFO()

/* {{{ proto array object_dump(object $object) */
PHP_FUNCTION(object_dump)
{
	zval *object = NULL, *v = NULL;
	HashTable *table, *ht;
	zend_string *k = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "o", &object) != SUCCESS) {
		return;
	}

	if (!Z_OBJ_HANDLER_P(object, get_properties)) {
		return;
	}

#if PHP_VERSION_ID >= 80000
	table = Z_OBJ_HANDLER_P(object, get_properties)(Z_OBJ_P(object));
#else
	table = Z_OBJ_HANDLER_P(object, get_properties)(object);
#endif
	if (!table) {
		return;
	}

	array_init(return_value);

	ZEND_HASH_FOREACH_STR_KEY_VAL(table, k, v) {
		const char *class_name, 
					*prop_name;
		zend_long    class_name_len,
					 prop_name_len;

		zend_unmangle_property_name_ex(k, &class_name, &prop_name, &prop_name_len);

		if (class_name) {
			if (class_name[0] == '*') {
				if (zend_hash_str_add(Z_ARRVAL_P(return_value), prop_name, prop_name_len, v)) {
					Z_TRY_ADDREF_P(v);
				}
			} else {
				zend_string *pk = strpprintf(0, "%s::%s", class_name, prop_name);

				if (zend_hash_add(Z_ARRVAL_P(return_value), pk, v)) {
					Z_TRY_ADDREF_P(v);
				}

				zend_string_release(pk);
			}
		} else {
			if (zend_hash_add(Z_ARRVAL_P(return_value), k, v)) {
				Z_TRY_ADDREF_P(v);
			}
		}
	} ZEND_HASH_FOREACH_END();
} /* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(kore)
{
#if defined(COMPILE_DL_KORE) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(kore)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "kore support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ kore_functions[]
 */
const zend_function_entry kore_functions[] = {
	PHP_FE(func_get_named_args,		func_get_named_args_info)
	PHP_FE(func_get_return_type,	func_get_return_type_info)

	PHP_FE(object_dump,             object_dump_info)
	PHP_FE_END
};
/* }}} */

/* {{{ kore_module_entry
 */
zend_module_entry kore_module_entry = {
	STANDARD_MODULE_HEADER,
	"kore",
	kore_functions,
	NULL,
	NULL,
	PHP_RINIT(kore),
	NULL,
	PHP_MINFO(kore),
	PHP_KORE_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_KORE
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(kore)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
