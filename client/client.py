from __future__ import print_function, unicode_literals

import sys
import subprocess
import mysql.connector
import re
import tabulate

# Fill this in with your own configuration values
config = {
    'user': 'cs174a',
    'password': 'password',
    'host': '127.0.0.1',  # Localhost. If your MySQL Server is running on your own computer.
    'port': '3306',  # Default port on Windows/Linux is 3306. On Mac it may be 3307.
    'database': 'airport'
}

def _exec_bin(args):
    popen = subprocess.Popen(args, stdout=subprocess.PIPE)
    popen.wait()
    output = popen.stdout.read()
    return output

def decrypt(cipher_text, pub_key, prv_key):
    args = ("decrypt", pub_key, prv_key, cipher_text)
    return _exec_bin(args)

def encrypt(plain_text, pub_key):
    args = ("encrypt", pub_key, plain_text)
    return _exec_bin(args)


def execute(cursor, query):
    # print("Executing: {} ... ".format(query), end="")
    try:
        cursor.execute(query)
        return None;
    except mysql.connector.Error as err:
        # print("ERROR\nMySQL Error: {}\n".format(err))
        return str(err);


def main(pub_key, prv_key):
    cnx = None
    cursor = None
    try:
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()
    except mysql.connector.Error as err:
        print("Connection Error: {}".format(err))
        sys.exit(1)

    while(True):
        input = raw_input(">> ").strip()

        if input == 'exit':
            break

        tokens = input.split()

        if tokens[0].lower() == 'insert':
            if len(tokens) != 4:
                print("USAGE Invalid Query, expecting INSERT <emp_id> <emp_age> <emp_salary>")
                continue
            # encrypt
            token[3] = encrypt(token[3], pub_key)
            insert_query = 'INSERT INTO Employees VALUES ({}, {}, {});'
            formatted_query = insert_query.format(token[1], token[2], token[3])
            error = execute(cursor, formatted_query)
            if error:
                print("ERROR {}\n".format(error))
            else:
                print("Success\n")
        elif tokens[0].lower() == 'select':
            if tokens[1].lower() == 'sum':
                select_query = 'SELECT SUM_HE(salary) AS sum FROM Employees '
                if re.search("group\s+by", input.lower()):
                    select_query = 'SELECT SUM_HE(salary) AS sum, age FROM Employees '
                # Re-join tokens excluding SELECT SUM
                select_query + " ".join(tokens[2:]) + ';'
                error = execute(cursor, select_query)
                if error:
                    print("ERROR {}".format(error))
                else:
                    row_list = None
                    for row in cursor.fetchall():
                        row_dict = dict(zip(cursor.column_names, row))
                        row_dict['sum'] = decrypt(row_dict['sum'], pub_key, priv_key)
                        row_list.append(row_dict.values())
                    print tabulate(row_list, cursor.column_names,tablefmt="psql")
            elif tokens[1].lower() == 'avg':
                select_query = 'SELECT SUM_HE(salary) AS sum, COUNT(*) AS num FROM Employees '
                if re.search("group\s+by", input.lower()):
                    select_query = 'SELECT SUM_HE(salary) AS sum, COUNT(*) AS num, age FROM Employees '
                # Re-join tokens excluding SELECT SUM
                select_query + " ".join(tokens[2:]) + ';'
                error = execute(cursor, select_query)
                if error:
                    print("ERROR {}".format(error))
                else:
                    row_list = None
                    headers = list(cursor.column_names)
                    headers.remove('sum')
                    headers.remove('num')
                    for row in cursor.fetchall():
                        row_dict = dict(zip(cursor.column_names, row))
                        row_dict['sum'] = decrypt(row_dict['sum'], pub_key, priv_key)
                        row_dict['avg'] = row_dict['sum'] / row_dict['num']
                        row_dict.pop('sum')
                        row_dict.pop('num')
                        row_list.append(row_dict.values())
                    print tabulate(row_list, headers, tablefmt="psql")
            elif tokens[1] == '*':
                select_query = 'SELECT * FROM Employees;'
                error = execute(cursor, select_query)
                if error:
                    print("ERROR {}".format(error))
                else:
                    row_list = None
                    for row in cursor.fetchall():
                        row_dict = dict(zip(cursor.column_names, row))
                        row_dict['salary'] = decrypt(row_dict['salary'], pub_key, priv_key)
                        row_list.append(row_dict.values())
                    print tabulate(row_list, cursor.column_names,tablefmt="psql")
            elif len(tokens) == 2
                select_query = 'SELECT * FROM Employees WHERE id = {};'
                formatted_query = select_query.format(token[1])
                error = execute(cursor, formatted_query)
                if error:
                    print("ERROR {}".format(error))
                else:
                    row_dict = dict(zip(cursor.column_names, cursor.fetchone()))
                    row_dict['salary'] = decrypt(row['salary'], pub_key, priv_key)
                    print tabulate([row_dict], cursor.column_names,tablefmt="psql")
            else
                print("USAGE Invalid Query, expecting ...")
                continue
            pass
        else:
            print("ERROR Invalid Query\n")
            continue


    # Commit data
    # cnx.commit()

    cursor.close()
    cnx.close()


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: {} <pub> <prv>\n".format(sys.argv[0]))
        sys.exit(1)
    main(argv[1], argv[2])
