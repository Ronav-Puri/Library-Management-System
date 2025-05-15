# Library Management System
A mini project, part of the course CS253: Software Development and Operations, during my fourth semester at IITK.

This is a simple Library Management System implemented in C++. It enables users to manage books and user accounts, facilitating actions such as borrowing, returning, and reserving books.  

## Features  

- View all books, available books, borrowed books, reserved books, and overdue books.  
- Borrow, return, and reserve books.  
- Manage user accounts (add, remove, and update users).  
- Calculate fines for overdue books.  

## Requirements  

- C++ compiler (e.g., `g++`)  
- Standard C++ library  

## Files  

- `main.cpp` – Contains the implementation of the Library Management System.  
- `book.csv` – Stores book data.  
- `user.csv` – Stores user data.  

## Compilation  

To compile the project, run:  

```sh
g++ -o lms main.cpp
./lms
