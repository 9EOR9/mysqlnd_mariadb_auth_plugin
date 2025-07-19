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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/mysqlnd/mysqlnd.h"
#include "ext/mysqlnd/mysqlnd_auth.h"
#include "ext/mysqlnd/mysqlnd_plugin.h"
#include "php_mariadb_auth_plugin.h"
#include <sodium.h>

#define SHA512_LENGTH 64
#define NONCE_LENGTH 32

int ma_crypto_sign(
  unsigned char *sm,                     // output: 32 bytes R || 32 bytes signature scalar
  const unsigned char *m, unsigned long long mlen, // message (scramble)
  const unsigned char *pw, unsigned long long pwlen // password
)
{
    unsigned char az[crypto_hash_sha512_BYTES];
    unsigned char nonce[crypto_hash_sha512_BYTES];
    unsigned char hram[crypto_hash_sha512_BYTES];
    unsigned char r[crypto_core_ed25519_SCALARBYTES];
    unsigned char s[crypto_core_ed25519_SCALARBYTES];
    unsigned char k[crypto_core_ed25519_SCALARBYTES];
    unsigned char R[crypto_core_ed25519_BYTES];
    unsigned char A[crypto_core_ed25519_BYTES];

    /* az = SHA512(password) */
    crypto_hash_sha512(az, pw, pwlen);

    /* Clamp az[:32] to produce the secret scalar s */
    az[0] &= 248;
    az[31] &= 63;
    az[31] |= 64;
    memcpy(s, az, crypto_core_ed25519_SCALARBYTES);

    /* nonce = SHA512(az[32:] || m) */
    crypto_hash_sha512_state hs;
    crypto_hash_sha512_init(&hs);
    crypto_hash_sha512_update(&hs, az + 32, 32);
    crypto_hash_sha512_update(&hs, m, mlen);
    crypto_hash_sha512_final(&hs, nonce);

    /* r = reduce(nonce) */
    crypto_core_ed25519_scalar_reduce(r, nonce);

    /* R = r * basepoint */
    crypto_scalarmult_ed25519_base_noclamp(R, r);

    /* A = s * basepoint */
    crypto_scalarmult_ed25519_base_noclamp(A, s);

    /* hram = SHA512(R || A || m) */
    crypto_hash_sha512_init(&hs);
    crypto_hash_sha512_update(&hs, R, crypto_core_ed25519_BYTES);
    crypto_hash_sha512_update(&hs, A, crypto_core_ed25519_BYTES);
    crypto_hash_sha512_update(&hs, m, mlen);
    crypto_hash_sha512_final(&hs, hram);

    /* k = reduce(hram) */
    crypto_core_ed25519_scalar_reduce(k, hram);

    /* ks = k * s */
    unsigned char ks[crypto_core_ed25519_SCALARBYTES];
    crypto_core_ed25519_scalar_mul(ks, k, s);

    /* S = ks + r */
    unsigned char S[crypto_core_ed25519_SCALARBYTES];
    crypto_core_ed25519_scalar_add(S, ks, r);

    /* output sm = R || S */
    memcpy(sm, R, crypto_core_ed25519_BYTES);
    memcpy(sm + crypto_core_ed25519_BYTES, S, crypto_core_ed25519_SCALARBYTES);

    return 0;
}

static zend_uchar *
mariadb_ed25519_auth(struct st_mysqlnd_authentication_plugin * self,
								   size_t * auth_data_len,
							 	   MYSQLND_CONN_DATA * conn, const char * const user, const char * const passwd,
								   const size_t passwd_len, zend_uchar * auth_plugin_data, const size_t auth_plugin_data_len,
								   const MYSQLND_SESSION_OPTIONS * const session_options,
								   const MYSQLND_PFC_DATA * const pfc_data,
								   const zend_ulong mysql_flags
								  )
{
  zend_uchar * ret = NULL;

  if (auth_plugin_data_len != NONCE_LENGTH)
    return NULL;

  if (passwd && passwd_len)
  {
    ret = calloc(SHA512_LENGTH + 1, 1);

    if (!(ma_crypto_sign(ret, auth_plugin_data, auth_plugin_data_len, passwd, passwd_len)))
    {
      *auth_data_len = SHA512_LENGTH;
      ret[SHA512_LENGTH] = 0;
    } else {
      free(ret);
      ret= NULL;
    }
  }
  return ret;
}

static struct st_mysqlnd_authentication_plugin mariadb_ed25519_auth_plugin = {
    {
        MYSQLND_PLUGIN_API_VERSION,
        "auth_plugin_client_ed25519",
        PHP_VERSION_ID,
        PHP_MARIADB_AUTH_PLUGIN_VERSION,
        "PHP License 3.01",
        "Georg Richter <georg@php.net>",
        {NULL, NULL},
        {NULL},
    },
    mariadb_ed25519_auth
};

PHP_MINIT_FUNCTION(mariadb_auth_plugin)
{
    mysqlnd_plugin_register_ex((struct st_mysqlnd_plugin_header *)&mariadb_ed25519_auth_plugin);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(mariadb_auth_plugin)
{
    return SUCCESS;
}

zend_module_entry mariadb_auth_plugin_module_entry = {
    STANDARD_MODULE_HEADER,
    "mariadb_auth_plugin",
    NULL,
    PHP_MINIT(mariadb_auth_plugin),
    PHP_MSHUTDOWN(mariadb_auth_plugin),
    NULL,
    NULL,
    NULL,
    PHP_MARIADB_AUTH_PLUGIN_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_MARIADB_AUTH_PLUGIN
ZEND_GET_MODULE(mariadb_auth_plugin)
#endif

