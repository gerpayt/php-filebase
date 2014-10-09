 /*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_filebase.h"
/* If you declare any globals in php_filebase.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(filebase)
*/

/* True global resources - no need for thread safety here */
static int le_filebase;


/* {{{ PHP_FUNCTION
*/
PHP_FUNCTION(filebase_get)
{
	char *bucket, *path;
	int bucket_len, path_len;
	const char *root = INI_STR("filebase.root");
	const zend_bool debug = INI_BOOL("filebase.debug");
	const int root_len = strlen(root);
	php_stream *stream;

	char *filename;
	char subpath[3];

	int options;
	int file_size;
	char *buf;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &bucket, &bucket_len, &path, &path_len) == FAILURE)
	{
		return;
	}

	filename =(char*)emalloc((root_len + bucket_len + path_len + 5)*sizeof(char));
	subpath[0] = path[path_len - 2];
	subpath[1] = path[path_len - 1];
	subpath[2] = '\0';
	php_sprintf(filename, "%s/%s/%s/%s", root, bucket, subpath, path);
	
	if(debug){
		php_printf("DEBUG: filename:%s\n", filename);
	}

	options = ENFORCE_SAFE_MODE | STREAM_MUST_SEEK;
	
	stream = php_stream_open_wrapper(filename, "r", options, NULL);

	if (!stream) {
		RETURN_FALSE;
	}
	
	php_stream_seek(stream, 0, SEEK_END);
	file_size = php_stream_tell(stream);
	php_stream_seek(stream, 0, SEEK_SET);
	buf = (char*)malloc((file_size+1)*sizeof(char));
	php_stream_read(stream, buf, file_size);
	buf[file_size] = '\0';
	php_stream_free(stream, PHP_STREAM_FREE_CLOSE_PERSISTENT);

	efree(filename);

	RETURN_STRING(buf, 1);
}
ZEND_BEGIN_ARG_INFO(arginfo_filebase_get, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP_FUNCTION
*/
PHP_FUNCTION(filebase_put)
{
	char *bucket, *path, *content;
	int bucket_len, path_len, content_len;
	const char *root = INI_STR("filebase.root");
	const zend_bool debug = INI_BOOL("filebase.debug");
	const int root_len = strlen(root);
	php_stream *stream, *stream_dir;

	char *filename;
	char *dirname;
	char subpath[3];

	int options;
	int file_size;

	int res_mkdir;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss", &bucket, &bucket_len, &path, &path_len, &content, &content_len) == FAILURE)
	{
		return;
	}

	filename = (char*)emalloc((root_len + bucket_len + path_len + 5)*sizeof(char));
	dirname = (char*)emalloc((root_len + bucket_len + path_len + 5)*sizeof(char));

	subpath[0] = path[path_len - 2];
	subpath[1] = path[path_len - 1];
	subpath[2] = '\0';

	php_sprintf(filename, "%s/%s/%s/%s", root, bucket, subpath, path);
	php_sprintf(dirname, "%s/%s/%s", root, bucket, subpath);

	if(debug){
		php_printf("DEBUG: filename:%s\n        dirname:%s\n", filename, dirname);
	}

	options = ENFORCE_SAFE_MODE | STREAM_MUST_SEEK;

	stream_dir = php_stream_opendir(dirname, options, NULL);
	if (!stream_dir)
	{
		res_mkdir = php_stream_mkdir(dirname, 0755, options, NULL);
		if (!res_mkdir) {
			RETURN_FALSE;
		}
	}
	stream = php_stream_open_wrapper(filename, "w+", options, NULL);

	if (!stream) {
		RETURN_FALSE;
	}

	file_size = php_stream_write(stream, content, content_len);
	php_stream_free(stream, PHP_STREAM_FREE_CLOSE_PERSISTENT);
	efree(filename);
	efree(dirname);
	RETURN_LONG(file_size);
}
ZEND_BEGIN_ARG_INFO(arginfo_filebase_put, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ PHP_FUNCTION
*/
PHP_FUNCTION(filebase_del)
{
	char *bucket, *path;
	int bucket_len, path_len;
	const char *root = INI_STR("filebase.root");
	const zend_bool debug = INI_BOOL("filebase.debug");
	const int root_len = strlen(root);
	int result;

	char *filename;
	char subpath[3];

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &bucket, &bucket_len, &path, &path_len) == FAILURE)
	{
		return;
	}

	filename = (char*)emalloc((root_len + bucket_len + path_len + 5)*sizeof(char));

	subpath[0] = path[path_len - 2];
	subpath[1] = path[path_len - 1];
	subpath[2] = '\0';

	php_sprintf(filename, "%s/%s/%s/%s", root, bucket, subpath, path);

	if(debug){
		php_printf("DEBUG: filename:%s\n", filename);
	}

	// TODO use php wrapper function
	result = unlink(filename);
	if (result)
	{
		RETURN_FALSE;
	}

	efree(filename);
	RETURN_TRUE;
}
ZEND_BEGIN_ARG_INFO(arginfo_filebase_del, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ filebase_functions[]
 *
 * Every user visible function must have an entry in filebase_functions[].
 */
const zend_function_entry filebase_functions[] = {
	PHP_FE(filebase_get, arginfo_filebase_get)
	PHP_FE(filebase_put, arginfo_filebase_put)
	PHP_FE(filebase_del, arginfo_filebase_del)
	PHP_FE_END	/* Must be the last line in filebase_functions[] */
};
/* }}} */

/* {{{ filebase_module_entry
 */
zend_module_entry filebase_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"filebase",
	filebase_functions,
	PHP_MINIT(filebase),
	PHP_MSHUTDOWN(filebase),
	NULL,
	NULL,
	PHP_MINFO(filebase),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_FILEBASE_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_FILEBASE
ZEND_GET_MODULE(filebase)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    PHP_INI_ENTRY("filebase.root", "filebase", PHP_INI_ALL, NULL)
    PHP_INI_ENTRY("filebase.debug", "0", PHP_INI_ALL, NULL)
PHP_INI_END()
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(filebase)
{
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(filebase)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(filebase)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "filebase support", "enabled");
	php_info_print_table_row(2, "author", "Chen Feng");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
