# demoSQLwCpp
Demo of how to use the soci library with a MySQL db on an Ubuntu OS.  
This demo was inspired by the post: https://dane-bulat.medium.com/working-with-databases-in-c-an-introduction-7d6a6a78ae66

## Install prerequisites

Install gcc:
```
sudo apt update
sudo apt install build-essential
```

Install cmake:
```
sudo apt install cmake
```

Install MySQL server:  
If running Ubuntu in a Windows Subsystem for Linux enable systemd(https://learn.microsoft.com/en-us/windows/wsl/systemd#how-to-enable-systemd). Edit (or create) ```/etc/wsl.conf``` to add:
```
[boot]
systemd=true
```

Install my MySQL server:
```
sudo apt-get install mysql-server
```

Install libmysqlclient:
```
sudo apt-get install mysqlclient
```

Check the mysqlclient library install:   
The include location at:
```
ls /usr/include/mysql
```
should contain the library includes (\*.h files). The library location at:
```
ls /usr/lib/mysql/plugin
```
should contain the library shared objects (\*.so files).

### Creating the MySQL test_db and creating a user

Start the MySQL server:
```
sudo systemctl status mysql
```

Login to sql as root (use your OS password):
```
sudo mysql
```

Create the test_db, table, user and login permissions (The SQL commands are in create_test_db.sql):

```
mysql> source create_test_db.sql
```

Exit the mysql prompt:
```
mysql> exit
```

## Install soci

Clone the soci repo and use cmake to install it:
```
git clone https://github.com/SOCI/soci.git
git checkout -b release/4.0 
cd soci
mkdir build
cd build
cmake -G "Unix Makefiles" -DWITH_MYSQL=ON -DSOCI_CXX11=ON ..
make
sudo make install
```

Verify the install by checking these locations for header files:
```
ls ls /usr/local/include/soci
ls /usr/local/include/soci/mysql
``` 
and this location for shared objects (libsoci_\*.so.\*):
```
ls /usr/local/lib/
```

## Compile C++ and run

Set some environment variables:
```
export CPATH="/usr/local/include/soci:/usr/include/mysql"
export LD_LIBRARY_PATH="/usr/local/lib:/usr/lib/mysql"
```

Compile sqlwcpp.cpp:
```
g++ -std=c++11 sqlwcpp.cpp -o demosqlwcpp -lsoci_core -lsoci_mysql -ldl -lmysqlclient
```

Run the application:
```
./demosqlwcpp
```