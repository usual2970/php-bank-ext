/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
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
#include "php_bank.h"
#include "cmap.c"
struct StrHashTable tbl = {{0},NULL,NULL,simple_strhash,strcmp};
zval *cfg_value;
/* If you declare any globals in php_bank.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(bank)
*/

/* True global resources - no need for thread safety here */
static int le_bank;

/* {{{ bank_functions[]
 *
 * Every user visible function must have an entry in bank_functions[].
 */
const zend_function_entry bank_functions[] = {
	PHP_ME(Bank, bankinfo, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in bank_functions[] */
};
/* }}} */

/* {{{ bank_module_entry
 */
zend_module_entry bank_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"Bank",
	bank_functions,
	PHP_MINIT(bank),
	PHP_MSHUTDOWN(bank),
	PHP_RINIT(bank),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(bank),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(bank),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_BANK_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_BANK
ZEND_GET_MODULE(bank)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("bank.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_bank_globals, bank_globals)
    STD_PHP_INI_ENTRY("bank.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_bank_globals, bank_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_bank_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_bank_init_globals(zend_bank_globals *bank_globals)
{
	bank_globals->global_value = 0;
	bank_globals->global_string = NULL;
}
*/
/* }}} */

zend_class_entry *Bank_ce;

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(bank)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	REGISTER_STRING_CONSTANT("BANK_VERSION", "0.1", CONST_CS | CONST_PERSISTENT);
    //REGISTER_INI_ENTRIES();
    zend_class_entry Bank_entry;
    INIT_CLASS_ENTRY(Bank_entry, "Bank", bank_functions);
    Bank_ce = zend_register_internal_class_ex(&Bank_entry, NULL, NULL TSRMLS_CC);//注册类

    char *cfg_name="bin_data_path";
    cfg_value = cfg_get_entry(cfg_name, strlen(cfg_name) + 1);
    if(!cfg_value){
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "bin_data_path should be set in php.ini");
		return SUCCESS;
	}
    
	
	FILE *fp;
	char bin_key[8];
	char bin_value[100];
	
	int pre_len = 6;
	//hashmap
	if((fp=fopen(Z_STRVAL_P(cfg_value),"r"))==NULL) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Cannot open bin data file");
		return SUCCESS;
	}else{
		int line_no = 0;
		while (fgets(bin_key,pre_len+1,fp)!=NULL) {
			fseek(fp,-pre_len,SEEK_CUR);
			fgets(bin_value,150,fp);
			strncpy(bin_keys[line_no], bin_key, pre_len); 
			strncpy(bin_values[line_no], bin_value, 150); 
			cmap_insert(&tbl,bin_keys[line_no],bin_values[line_no]);
			
			//this total cost 3+ seconds
			//cmap_insert(&tbl,malloc_value(phone_key),malloc_value(phone_value));
			line_no++;
		}
		
	}
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(bank)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(bank)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(bank)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(bank)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "bank support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_bank_compiled(string arg)
   Return a string to confirm that the module is compiled in */


/* {{{ bank::bankinfo
 */
PHP_METHOD(Bank, bankinfo) {

	char *arg = NULL;
	int arg_len;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	FILE *fp;
	char str[7];
	char result[150];
	char str_value[150];
	int has_result = 0;
	int pre_len = 6;
	char bin_pre[7];
	if(arg_len<pre_len){
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Argument length should be 6 or more");
		return;
	}
	//zend_printf("debug:%s",result);
	strncpy(bin_pre, arg, 6); 
	//开启开关，则载入扩展时，读取数据载入内存
	
	if(cmap_get(&tbl,bin_pre)){
		has_result = 1;
		strncpy(result, (char*)cmap_get(&tbl,bin_pre), 150); 
	}
	
	
	if(has_result == 0){
		//php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Cannot find this mobile phone number in data file");
		return;
	}else{
		array_init(return_value);
		char *delim = "\t";
		char *slice_value;
		//BIN号	发卡行	发卡行代码	卡种名称	银行卡类型	卡号长度	
		char value_keys[5][10] = {
			"bank_name",
			"bank_code",
			"kind_name",
			"bank_type",
			"card_len"
		};
		//zend_printf("if %s,%s,%s",strstr(strtok(result, delim),phone_pre),strtok(result, delim),phone_pre);
		if(strstr(strtok(result, delim),bin_pre)){
			int i;
			for(i=0;i<5;i++){
				slice_value = strtok(NULL, delim);
				//zend_printf("v:%s",slice_value);
				if(slice_value){
					add_assoc_string(return_value,value_keys[i],slice_value,1);
				}
			}
		}
	}

    
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
