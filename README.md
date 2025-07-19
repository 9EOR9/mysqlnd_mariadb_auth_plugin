# mariadb_auth_plugin

This is a skeleton for a `mysqlnd` authentication plugin, specifically named for MariaDB compatibility, as a PECL module.

## Building and Installation

1.  **Prepare your environment:**
    Make sure you have PHP development headers and `mysqlnd` development files installed.
    On Debian/Ubuntu: `sudo apt-get install php-dev libmysqlclient-dev` (or `libmysqlnd-dev` if available and preferred).

2.  **Navigate to the module directory:**
    ```bash
    cd mariadb_auth_plugin
    ```

3.  **Run `phpize`:**
    ```bash
    phpize
    ```

4.  **Configure:**
    ```bash
    ./configure --with-mariadb-auth-plugin
    ```

5.  **Compile:**
    ```bash
    make
    ```

6.  **Install:**
    ```bash
    sudo make install
    ```

7.  **Enable the extension in `php.ini`:**
    Add the following line to your `php.ini` file:
    ```ini
    extension=mariadb_auth_plugin.so
    ```

8.  **Restart your web server or PHP-FPM.**

## Usage

This plugin registers an authentication method named `my_mariadb_auth_method`. To use it, your MariaDB server would need to be configured to use this authentication method for a specific user.

**MariaDB Server Configuration (Example - Highly simplified for a custom plugin):**

On the MariaDB server, you would typically use `CREATE USER` or `ALTER USER` with the `AUTHENTICATION PLUGIN` clause. For example:

```sql
CREATE USER 'myuser'@'localhost' IDENTIFIED WITH my_mariadb_auth_method BY 'some_password';
