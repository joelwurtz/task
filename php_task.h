/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Authors: Martin Schröder <m.schroeder2007@gmail.com>                 |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_TASK_H
#define PHP_TASK_H

#include "fiber.h"
#include "awaitable.h"
#include "context.h"
#include "task.h"
#include "task_scheduler.h"

extern zend_module_entry task_module_entry;
#define phpext_task_ptr &task_module_entry

#define PHP_TASK_VERSION "0.1.0"

#ifdef PHP_WIN32
#define PHP_TASK_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define PHP_TASK_API __attribute__ ((visibility("default")))
#else
#define PHP_TASK_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define TASK_DEBUG 0

#ifndef TASK_DEBUG
#define TASK_DEBUG 0
#endif

#if TASK_DEBUG > 0
#ifdef __GNUC__
#define TASK_DEBUG_PRINT(message, ...) php_printf(message, ##__VA_ARGS__); php_printf("\n");
#define TASK_DEBUG_PRINTF(fiber, message, ...) php_printf("[%d] ", (int) ((concurrent_task *) fiber)->id); php_printf(message, ##__VA_ARGS__); php_printf("\n");
#else
#define TASK_DEBUG_PRINT(message, ...) php_printf(message, __VA_ARGS__); php_printf("\n");
#define TASK_DEBUG_PRINTF(fiber, message, ...) php_printf("[%d] ", (int) ((concurrent_task *) fiber)->id); php_printf(message, __VA_ARGS__); php_printf("\n");
#endif
#else
#define TASK_DEBUG_PRINT(message, ...)
#define TASK_DEBUG_PRINTF(fiber, message, ...)
#endif

ZEND_BEGIN_MODULE_GLOBALS(task)
	/* Root fiber context (main thread). */
	concurrent_fiber_context root;

	/* Active fiber, NULL when in main thread. */
	concurrent_fiber *current_fiber;

	/* Default fiber C stack size. */
	zend_long stack_size;

	/* Error to be thrown into a fiber (will be populated by throw()). */
	zval *error;

	/* Active async task scheduler. */
	concurrent_task_scheduler *scheduler;

	zend_object *fatal;

	size_t counter;

ZEND_END_MODULE_GLOBALS(task)

PHP_TASK_API ZEND_EXTERN_MODULE_GLOBALS(task)
#define TASK_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(task, v)

#if defined(ZTS) && defined(COMPILE_DL_TASK)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif

/*
 * vim: sw=4 ts=4
 * vim600: fdm=marker
 */
