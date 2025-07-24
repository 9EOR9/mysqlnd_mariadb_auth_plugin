# mysqlnd_mariadb_auth

This is a collection of MariaDB specific plugins for mysqlnd, provided as a PECL module.

## Building and Installation

1.  **Prepare your environment:**
    Make sure you have PHP development headers and `mysqlnd` development files installed.

2.  **Navigate to the module directory:**
    ```bash
    cd mysqlnd_mariadb_auth_plugin
    ```

3.  **Run `phpize`:**
    ```bash
    phpize
    ```

4.  **Configure:**
    ```bash
    ./configure --enable-mysqlnd_mariadb_auth
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
    extension=mysqlnd_mariadb_auth.so
    ```

## Usage

This plugin registers an authentication method named `ed25519`. To use it, your MariaDB server would need to be configured to use this authentication method for a specific user.

**MariaDB Server Configuration (Example):**

Install the ed25519 authentication support:

```sql
INSTALL SONAME 'auth_ed25519'


On the MariaDB server, you would typically use `CREATE USER` or `ALTER USER` with the `AUTHENTICATION PLUGIN` clause. For example:

```sql
CREATE USER 'myuser'@'localhost' IDENTIFIED WITH ed25519 BY 'secret';
