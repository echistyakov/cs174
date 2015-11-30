# CS 174 Final Project
Eric Swenson, Evgeny Chistyakov

### System setup
1. Install MySQL:
   * `sudo apt-get install mysql-server` (MySQL server)
   * `sudo apt-get install libmysqlclient-dev` (MySQL header files)
2. Install **m4** (**gmp** library dependens on it)
   * `sudo apt-get install m4`
3. Download/clone this repo
4. Extract the libraries (*paillier/libs*)
   * `tar xvfz libpaillier-0.8.tar.gz`
   * `tar xvfj gmp-6.1.0.tar.bz2` 
5. Install the **gmp** library (depends on **m4**)
   * Navigate to extracted library folder
   * `./configure`
   * `make`
   * `make install` (may require `sudo` mode)
6. Install the **paillier** library (depends on **gmp**)
   * Navigate to extracted library folder
   * `./configure`
   * `make`
   * Open generated *Makefile* in text editor and add `-fPIC` flag to the list of CFLAGS
   * `make install` (may require `sudo` mode)
7. Compile the C code
   * Navigate to **paillier/src**
   * `make`
   * Output binaries are: **encrypt**, **decrypt**, **key_factory**, **sum_he.so**
8. Copy **sum_he.so** to MySQL plugins folder:
   * `cp sum_he.so /usr/lib/mysql/plugin` (may require `sudo` mode)
9. Register **SUM_HE** with MySQL:
   * `CREATE AGGREGATE FUNCTION sum_he RETURNS STRING SONAME 'sum_he.so';`
   * To deregister: `DROP FUNCTION sum_he;`
