PHP_ARG_ENABLE(mariadb_auth_plugin, whether to enable mariadb_auth_plugin, [ --enable-mariadb_auth_plugin Enable MariaDB mysqlnd auth plugin ])

if test "$PHP_MARIADB_AUTH_PLUGIN" != "no"; then
  dnl Check for libsodium library
  AC_CHECK_HEADER(sodium.h, [have_sodium_h=yes], [have_sodium_h=no])
  AC_CHECK_LIB(sodium, sodium_init, [have_sodium_lib=yes], [have_sodium_lib=no])

  if test "$have_sodium_lib" = "no" || test "$have_sodium_h" = "no"; then
    AC_MSG_ERROR([libsodium library and headers not found. Please install libsodium development files.])
  fi

  PHP_ADD_LIBRARY(sodium, 1, MARIADB_AUTH_PLUGIN_SHARED_LIBADD)

  dnl If you depend on PHP's sodium extension:
  PHP_ADD_EXTENSION_DEP(mariadb_auth_plugin, sodium, true)
  PHP_REQUIRE_CXX()
  PHP_ADD_INCLUDE($MYSQLND_DIR/include)
  PHP_NEW_EXTENSION(mariadb_auth_plugin, mariadb_auth.c, $ext_shared)
  PHP_SUBST(MARIADB_AUTH_PLUGIN_SHARED_LIBADD)
fi
