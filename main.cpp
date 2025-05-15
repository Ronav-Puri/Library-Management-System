#include <bits/stdc++.h>
using namespace std;

#define RESET "\033[0m"
#define BOLD_YELLOW "\033[1;33m"
#define LIGHT_BLUE "\033[1;36m"
#define GREEN "\033[1;32m"
#define BOLD_RED "\033[1;31m"
#define BOLD_WHITE "\033[1;37m"

// Function to calculate the number of days between two dates
int noOfDays(string start, string end){ 
    int monthDays[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    int d1 = stoi(start.substr(0,2));
    int m1 = stoi(start.substr(3,2));
    int y1 = stoi(start.substr(6,4));

    int d2 = stoi(end.substr(0,2));
    int m2 = stoi(end.substr(3,2));
    int y2 = stoi(end.substr(6,4));

    for(int i=0; i<m1-1; i++) d1 += monthDays[i];
    if(m1>2 && (y1%4==0 && y1%100!=0) || (y1%400==0)) d1++;
    d1 += y1*365 + y1/4 - y1/100 + y1/400;
    
    for(int i=0; i<m2-1; i++) d2 += monthDays[i];
    if(m2>2 && (y2%4==0 && y2%100!=0) || (y2%400==0)) d2++;
    d2 += y2*365 + y2/4 - y2/100 + y2/400;

    return d2-d1;
}

// Function to calculate the due date after a certain number of days
string calculateDueDate(string start, int days){
    int d1 = stoi(start.substr(0,2));
    int m1 = stoi(start.substr(3,2));
    int y1 = stoi(start.substr(6,4));

    int monthDays[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    d1 += days;
    while(d1>monthDays[m1-1]){
        d1-= monthDays[m1-1];
        m1++;
        if(m1>12){
            m1=1;
            y1++;
        }
    }

    string due_d = (d1<10 ? "0" : "") + to_string(d1);
    string due_m = (m1<10 ? "0" : "") + to_string(m1);
    string due_y = to_string(y1);

    return due_d + '/' + due_m + '/' + due_y;
}

// Function to check if the date is in the correct format
bool checkDate(string date){
    if(date.size()!=10) return false;
    if(date[2]!='/' || date[5]!='/') return false;
    if(stoi(date.substr(0,2))>31 || stoi(date.substr(0,2))<1) return false;
    if(stoi(date.substr(3,2))>12 || stoi(date.substr(3,2))<1) return false;
    if(stoi(date.substr(6,4))<0) return false;
    return true;
}

class Book{
    private:
        int id;
        string title, author, publisher, isbn;
        int year;
        int status;         // 0 = Borrowed, 1 = Available, 2 = Reserved
        string issuedTo;    // "-" if issued to no one
        string issueDate;   // "-" if not issued
        string reservedBy;  // "-" if not reserved

    public:
        Book(int id, string &title, string &author, string &publisher, int year, string &isbn){
            this->id = id;
            this->title = title;
            this->author = author;
            this->publisher = publisher;
            this->year = year;
            this->isbn = isbn;
            status = 1;
            issuedTo = "-";
            issueDate = "-";
            reservedBy = "-";
        }

        void setID(int id){ this->id = id; }
        void setTitle(string title){ this->title = title; }
        void setAuthor(string author){ this->author = author; }
        void setPublisher(string publisher){ this->publisher = publisher; }
        void setYear(int year){ this->year = year; }
        void setISBN(string isbn){ this->isbn = isbn; }
        void setStatus(int status){ this->status = status; }
        void setOwner(string owner){ issuedTo = owner; }
        void setIssueDate(string date){ issueDate = date; }
        void setReserver(string user){ reservedBy = user; }

        int getID(){ return id; }
        string getTitle(){ return title; }
        string getAuthor(){ return author; }
        string getPublisher(){ return publisher; }
        int getYear(){ return year; }
        string getISBN(){ return isbn; }
        int getStatus(){ return status; }
        string getOwner(){ return issuedTo; }
        string getIssueDate(){ return issueDate; }
        string getReserver(){ return reservedBy; }

        void viewBookRestrict(){
            string s;
            switch(status){
                case 0: s = "Borrowed"; break;
                case 1: s = "Available"; break;
                case 2: s = "Reserved"; break;
            }
            cout << "ID: " << id << " | Title: " << title << " | Author: " << author << " | Publisher: " << publisher << " | Year: " << year << " | Status: " << s << endl;
        }

        void viewBook(){
            string s;
            switch(status){
                case 0: s = "Borrowed"; break;
                case 1: s = "Available"; break;
                case 2: s = "Reserved"; break;
            }
            cout << "ID: " << id << " | Title: " << title << " | Author: " << author << " | Publisher: " << publisher << " | Year: " 
            << year << " | Status: " << s << " | Issued To: " << issuedTo << " | Issue Date: " << issueDate << " | Reserved By: " << reservedBy << endl;
        }
};
vector<Book*> listOfBooks;

class Account{
    private:
        vector< pair<Book*,string> > borrowedBooks;
        int fine;

    public:
        Account(){ fine = 0; }

        void addFine(int fine){ this->fine += fine; }
        void clearFine(){ fine = 0; }

        vector< pair<Book*,string> >& getBorrowedBooks(){ return borrowedBooks; }
        double getFine(){ return fine; }

        vector< pair<Book*,int> > getOverdueBooks(vector< pair<Book*, string> > borrowedBooks, string today){
            vector< pair<Book*,int> > overdue;
            for(auto book : borrowedBooks){
                int days = noOfDays(book.second, today);
                if(days>0) overdue.push_back({book.first,days});
            }
            return overdue;
        }
        
        void viewBorrowedBooks(){
            for(auto book : borrowedBooks) book.first->viewBook();
        }

        void viewOverdueBooks(string today){
            for(auto book : getOverdueBooks(borrowedBooks, today)) cout << " - " << book.first->getTitle() << " | Overdue by " << book.second << " days." << endl;
        }

        void viewFine(){
            cout << "Fine: " << fine << endl;
        }

        void viewAccount(){
            cout << "Borrowed Books: " << endl;
            for (auto book : borrowedBooks) cout << " - " << book.first->getTitle() << endl;
            cout << endl;
            viewFine();
        }
};

class Library;
class User{
    protected:
        int id;
        int type;
        string username;
        string password;
        Account account;

    public:
        User(int id, int type, string username, string password){
            this->id = id;
            this->type = type;
            this->username = username;
            this->password = password;
        }

        void setID(int id){ this->id = id; }
        void setType(int type){ this->type = type; }
        void setUsername(string username){ this->username = username; }
        void setPassword(string password){ this->password = password; }

        int getID(){ return id; }
        int getType(){ return type; }
        string getUsername(){ return username; }
        string getPassword(){ return password; }
        Account& getAccount(){ return account; }

        virtual void borrowBook(Book* book, string date) = 0;
        virtual void returnBook(Book* book, string date) = 0;
        virtual void cancelReservation(Book* book) = 0;
        virtual void reserveBook(Book* book) = 0;
        virtual void interface(Library &lib) = 0;

        void viewUserDetails(){
            string s;
            switch(type){
                case 1: s = "Student"; break;
                case 2: s = "Faculty"; break;
                case 3: s = "Librarian"; break;
            }
            cout << "ID: " << id << " | Username: " << username << " | Type: " << s << endl;
        }
        
        void viewAccount(){
            viewUserDetails();
            cout << endl;
            account.viewAccount();
        }
};
vector<User*> listOfUsers;

class Student : public User{
    private:
        int maxBooks = 3;
        int maxDays = 15;
        int finePerDay = 10;

    public:
        Student(int id, int type, string username, string password) : User(id, 1, username, password){}
        
        int getMaxDays(){ return maxDays; }

        void borrowBook(Book* book, string date){
            if(book->getStatus()!=1){
                if(book->getStatus()==2 && book->getReserver()==username) book->setReserver("-");
                else if(book->getStatus()==2){
                    cout << BOLD_RED << "ERROR: This book is reserved by another user." << RESET << endl;
                    return;
                }
                else{
                    cout << BOLD_RED << "ERROR: This book is borrowed." << RESET << endl;
                    cout << "Do you want to reserve this book? (Y/N): ";
                    char c; cin >> c; cin.ignore();
                    if(c=='Y'||c=='y') reserveBook(book);
                    return;
                }
            }
        
            string dueDate = calculateDueDate(date, maxDays);
            account.getBorrowedBooks().push_back({book,dueDate});
            book->setStatus(0);
            book->setOwner(username);
            book->setIssueDate(date);
        
            cout << GREEN << "SUCCESS: Student " << username << " has borrowed \"" << book->getTitle() << "\" on " << date << "." << RESET << endl;
        }

        void returnBook(Book* book, string date){
            if(!checkDate(date)){
                cout << BOLD_RED "ERROR: Invalid return date." << RESET << endl;
                return;
            }

            auto it = account.getBorrowedBooks().begin();
            for (; it != account.getBorrowedBooks().end(); it++){
                if(it->first==book){
                    int daysBorrowed = noOfDays(book->getIssueDate(), date);
                    if(daysBorrowed<0){
                        cout << BOLD_RED << "ERROR: Invalid return date." << RESET << endl;
                        return;
                    }
                    else if(daysBorrowed>maxDays){
                        int overdue = daysBorrowed - maxDays;
                        int fine = overdue * finePerDay;
                        account.addFine(fine);

                        cout << GREEN << "SUCCESS: Student " << username << " has returned \"" << book->getTitle() << "\" with " << daysBorrowed << " overdue days. Fine added: Rs " << fine << "." << RESET << endl;
                    }
                    else cout << GREEN << "SUCCESS: Student " << username << " has returned \"" << book->getTitle() << "\" on time." << RESET << endl;

                    if(book->getReserver() != "-") book->setStatus(2);
                    else book->setStatus(1);

                    book->setOwner("-");
                    book->setIssueDate("-");
                    account.getBorrowedBooks().erase(it);
                    return;
                }
            }

            cout << BOLD_RED << "ERROR: You haven't borrowed this book." << RESET << endl;
        }

        void cancelReservation(Book* book){
            if(book->getReserver()!=username){
                cout << BOLD_RED << "ERROR: You have not reserved this book." << RESET << endl;
                return;
            }
            book->setStatus(1);
            book->setReserver("-");
            cout << GREEN << "SUCCESS: Student " << username << " has canceled the reservation for \"" << book->getTitle() << "\"." << RESET << endl;
        }

        void clearFine(){
            account.clearFine();
            cout << GREEN << "SUCCESS: Student " << username << " has cleared their fine." << RESET << endl;
        }

        void reserveBook(Book* book){
            if(book->getReserver()!="-"){
                if(book->getReserver()==username) cout << BOLD_RED << "ERROR: You have already reserved this book." << RESET << endl;
                else cout << BOLD_RED << "ERROR: This book is already reserved by another user." << RESET << endl;
                return;
            }

            book->setStatus(2);
            book->setReserver(username);
            cout << GREEN << "SUCCESS: Student " << username << " has reserved \"" << book->getTitle() << "\"." << RESET << endl;
        }

        void interface(Library &lib);
};
vector<Student*> listOfStudents;

class Faculty : public User{
    private:
        int maxBooks = 5;
        int maxDays = 30;
        
    public:
        Faculty(int id, int type, string username, string password) : User(id, 2, username, password){}

        int getMaxDays(){ return maxDays; }

        void borrowBook(Book* book, string date){
            if(book->getStatus()!=1){
                if(book->getStatus()==2 && book->getReserver()==username) book->setReserver("-");
                else if(book->getStatus()==2){
                    cout << BOLD_RED << "ERROR: This book is reserved by another user." << RESET << endl;
                    return;
                }
                else{
                    cout << BOLD_RED << "ERROR: This book is borrowed." << RESET << endl;
                    cout << "Do you want to reserve this book? (Y/N): ";
                    char c; cin >> c; cin.ignore();
                    if(c=='Y'||c=='y') reserveBook(book);
                    return;
                }
            }
        
            for(auto b : account.getOverdueBooks(account.getBorrowedBooks(), date)){
                if(b.second > 60){
                    cout << BOLD_RED << "ERROR: You have an overdue of " << b.second << " days. You cannot borrow new books until resolved." << RESET << endl;
                    return;
                }
            }
        
            string dueDate = calculateDueDate(date, maxDays);
            account.getBorrowedBooks().push_back({book,dueDate});
            book->setStatus(0);
            book->setOwner(username);
            book->setIssueDate(date);
        
            cout << GREEN << "SUCCESS: Faculty " << username << " has borrowed \"" << book->getTitle() << "\" on " << date << "." << RESET << endl;
        }

        void returnBook(Book* book, string date){
            if(!checkDate(date)){
                cout << BOLD_RED "ERROR: Invalid return date." << RESET << endl;
                return;
            }

            auto it = account.getBorrowedBooks().begin();
            for (; it != account.getBorrowedBooks().end(); it++){
                if(it->first==book){
                    int daysBorrowed = noOfDays(book->getIssueDate(), date);
                    if(daysBorrowed<0){
                        cout << BOLD_RED << "ERROR: Invalid return date." << RESET << endl;
                        return;
                    }
                    else if(daysBorrowed>maxDays){
                        int overdue = daysBorrowed - maxDays;
                        cout << GREEN << "SUCCESS: Faculty " << username << " has returned \"" << book->getTitle() << "\" with overdue of" << overdue << " days." << RESET << endl;
                    }
                    else cout << GREEN << "SUCCESS: Faculty " << username << " has returned \"" << book->getTitle() << "\" on time." << RESET << endl;

                    if(book->getReserver() != "-") book->setStatus(2);
                    else book->setStatus(1);

                    book->setOwner("-");
                    book->setIssueDate("-");
                    account.getBorrowedBooks().erase(it);
                    return;
                }
            }

            cout << BOLD_RED << "ERROR: You haven't borrowed this book." << RESET << endl;
        }

        void cancelReservation(Book* book){
            if(book->getReserver()!=username){
                cout << BOLD_RED << "ERROR: You have not reserved this book." << RESET << endl;
                return;
            }
            book->setStatus(1);
            book->setReserver("-");
            cout << GREEN << "SUCCESS: Faculty " << username << " has canceled the reservation for \"" << book->getTitle() << "\"." << RESET << endl;
        }

        void reserveBook(Book* book){
            if(book->getReserver()!="-"){
                if(book->getReserver()==username){
                    cout << BOLD_RED << "ERROR: You have already reserved this book." << RESET << endl;
                    return;
                }
                else{
                    cout << BOLD_RED << "ERROR: This book is already reserved by another user." << RESET << endl;
                    return;
                }
            }

            book->setStatus(2);
            book->setReserver(username);
            cout << GREEN << "SUCCESS: Faculty " << username << " has reserved \"" << book->getTitle() << "\"." << RESET << endl;
        }

        void interface(Library &lib);  
};
vector<Faculty*> listOfFaculty;

class Librarian : public User{
    public:
        Librarian(int id, int type, string username, string password) : User(id, 3, username, password){}

        void borrowBook(Book* book, string date){ cout << BOLD_RED << "ERROR: You cannot borrow books." << RESET << endl; }
        void returnBook(Book* book, string date){ cout << BOLD_RED << "ERROR: You cannot return books." << RESET << endl; }
        void reserveBook(Book* book){ cout << BOLD_RED << "ERROR: You cannot make or cancel reservations." << RESET << endl; }
        void cancelReservation(Book* book){ cout << BOLD_RED << "ERROR: You cannot make or cancel reservations." << RESET << endl; }

        void addBook(Book* book){
            listOfBooks.push_back(book);
            cout << GREEN << "SUCCESS: Librarian " << username << " added book: \"" << book->getTitle() << "\"." << RESET << endl;
        }

        void removeBook(Book* book){
            if(book->getStatus()==1 || book->getStatus()==2){
                for(auto it = listOfBooks.begin(); it != listOfBooks.end(); it++){
                    if(*it == book){
                        cout << GREEN << "SUCCESS: Librarian " << username << " removed book: \"" << book->getTitle() << "\"." << RESET << endl;
                        listOfBooks.erase(it);
                        break;
                    }
                }
            }
            else cout << BOLD_RED << "ERROR: You cannot remove this book: \"" << book->getTitle() << "\". Book is borrowed." << RESET << endl;
        }
        
        void updateBook(Book* book, string title, string author, string publisher, int year, string isbn){
            book->setTitle(title);
            book->setAuthor(author);
            book->setPublisher(publisher);
            book->setYear(year);
            book->setISBN(isbn);
            cout << GREEN << "SUCCESS: Librarian " << username << " updated book: \"" << book->getTitle() << "\"." << RESET << endl;
        }

        void addUser(User* user);

        void removeUser(User* user);

        void updateUser(User* user, string username, string password){
            user->setUsername(username);
            user->setPassword(password);
            cout << GREEN << "SUCCESS: Librarian " << username << " updated user: " << user->getUsername() << "." << RESET << endl;
        }

        void interface(Library &lib);
};
vector<Librarian*> listOfLibrarians;

class Library{
    public:
        Book* findBook(int id){
            Book* book = nullptr;
            for(auto b : listOfBooks){
                if(b->getID() == id) book = b;
            }
            return book;
        }

        void viewBooksRestrict(){
            cout << "Library Books:" << endl;
            for(auto b : listOfBooks) b->viewBookRestrict();
        }
        void viewBooks(){
            cout << "Library Books:" << endl;
            for(auto b : listOfBooks) b->viewBook();
        }
        
        void viewAvailableBooksRestrict(){
            cout << "Available Books:" << endl;
            for(auto b : listOfBooks){
                if(b->getStatus() == 1) b->viewBookRestrict();
            }
        }
        void viewAvailableBooks(){
            cout << "Available Books:" << endl;
            for(auto b : listOfBooks){
                if(b->getStatus() == 1) b->viewBook();
            }
        }

        void viewBorrowedBooks(){
            cout << "Borrowed Books:" << endl;
            for(auto b : listOfBooks){
                if(b->getStatus() == 0) b->viewBook();
            }
        }

        void viewReservedBooks(){
            cout << "Reserved Books:" << endl;
            for(auto b : listOfBooks){
                if(b->getReserver() != "-") b->viewBook();
            }
        }

        void viewOverdueBooks(string date){
            cout << "Overdue Books:" << endl;
            for(auto b : listOfBooks){
                if(b->getStatus() == 0){
                    int days = noOfDays(b->getIssueDate(), date);
                    int maxDays = 0;
                    for(auto user : listOfUsers){
                        if(user->getUsername() == b->getOwner()){
                            if(dynamic_cast<Student*>(user)) maxDays = 15;
                            else if(dynamic_cast<Faculty*>(user)) maxDays = 30;
                            break;
                        }
                    }
                    if(days>maxDays){
                        cout << " - " << b->getTitle() << " | Issued To: " << b->getOwner() << " | Overdue by " << days-maxDays << " days." << endl;
                    }
                }
            }
        }

        void checkBookAvailability(Book* book){
            string word;
            if(book){
                if(book->getStatus() == 1) cout << GREEN << "This book is available." << RESET << endl;
                else if(book->getStatus() == 0) cout << GREEN << "This book is borrowed." << RESET << endl;
                else cout << GREEN << "This book is reserved." << RESET << endl;
            }
        }

        User* findUser(int id){
            User* user = nullptr;
            for(auto u : listOfUsers){
                if(u->getID() == id) user = u;
            }
            return user;
        }

        void viewUsers(){
            cout << "Library Users:" << endl;
            for(auto u : listOfUsers) u->viewUserDetails();
        }

        void viewStudents(){
            cout << "Students:" << endl;
            for(auto s : listOfStudents) s->viewUserDetails();
        }

        void viewFaculty(){
            cout << "Faculty:" << endl;
            for(auto f : listOfFaculty) f->viewUserDetails();
        }

        void viewLibrarians(){
            cout << "Librarians:" << endl;
            for(auto l : listOfLibrarians) l->viewUserDetails();
        }
};

void Student:: interface(Library &lib){
    int action;
    while(true){
        cout << BOLD_YELLOW << "=========================================" << RESET << endl;
        cout << BOLD_YELLOW << "           STUDENT DASHBOARD             " << RESET << endl;
        cout << BOLD_YELLOW << "=========================================" << RESET << endl;
    
        cout << LIGHT_BLUE << "  1.  View all books" << RESET << endl;
        cout << LIGHT_BLUE << "  2.  View available books" << RESET << endl;
        cout << LIGHT_BLUE << "  3.  Check the availability of a book" << RESET << endl;
        cout << LIGHT_BLUE << "  4.  View book details" << RESET << endl;
        cout << LIGHT_BLUE << "  5.  View my borrowed books" << RESET << endl;
        cout << LIGHT_BLUE << "  6.  View my overdue books" << RESET << endl;
        cout << LIGHT_BLUE << "  7.  Borrow a book" << RESET << endl;
        cout << LIGHT_BLUE << "  8.  Return a book" << RESET << endl;
        cout << LIGHT_BLUE << "  9.  Cancel reservation" << RESET << endl;
        cout << LIGHT_BLUE << " 10.  View fine amount" << RESET << endl;
        cout << LIGHT_BLUE << " 11.  Clear fine amount" << RESET << endl;
        cout << LIGHT_BLUE << " 12.  View my account" << RESET << endl;
        cout << LIGHT_BLUE << " 13.  Logout" << RESET << endl;
    
        cout << BOLD_YELLOW << "=========================================" << RESET << endl;
        cout << BOLD_WHITE << " Enter the action you would like to perform: " << RESET;
    
        cin >> action;
        cin.ignore();

        system("cls");
        
        if(action==1){
            cout << BOLD_WHITE << "VIEW ALL BOOKS\n\n" << RESET;
            lib.viewBooksRestrict();
        }
        else if(action==2){
            cout << BOLD_WHITE << "VIEW AVAILABLE BOOKS\n\n" << RESET;
            lib.viewAvailableBooksRestrict();
        }
        else if(action==3){
            cout << BOLD_WHITE << "CHECK THE AVAILABILITY OF A BOOK\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
            if(book) lib.checkBookAvailability(book);
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==4){
            cout << BOLD_WHITE << "VIEW BOOK DETAILS\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
            if(book) book->viewBookRestrict();
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==5){
            cout << BOLD_WHITE << "VIEW MY BORROWED BOOKS\n\n" << RESET;
            account.viewBorrowedBooks();
        }
        else if(action==6){
            cout << BOLD_WHITE << "VIEW MY OVERDUE BOOKS\n\n" << RESET;
            cout << "Enter today's date (DD/MM/YYYY): ";
            string date; cin >> date; cin.ignore();
            if(checkDate(date)) account.viewOverdueBooks(date);
            else cout << BOLD_RED << "ERROR: Invalid date." << RESET << endl;
        }
        else if(action==7){
            cout << BOLD_WHITE << "BORROW A BOOK\n\n" << RESET;
            if(account.getBorrowedBooks().size()>=maxBooks){
                cout << BOLD_RED << "ERROR: You cannot borrow more than " << maxBooks << " books." << RESET << endl;
                continue;
            }
            if(account.getFine()!=0){
                cout << BOLD_RED << "ERROR: You have a fine of Rs " << account.getFine() << ". Clear the fine to borrow books." << RESET << endl;
                continue;
            }
            
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);

            if(book){
                cout << "Enter date of issue (DD/MM/YYYY):";
                string date; cin >> date; cin.ignore();
                if(checkDate(date)) borrowBook(book, date);
                else cout << BOLD_RED << "ERROR: Invalid date." << RESET << endl;
            }
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==8){
            cout << BOLD_WHITE << "RETURN A BOOK\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
 
            if(book && book->getOwner()==this->getUsername()){
                cout << "Enter the date of return (DD/MM/YYYY): ";
                string date; cin >> date; cin.ignore();
                if(checkDate(date) && noOfDays(book->getIssueDate(), date)>=0) returnBook(book, date);
                else cout << BOLD_RED << "ERROR: Invalid date." << RESET << endl;
            }
            else if(book) cout << BOLD_RED << "ERROR: You have not borrowed this book." << RESET << endl;
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==9){
            cout << BOLD_WHITE << "CANCEL RESERVATION\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
            if(book) cancelReservation(book);
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==10){
            cout << BOLD_WHITE << "VIEW FINE AMOUNT\n\n" << RESET;
            cout << "Fine: " << account.getFine() << endl;
        }
        else if(action==11){
            cout << BOLD_WHITE << "CLEAR FINE AMOUNT\n\n" << RESET;
            clearFine();
        }
        else if(action==12){
            cout << BOLD_WHITE << "DISPLAY ACCOUNT\n\n" << RESET;
            viewAccount();
        }
        else if(action==13) break;
        else cout << BOLD_RED << "ERROR: Invalid Action. Try again." << RESET << endl;
        cout << endl;
    }
}

void Faculty:: interface(Library &lib){
    int action;
    while(true){
        cout << BOLD_YELLOW << "=========================================" << RESET << endl;
        cout << BOLD_YELLOW << "           FACULTY DASHBOARD             " << RESET << endl;
        cout << BOLD_YELLOW << "=========================================" << RESET << endl;

        cout << LIGHT_BLUE << "  1.  View all books" << RESET << endl;
        cout << LIGHT_BLUE << "  2.  View available books" << RESET << endl;
        cout << LIGHT_BLUE << "  3.  Check the availability of a book" << RESET << endl;
        cout << LIGHT_BLUE << "  4.  View book details" << RESET << endl;
        cout << LIGHT_BLUE << "  5.  View my borrowed books" << RESET << endl;
        cout << LIGHT_BLUE << "  6.  View my overdue books" << RESET << endl;
        cout << LIGHT_BLUE << "  7.  Borrow a book" << RESET << endl;
        cout << LIGHT_BLUE << "  8.  Return a book" << RESET << endl;
        cout << LIGHT_BLUE << "  9.  Cancel reservation" << RESET << endl;
        cout << LIGHT_BLUE << " 10.  View my account" << RESET << endl;
        cout << LIGHT_BLUE << " 11.  Logout" << RESET << endl;

        cout << BOLD_YELLOW << "=========================================" << RESET << endl;
        cout << BOLD_WHITE << " Enter the action you would like to perform: " << RESET;
    
        cin >> action;
        cin.ignore();

        system("cls");

        if(action==1){
            cout << BOLD_WHITE << "VIEW ALL BOOKS\n\n" << RESET;
            lib.viewBooksRestrict();
        }
        else if(action==2){
            cout << BOLD_WHITE << "VIEW AVAILABLE BOOKS\n\n" << RESET;
            lib.viewAvailableBooksRestrict();
        }
        else if(action==3){
            cout << BOLD_WHITE << "CHECK THE AVAILABILITY OF A BOOK\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
            if(book) lib.checkBookAvailability(book);
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==4){
            cout << BOLD_WHITE << "VIEW BOOK DETAILS\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
            if(book) book->viewBookRestrict();
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==5){
            cout << BOLD_WHITE << "VIEW MY BORROWED BOOKS\n\n" << RESET;
            account.viewBorrowedBooks();
        }
        else if(action==6){
            cout << BOLD_WHITE << "VIEW MY OVERDUE BOOKS\n\n" << RESET;
            cout << "Enter today's date (DD/MM/YYYY): ";
            string date; cin >> date; cin.ignore();
            if(checkDate(date)) account.viewOverdueBooks(date);
            else cout << BOLD_RED << "ERROR: Invalid date." << RESET << endl;
        }
        else if(action==7){
            cout << BOLD_WHITE << "BORROW A BOOK\n\n" << RESET;
            if(account.getBorrowedBooks().size()>=maxBooks){
                cout << BOLD_RED << "ERROR: You cannot borrow more than " << maxBooks << " books." << RESET << endl;
                continue;
            }
            
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);

            if(book){
                cout << "Enter date of issue (DD/MM/YYYY):";
                string date; cin >> date; cin.ignore();
                if(checkDate(date)) borrowBook(book, date);
                else cout << BOLD_RED << "ERROR: Invalid date." << RESET << endl;
            }
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==8){
            cout << BOLD_WHITE << "RETURN A BOOK\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
 
            if(book && book->getOwner()==this->getUsername()){
                cout << "Enter the date of return (DD/MM/YYYY): ";
                string date; cin >> date; cin.ignore();
                if(checkDate(date) && noOfDays(book->getIssueDate(), date)>=0) returnBook(book, date);
                else cout << BOLD_RED << "ERROR: Invalid date." << RESET << endl;
            }
            else if(book) cout << BOLD_RED << "ERROR: You have not borrowed this book." << RESET << endl;
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==9){
            cout << BOLD_WHITE << "CANCEL RESERVATION\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
            if(book) cancelReservation(book);
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==10){
            cout << BOLD_WHITE << "DISPLAY ACCOUNT\n\n" << RESET;
            viewAccount();
        }
        else if(action==11) break;
        else cout << BOLD_RED << "ERROR: Invalid Action. Try again." << RESET << endl;
        cout << endl;
    }
}

