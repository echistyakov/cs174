DROP DATABASE IF EXISTS project;
CREATE DATABASE project;
USE project;


CREATE TABLE Employees(
  id INTEGER(10),
  age INTEGER(10),
  salary TEXT NOT NULL,
  PRIMARY KEY(id)
);
