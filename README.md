# CS 174 Final Project
Eric Swenson, Evgeny Chistyakov

### System Setup
1. Install requred packages:
   * `sudo apt-get install mysql-server` (MySQL server)
   * `sudo apt-get install libmysqlclient-dev` (MySQL header files)
   * `sudo apt-get install m4` (**gmp** library dependens on it)
   * `sudo apt-get install make` (needed for compilation)
2. Download/clone this repo
3. Extract the libraries (*paillier/libs*)
   * `tar xvfz libpaillier-0.8.tar.gz`
   * `tar xvfj gmp-6.1.0.tar.bz2` 
4. Install the **gmp** library (depends on **m4**)
   * Navigate to extracted library folder
   * `./configure`
   * `make`
   * `sudo make install`
5. Install the **paillier** library (depends on **gmp**)
   * Navigate to extracted library folder
   * `./configure`
   * `make`
   * Open generated *Makefile* in text editor and add `-fPIC` flag to the list of CFLAGS
   * `sudo make install`
6. Compile the C code
   * Navigate to **paillier/src**
   * `make`
   * Output binaries are: **encrypt**, **decrypt**, **key_factory**, **sum_he.so**
7. Copy **sum_he.so** to MySQL plugins folder:
   * `cp sum_he.so /usr/lib/mysql/plugin` (may require `sudo` mode)
8. Register **SUM_HE** with MySQL:
   * `CREATE AGGREGATE FUNCTION sum_he RETURNS STRING SONAME 'sum_he.so';`
   * To deregister: `DROP FUNCTION sum_he;`

### Client Setup
1. Install the following:
   * `sudo apt-get install python-pip`
   * `sudo apt-get install python-dev`
   * `sudo pip install mysql-connector-python --allow-external mysql-connector-python`
   * `sudo pip install tabulate`