void Librarian:: addUser(User* user){
    listOfUsers.push_back(user);
    string s;

    if(user->getType()==1){
        listOfStudents.push_back((Student*)user);
        s = "Student";
    }
    else if(user->getType()==2){
        listOfFaculty.push_back((Faculty*)user);
        s = "Faculty";
    }
    else if(user->getType()==3){
        listOfLibrarians.push_back((Librarian*)user);
        s = "Librarian";
    }

    cout << GREEN << "SUCCESS: Librarian " << username << " added user: " << user->getUsername() << " (" << s << ")." << RESET << endl;
}

void Librarian:: removeUser(User* user){
    if(user->getAccount().getBorrowedBooks().size()){
        cout << BOLD_RED << "ERROR: User " << user->getUsername() << " has borrowed books. Cannot remove user." << RESET << endl;
        return;
    }
    if(user->getAccount().getFine()){
        cout << BOLD_RED << "ERROR: User " << user->getUsername() << " has a fine of Rs " << user->getAccount().getFine() << ". Clear the fine to remove user." << RESET << endl;
        return;
    }

    for(auto& book : listOfBooks){
        if(book->getReserver() == user->getUsername()){
            book->setReserver("-");
            if(book->getStatus()==2) book->setStatus(1);
        }
    }

    if(user->getType()==1){
        for(auto it = listOfStudents.begin(); it != listOfStudents.end(); it++){
            if(*it == user){
                listOfStudents.erase(it);
                break;
            }
        }
    }
    else if(user->getType()==2){
        for(auto it = listOfFaculty.begin(); it != listOfFaculty.end(); it++){
            if(*it == user){
                listOfFaculty.erase(it);
                break;
            }
        }
    }
    else if(user->getType()==3){
        for(auto it = listOfLibrarians.begin(); it != listOfLibrarians.end(); it++){
            if(*it == user){
                listOfLibrarians.erase(it);
                break;
            }
        }
    }

    for(auto it = listOfUsers.begin(); it != listOfUsers.end(); it++){
        if(*it == user){
            cout << GREEN << "SUCCESS: Librarian " << username << " removed user: " << user->getUsername() << "." << RESET << endl;
            listOfUsers.erase(it);
            break;
        }
    }
}

