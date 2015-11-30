from __future__ import print_function, unicode_literals

import sys

import mysql.connector

# Fill this in with your own configuration values
config = {
    'user': 'cs174a',
    'password': 'password',
    'host': '127.0.0.1',  # Localhost. If your MySQL Server is running on your own computer.
    'port': '3306',  # Default port on Windows/Linux is 3306. On Mac it may be 3307.
    'database': 'airport'
}

try:
    cnx = mysql.connector.connect(**config)
    cursor = cnx.cursor()
except mysql.connector.Error as err:
    print("Connection Error: {}".format(err))
    sys.exit(1)


def execute(query, values):
    your_query = query % values
    print("Executing: {} ... ".format(your_query), end="")
    try:
        cursor.execute(query, values)
    except mysql.connector.Error as err:
        print("ERROR\nMySQL Error: {}\n".format(err))
        sys.exit(1)
    else:
        print("Success")


if __name__ == '__main__':
    # TODO
    
    # Commit data
    cnx.commit()

    cursor.close()
    cnx.close()
