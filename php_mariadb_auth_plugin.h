/*
  +----------------------------------------------------------------------+
  | PHP Version 8                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2025 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | https://www.php.net/license/3_01.txt                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Georg Richter <georg@php.net>                                |
  +----------------------------------------------------------------------+
*/
#ifndef PHP_MARIADB_AUTH_PLUGIN_H
#define PHP_MARIADB_AUTH_PLUGIN_H

#define PHP_MARIADB_AUTH_PLUGIN_VERSION "0.0.1"

extern zend_module_entry mariadb_auth_plugin_module_entry;
#define phpext_mariadb_auth_plugin_ptr &mariadb_auth_plugin_module_entry

#endif