void Librarian:: interface(Library &lib){
    int action;
    while(true){
        cout << BOLD_YELLOW << "===================================================================" << RESET << endl;
        cout << BOLD_YELLOW << "                        LIBRARIAN DASHBOARD                        " << RESET << endl;
        cout << BOLD_YELLOW << "===================================================================" << RESET << endl;

        cout << LIGHT_BLUE << left << setw(40) << " 1.  View all books"           << "11.  View all users" << RESET << endl;
        cout << LIGHT_BLUE << left << setw(40) << " 2.  View available books"     << "12.  View students" << RESET << endl;
        cout << LIGHT_BLUE << left << setw(40) << " 3.  View borrowed books"      << "13.  View faculty" << RESET << endl;
        cout << LIGHT_BLUE << left << setw(40) << " 4.  View reserved books"      << "14.  View librarians" << RESET << endl;
        cout << LIGHT_BLUE << left << setw(40) << " 5.  View overdue books"       << "15.  Add a user" << RESET << endl;
        cout << LIGHT_BLUE << left << setw(40) << " 6.  Check book availability"  << "16.  Remove a user" << RESET << endl;
        cout << LIGHT_BLUE << left << setw(40) << " 7.  View book details"        << "17.  Update a user" << RESET << endl;
        cout << LIGHT_BLUE << left << setw(40) << " 8.  Add a book"               << "18.  View user details" << RESET << endl;
        cout << LIGHT_BLUE << left << setw(40) << " 9.  Remove a book"            << "19.  View my account" << RESET << endl;
        cout << LIGHT_BLUE << left << setw(40) << "10.  Update a book"            << "20.  Logout" << RESET << endl;

        cout << BOLD_YELLOW << "===================================================================" << RESET << endl;
        cout << GREEN << " Enter the action you would like to perform: " << RESET;

        cin >> action;
        cin.ignore();

        system("cls");

        if(action==1){
            cout << BOLD_WHITE << "VIEW ALL BOOKS\n\n" << RESET;
            lib.viewBooks();
        }
        else if(action==2){
            cout << BOLD_WHITE << "VIEW AVAILABLE BOOKS\n\n" << RESET;
            lib.viewAvailableBooks();
        }
        else if(action==3){
            cout << BOLD_WHITE << "VIEW BORROWED BOOKS\n\n" << RESET;
            lib.viewBorrowedBooks();
        }
        else if(action==4){
            cout << BOLD_WHITE << "VIEW RESERVED BOOKS\n\n" << RESET;
            lib.viewReservedBooks();
        }
        else if(action==5){
            cout << BOLD_WHITE << "VIEW OVERDUE BOOKS\n\n" << RESET;
            cout << "Enter today's date (DD/MM/YYYY): ";
            string date; cin >> date; cin.ignore();
            if(checkDate(date)) lib.viewOverdueBooks(date);
            else cout << BOLD_RED << "ERROR: Invalid date." << RESET << endl;
        }
        else if(action==6){
            cout << BOLD_WHITE << "CHECK THE AVAILABILITY OF A BOOK\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
            if(book) lib.checkBookAvailability(book);
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==7){
            cout << BOLD_WHITE << "VIEW BOOK DETAILS\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
            if(book) book->viewBook();
            else cout << BOLD_RED << "ERROR: Book not found." << RESET << endl;
        }
        else if(action==8){
            cout << BOLD_WHITE << "ADD A BOOK\n\n" << RESET;
            cout << "Enter book title: "; string title; getline(cin >> ws, title);
            cout << "Enter book author: "; string author; getline(cin >> ws, author);
            cout << "Enter book publisher: "; string publisher; getline(cin >> ws, publisher);
            cout << "Enter book year: "; int year; cin >> year; cin.ignore();
            cout << "Enter book ISBN: "; string isbn; cin >> isbn; cin.ignore();
            Book* book = new Book(listOfBooks.size()+1, title, author, publisher, year, isbn);
            addBook(book);
        }
        else if(action==9){
            cout << BOLD_WHITE << "REMOVE A BOOK\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
            if(book) removeBook(book);
            else cout << "Book not found." << endl;
        }
        else if(action==10){
            cout << BOLD_WHITE << "UPDATE A BOOK\n\n" << RESET;
            cout << "Enter book ID: ";
            int id; cin >> id;
            Book* book = lib.findBook(id);
            if(book){
                book->viewBook();
                cout << "What would you like to update?" << endl;
                cout << "1. Title" << endl;
                cout << "2. Author" << endl;
                cout << "3. Publisher" << endl;
                cout << "4. Year" << endl;
                cout << "5. ISBN" << endl;
                int choice; cin >> choice; cin.ignore();
                if(choice==1){
                    cout << "Enter new title: "; string newTitle; getline(cin >> ws, newTitle);
                    updateBook(book, newTitle, book->getAuthor(), book->getPublisher(), book->getYear(), book->getISBN());
                }
                else if(choice==2){
                    cout << "Enter new author: "; string newAuthor; getline(cin >> ws, newAuthor);
                    updateBook(book, book->getTitle(), newAuthor, book->getPublisher(), book->getYear(), book->getISBN());
                }
                else if(choice==3){
                    cout << "Enter new publisher: "; string newPublisher; getline(cin >> ws, newPublisher);
                    updateBook(book, book->getTitle(), book->getAuthor(), newPublisher, book->getYear(), book->getISBN());
                }
                else if(choice==4){
                    cout << "Enter new year: "; int newYear; cin >> newYear;
                    updateBook(book, book->getTitle(), book->getAuthor(), book->getPublisher(), newYear, book->getISBN());
                }
                else if(choice==5){
                    cout << "Enter new ISBN: "; string newISBN; cin >> newISBN;
                    updateBook(book, book->getTitle(), book->getAuthor(), book->getPublisher(), book->getYear(), newISBN);
                }
                else cout << "Invalid choice." << endl;
            }
            else cout << "Book not found." << endl;
        }
        else if(action==11){
            cout << BOLD_WHITE << "VIEW USERS\n\n" << RESET;
            lib.viewUsers();
        }
        else if(action==12){
            cout << BOLD_WHITE << "VIEW STUDENTS\n\n" << RESET;
            lib.viewStudents();
        }
        else if(action==13){
            cout << BOLD_WHITE << "VIEW FACULTY\n\n" << RESET;
            lib.viewFaculty();
        }
        else if(action==14){
            cout << BOLD_WHITE << "VIEW LIBRARIANS\n\n" << RESET;
            lib.viewLibrarians();
        }
        else if(action==15){
            cout << BOLD_WHITE << "ADD A USER\n\n" << RESET;
            cout << "Enter user type (1 = Student, 2 = Faculty, 3 = Librarian): "; int type; cin >> type;
            if(type<1 || type>3){ cout << "Invalid user type." << endl; continue; }
            cout << "Enter username: "; string username; getline(cin >> ws, username);
            cout << "Enter password: "; string password; cin >> password; cin.ignore();

            User* user;
            if(type==1){
                user = new Student((*listOfUsers.rbegin())->getID()+1, type, username, password);
                listOfStudents.push_back((Student*)user);
            }
            else if(type==2){
                user = new Faculty((*listOfUsers.rbegin())->getID()+1, type, username, password);
                listOfFaculty.push_back((Faculty*)user);
            }
            else{
                user = new Librarian((*listOfUsers.rbegin())->getID()+1, type, username, password);
                listOfLibrarians.push_back((Librarian*)user);
            }
            listOfUsers.push_back(user);

            cout << GREEN << "SUCCESS: User " << user->getUsername() << " added." << RESET << endl;
        }
        else if(action==16){
            cout << BOLD_WHITE << "REMOVE USER\n\n" << RESET;
            cout << "Enter user ID: "; int id; cin >> id; cin.ignore();
            User* user = lib.findUser(id);
            if(user) removeUser(user);
            else cout << BOLD_RED << "ERROR: User not found." << RESET << endl;
        }
        else if(action==17){
            cout << BOLD_WHITE << "UPDATE USER\n\n" << RESET;
            cout << "Enter user ID: "; int id; cin >> id; cin.ignore();
            User* user = lib.findUser(id);
            if(user){
                user->viewUserDetails();
                cout << "What would you like to update?" << endl;
                cout << "1. Username" << endl;
                cout << "2. Password" << endl;
                int choice; cin >> choice;
                cin.ignore();
                if(choice==1){
                    cout << "Enter new username: "; string newUsername; cin >> newUsername;
                    cin.ignore();
                    updateUser(user, newUsername, user->getPassword());
                }
                else if(choice==2){
                    cout << "Enter new password: "; string newPassword; cin >> newPassword;
                    cin.ignore();
                    updateUser(user, user->getUsername(), newPassword);
                }
                else cout << BOLD_RED << "ERROR: Invalid choice." << RESET << endl;
            }
            else cout << BOLD_RED << "ERROR: User not found." << RESET << endl;  
        }
        else if(action==18){
            cout << BOLD_WHITE << "VIEW USER DETAILS\n\n" << RESET;
            cout << "Enter user ID: "; int id; cin >> id; cin.ignore();
            User* user = lib.findUser(id);
            if(user) user->viewUserDetails();
            else cout << BOLD_RED << "ERROR: User not found." << RESET << endl;
        }
        else if(action==19){
            cout << BOLD_WHITE << "VIEW MY ACCOUNT\n\n" << RESET;
            viewUserDetails();
        }
        else if(action==20) break;
        else cout << BOLD_RED << "ERROR: Invalid Action. Try again." << RESET << endl;
        cout << endl;
    }
}

