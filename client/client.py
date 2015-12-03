from __future__ import print_function, unicode_literals

import sys
import subprocess
import re
from collections import OrderedDict

import mysql.connector
from tabulate import tabulate


config = {
    'user': 'test',
    'password': 'test',
    'host': '127.0.0.1',
    'port': '3306',  # Default MySQL port
    'database': 'project'
}

pub_key = None
prv_key = None

cnx = None
cursor = None


def _exec_bin(args):
    args = (str(x) for x in args)  # args must be of type str
    program = subprocess.Popen(args, stdout=subprocess.PIPE)
    program.wait()
    output = program.stdout.read()
    return output.strip()


def decrypt(cipher_text):
    args = ('./decrypt', pub_key, prv_key, cipher_text)
    return _exec_bin(args)


def encrypt(plain_text):
    args = ('./encrypt', pub_key, plain_text)
    return _exec_bin(args)


def execute(query, commit=False):
    try:
        # print('Executing: {}'.format(query))
        cursor.execute(query)
        if commit:
            cnx.commit()
        return True
    except mysql.connector.Error as err:
        print('MySQL Error: {}'.format(err))
        return False


def run():
    while True:
        print()  # Empty line
        input = raw_input('>> ').strip(' ;')
        tokens = input.split()

        # Exit command
        if input.lower() == 'exit':
            break

        # Query must contain at least two tokens
        if len(tokens) < 2:
            print('Usage Error: Invalid Query')
            continue

        first_token = tokens[0].upper()
        second_token = tokens[1].upper()

        if first_token == 'INSERT':
            if len(tokens) != 4 or not all([x.isdigit() for x in tokens[1:]]):
                print('Usage Error: Invalid Query')
                continue

            # Encrypt salary
            tokens[3] = encrypt(tokens[3])
            query_template = "INSERT INTO Employees VALUES ({}, {}, '{}');"
            query = query_template.format(*tokens[1:])
            success = execute(query, commit=True)
            if success:
                print('Insert Successful')

        elif first_token == 'SELECT' and second_token == 'SUM':
            if re.search('GROUP\s+BY', input.upper()):
                select_query = 'SELECT age, SUM_HE(salary) AS sum_he FROM Employees'
            else:
                select_query = 'SELECT SUM_HE(salary) AS sum_he FROM Employees'

            # Re-join tokens excluding SELECT SUM
            query = select_query + ' ' + ' '.join(tokens[2:]) + ';'
            success = execute(query)
            if success:
                row_list = list()
                for row in cursor.fetchall():
                    row_dict = OrderedDict(zip(cursor.column_names, row))
                    row_dict['sum_he'] = decrypt(row_dict['sum_he'])
                    row_list.append(row_dict.values())
                print(tabulate(row_list, cursor.column_names, tablefmt='psql'))

        elif first_token == 'SELECT' and second_token == 'AVG':
            if re.search('GROUP\s+BY', input.upper()):
                select_query = 'SELECT age, SUM_HE(salary) AS sum_he, COUNT(*) AS num FROM Employees'
            else:
                select_query = 'SELECT SUM_HE(salary) AS sum_he, COUNT(*) AS num FROM Employees'

            # Re-join tokens excluding SELECT AVG
            query = select_query + ' ' + ' '.join(tokens[2:]) + ';'
            success = execute(query)
            if success:
                row_list = list()
                headers = list(cursor.column_names)
                headers.remove('sum_he')
                headers.remove('num')
                headers.append('avg')
                for row in cursor.fetchall():
                    row_dict = OrderedDict(zip(cursor.column_names, row))
                    row_dict['sum_he'] = decrypt(row_dict['sum_he'])
                    row_dict['avg'] = int(row_dict['sum_he']) / row_dict['num']
                    row_dict.pop('sum_he')
                    row_dict.pop('num')
                    row_list.append(row_dict.values())
                print(tabulate(row_list, headers, tablefmt='psql'))

        elif first_token == 'SELECT' and second_token == '*':
            query = 'SELECT * FROM Employees;'
            success = execute(query)
            if success:
                row_list = list()
                for row in cursor.fetchall():
                    row_dict = OrderedDict(zip(cursor.column_names, row))
                    row_dict['salary'] = decrypt(row_dict['salary'])
                    row_list.append(row_dict.values())
                print(tabulate(row_list, cursor.column_names, tablefmt='psql'))

        elif first_token == 'SELECT' and len(tokens) == 2:
            query_template = 'SELECT * FROM Employees WHERE id = {};'
            query = query_template.format(tokens[1])
            success = execute(query)
            if success:
                rows = cursor.fetchall()
                if len(rows) == 1:
                    row_dict = OrderedDict(zip(cursor.column_names, rows[0]))
                    row_dict['salary'] = decrypt(row_dict['salary'])
                    print(tabulate([row_dict], headers='keys', tablefmt='psql'))
                elif len(rows) == 0:
                    print('No matching rows were found!')

        else:
            print('Usage Error: Invalid Query')
            continue


if __name__ == '__main__':
    # Usage
    if len(sys.argv) != 3:
        print('Usage: {} <pub> <prv>'.format(sys.argv[0]))
        sys.exit(1)

    # Set keys
    pub_key = sys.argv[1]
    prv_key = sys.argv[2]

    # Establish connection with the MySQL server
    try:
        cnx = mysql.connector.connect(**config)
        cursor = cnx.cursor()
    except mysql.connector.Error as err:
        print('Connection Error: {}'.format(err))
        sys.exit(1)

    # We don't care if the following fails - 'finally' will execute no matter what
    try:
        run()
    finally:
        # Close connection to MySQL Server
        cursor.close()
        cnx.close()
