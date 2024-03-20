/* Create test_db*/
CREATE DATABASE IF NOT EXISTS test_db;
USE test_db;

/* Create a new users table*/
CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT,
    first_name VARCHAR(255) NOT NULL,
    last_name VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE,
    active BOOL DEFAULT TRUE,
    PRIMARY KEY(id)
);

/* Verify users table layout*/
DESC users;

/* Create a role_dev and show default privileges */
CREATE ROLE role_dev;
SHOW GRANTS FOR role_dev;

/* Create a role_dev and show default privileges */
GRANT ALL ON test_db.* to role_dev;
SHOW GRANTS FOR role_dev;

/* Create a new user called test_dev1@localhost */
CREATE USER test_dev1@localhost IDENTIFIED BY 'Secure123!!';

/* Apply the role to the new user and verify */
GRANT role_dev to test_dev1@localhost;
SHOW GRANTS FOR test_dev1@localhost USING role_dev;

/* Activate all of user's roles after logging into MySQL */
SET DEFAULT ROLE ALL TO test_dev1@localhost;