void getDatabaseData(){
    string line, word;
    unordered_map<string, User*> userMap;

    fstream bookfile("book.csv", ios::in);
    //ID, Title, Author, Publisher, Year, ISBN, Status, IssuedTo, IssueDate, ReservedBy

    if(bookfile.is_open()){
        getline(bookfile, line);    // Skip the heading row

        while(getline(bookfile, line)){
            if(line.empty()) continue; // Skip empty lines

            vector<string> bookdata;
            stringstream s(line);
            while(getline(s, word, ',')) bookdata.push_back(word);

            if(bookdata.size()!=10){
                cerr << "Error: Invalid book data format: " << line << endl;
                continue;
            }

            try{
                int id = stoi(bookdata[0]);
                string title = bookdata[1];
                string author = bookdata[2];
                string publisher = bookdata[3];
                int year = stoi(bookdata[4]);
                string isbn = bookdata[5];
                int status = stoi(bookdata[6]);
                string issuedTo = bookdata[7];
                string issueDate = bookdata[8];
                string reservedBy = bookdata[9];

                Book* book = new Book(id, title, author, publisher, year, isbn);
                book->setStatus(status);
                book->setOwner(issuedTo);
                book->setIssueDate(issueDate);
                book->setReserver(reservedBy);
                listOfBooks.push_back(book);
            }catch(const invalid_argument& e){
                cerr << "Error: Invalid book data: " << line << " - " << e.what() << endl;
            }
        }
        bookfile.close();
    }
    else cerr << "Error: book.csv not found!" << endl;

    fstream userfile("user.csv", ios::in);
    // ID, Type, Username, Password, BorrowedBooks, Fine

    if(userfile.is_open()){
        getline(userfile, line);    // Skip the heading row

        while(getline(userfile, line)){
            if(line.empty()) continue; // Skip empty lines

            vector<string> userdata;
            stringstream s(line);
            while(getline(s, word, ',')) userdata.push_back(word);

            if(userdata.size()!=6){
                cerr << "Error: Invalid user data format: " << line << endl;
                continue;
            }

            try{
                int id = stoi(userdata[0]);
                int type = stoi(userdata[1]);
                string username = userdata[2];
                string password = userdata[3];
                string borrowedBooksStr = userdata[4];
                int fine = stoi(userdata[5]);

                borrowedBooksStr.erase(remove(borrowedBooksStr.begin(), borrowedBooksStr.end(), '\"'), borrowedBooksStr.end());

                User* user = nullptr;
                if(type==1){
                    Student* student = new Student(id, type, username, password);
                    student->getAccount().addFine(fine);
                    listOfStudents.push_back(student);
                    user = student;
                }
                else if(type==2){
                    Faculty* faculty = new Faculty(id, type, username, password);
                    listOfFaculty.push_back(faculty);
                    user = faculty;
                }
                else{
                    Librarian* librarian = new Librarian(id, type, username, password);
                    listOfLibrarians.push_back(librarian);
                    user = librarian;
                }

                listOfUsers.push_back(user);
                userMap[username] = user;

                if(borrowedBooksStr != "-"){
                    stringstream ss(borrowedBooksStr);
                    string bookID;
                    while(getline(ss, bookID, ';')){
                        int bookId = stoi(bookID);
                        for(auto& book : listOfBooks){
                            if(book->getID() == bookId){
                                string dueDate = calculateDueDate(book->getIssueDate(), user->getType() == 1 ? ((Student*)user)->getMaxDays() : ((Faculty*)user)->getMaxDays());
                                user->getAccount().getBorrowedBooks().push_back({book, dueDate});
                                break;
                            }
                        }
                    }
                }
            }catch(const invalid_argument& e){
                cerr << "Error: Invalid user data: " << line << " - " << e.what() << endl;
            }
        }
        userfile.close();
    }
    else cerr << "Error: user.csv not found!" << endl;
}

