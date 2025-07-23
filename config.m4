PHP_ARG_ENABLE(mysqlnd_mariadb_auth, whether to enable mariadb_auth_plugin, [ --enable-mysqlnd_mariadb_auth Enable MariaDB authentication plugins for mysqlnd ])

if test "$PHP_MYSQLND_MARIADB_AUTH" != "no"; then
  dnl Check for libsodium library
  AC_CHECK_HEADER(sodium.h, [have_sodium_h=yes], [have_sodium_h=no])
  AC_CHECK_LIB(sodium, sodium_init, [have_sodium_lib=yes], [have_sodium_lib=no])

  if test "$have_sodium_lib" = "no" || test "$have_sodium_h" = "no"; then
    AC_MSG_ERROR([libsodium library and headers not found. Please install libsodium development files.])
  fi

  PHP_ADD_LIBRARY(sodium, 1, MYSQLND_MARIADB_AUTH_SHARED_LIBADD)

  PHP_ADD_EXTENSION_DEP(mysqlnd_mariadb_auth, mysqlnd, true)
  PHP_ADD_EXTENSION_DEP(mysqlnd_mariadb_auth, sodium, true)
  PHP_REQUIRE_CXX()
  PHP_ADD_INCLUDE($MYSQLND_DIR/include)
  PHP_NEW_EXTENSION(mysqlnd_mariadb_auth, mariadb_auth.c, $ext_shared)
  PHP_SUBST(MYSQLND_MARIADB_AUTH_SHARED_LIBADD)
fi