void updateDatabase(){
    remove("user.csv");
    remove("book.csv");
    fstream fout;

    fout.open("user.csv", ios::out);
    fout << "ID,Type,Username,Password,BorrowedBooks,Fine\n"; // Header line
    //ID, Type, Username, Password, BorrowedBooks, Fine

    for(auto& user : listOfUsers){
        fout << user->getID() << ",";
        fout << user->getType() << ",";
        fout << user->getUsername() << ",";
        fout << user->getPassword() << ",";

        vector< pair<Book*,string> > borrowedBooks = user->getAccount().getBorrowedBooks();
        
        ostringstream bookstream;
        for(int j=0; j<borrowedBooks.size(); j++){
            bookstream << borrowedBooks[j].first->getID();
            if(j != borrowedBooks.size() - 1) bookstream << ";";
        }

        fout << (borrowedBooks.empty() ? "-" : bookstream.str()) << ",";
        fout << user->getAccount().getFine() << "\n";
    }
    fout.close();
    
    fout.open("book.csv", ios::out);
    fout << "ID,Title,Author,Publisher,Year,ISBN,Status,IssuedTo,IssueDate,ReservedBy\n"; // Header line
    //ID, Title, Author, Publisher, Year, ISBN, Status, IssuedTo, IssueDate, ReservedBy

    for(auto& book : listOfBooks){
        fout << book->getID() << ",";
        fout << book->getTitle() << ",";
        fout << book->getAuthor() << ",";
        fout << book->getPublisher() << ",";
        fout << book->getYear() << ",";
        fout << book->getISBN() << ",";
        fout << book->getStatus() << ",";
        fout << book->getOwner() << ",";
        fout << book->getIssueDate() << ",";
        fout << book->getReserver() << "\n";
    }
    fout.close();
}

int main(){
    getDatabaseData();
    Library lib;
    
    string username, password;
    int end;
    
    while(true){
        cout << BOLD_WHITE "WELCOME TO PK KELKAR LIBRARY, IIT KANPUR" << RESET << endl;
        cout << "0. Exit" << endl;
        cout << "1. Login" << endl << endl;
        cout << BOLD_WHITE << "What would you like to do? " << RESET;
        cin >> end; cin.ignore();

        if(end==0||end==1) break;
        else{
            system("cls");
            cout << BOLD_RED << "ERROR: Invalid input. Please try again." << RESET << endl; 
        }
    }

    system("cls");

    while(end==1){
        cout << BOLD_YELLOW << "=========================================" << RESET << endl;
        cout << BOLD_YELLOW << "                  LOGIN                  " << RESET << endl;
        cout << BOLD_YELLOW << "=========================================" << RESET << endl;
        cout << "Enter username: "; cin >> username; cin.ignore();
        cout << "Enter password: "; cin >> password; cin.ignore();

        system("cls");
        
        User* currentUser = nullptr;
        for(auto user : listOfUsers){
            if(user->getUsername()==username && user->getPassword()==password){
                currentUser = user;
                break;
            }
        }
        
        if(!currentUser){
            cout << BOLD_RED << "ERROR: Invalid credentials." << RESET << endl;
            continue;
        }
        
        cout << GREEN << "Login successful!" << RESET << endl << endl;
        cout << GREEN << "Welcome, " << currentUser->getUsername() << "!" << RESET << endl << endl;

        if (Student* student = dynamic_cast<Student*>(currentUser)) student->interface(lib);
        else if (Faculty* faculty = dynamic_cast<Faculty*>(currentUser)) faculty->interface(lib);
        else if (Librarian* librarian = dynamic_cast<Librarian*>(currentUser)) librarian->interface(lib);

        while(true){
            cout << BOLD_WHITE "WELCOME TO PK KELKAR LIBRARY, IIT KANPUR" << RESET << endl;
            cout << "0. Exit" << endl;
            cout << "1. Login" << endl << endl;
            cout << BOLD_WHITE << "What would you like to do? " << RESET;
            cin >> end; cin.ignore();
    
            if(end==0||end==1) break;
            else{
                system("cls");
                cout << BOLD_RED << "ERROR: Invalid input. Please try again." << RESET << endl; 
            }
        }
    
        system("cls");
    }
    
    updateDatabase();
    cout << "Library data has been updated. Exiting system." << endl;
    return 0;
}