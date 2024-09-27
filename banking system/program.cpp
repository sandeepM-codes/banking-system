#include <iostream> // Input-output stream library
#include <fstream>  // File stream library
#include <cstdlib>
#include <vector> // Vector library
#include <map>    // Map data structure

using namespace std;

#define MIN_BALANCE 500 // Minimum balance required

class InsufficientFunds
{
};
class Account
{
private:
    long accountNumber;
    string FirstName;
    string LastName;
    float balance; // Account balance
    static long NextAccountNumber;

public:
    Account() {}
    Account(string fname, string lname, float balance);

    long getAccNo() { return accountNumber; }
    string getFirstName() { return FirstName; }
    string getLastName() { return LastName; }
    float getBalance() { return balance; }

    // Deposit money 
    void Deposit(float amount);

    // Withdraw money 
    void Withdraw(float amount);

    static void setLastAccountNumber(long accountNumber);
    static long getLastAccountNumber();

    friend ofstream &operator<<(ofstream &ofs, Account &acc);
    friend ifstream &operator>>(ifstream &ifs, Account &acc);
    friend ostream &operator<<(ostream &os, Account &acc);
};

long Account::NextAccountNumber = 0;

Account::Account(string fname, string lname, float balance)
{
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
    FirstName = fname;
    LastName = lname;
    this->balance = balance;
}

// Deposit function
void Account::Deposit(float amount)
{
    balance += amount;
}

// Withdraw function
void Account::Withdraw(float amount)
{
    if (balance - amount < MIN_BALANCE)
        throw InsufficientFunds(); 
    balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber)
{
    NextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber()
{
    return NextAccountNumber;
}

ofstream &operator<<(ofstream &ofs, Account &acc)
{
    ofs << acc.accountNumber << endl;
    ofs << acc.FirstName << endl;
    ofs << acc.LastName << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream &operator>>(ifstream &ifs, Account &acc)
{
    ifs >> acc.accountNumber;
    ifs >> acc.FirstName;
    ifs >> acc.LastName;
    ifs >> acc.balance;
    return ifs;
}

ostream &operator<<(ostream &os, Account &acc)
{
    os << "First Name: " << acc.getFirstName() << endl;
    os << "Last Name: " << acc.getLastName() << endl;
    os << "Account Number: " << acc.getAccNo() << endl;
    os << "Balance: " << acc.getBalance() << endl;
    return os;
}

// Bank class
class Bank
{
private:
    map<long, Account> accounts;

public:
    Bank();

    // Bank functionalities
    Account OpenAccount(string fname, string lname, float balance);
    Account BalanceEnquiry(long accountNumber);
    Account Deposit(long accountNumber, float amount);
    Account Withdraw(long accountNumber, float amount);
    void CloseAccount(long accountNumber);
    void ShowAllAccounts();

    ~Bank();
};

Bank::Bank()
{
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if (!infile)
    {
        cout << "Error in Opening! File Not Found!" << endl;
        return;
    }
    while (infile >> account)
    {
        accounts.insert(pair<long, Account>(account.getAccNo(), account));
    }
    Account::setLastAccountNumber(account.getAccNo()); // Update last account number
    infile.close();
}

// Open an account
Account Bank::OpenAccount(string fname, string lname, float balance)
{
    ofstream outfile;
    Account account(fname, lname, balance);
    accounts.insert(pair<long, Account>(account.getAccNo(), account));
    outfile.open("Bank.data", ios::trunc);
    for (auto itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile << itr->second;
    }
    outfile.close();
    return account;
}

// Balance enquiry
Account Bank::BalanceEnquiry(long accountNumber)
{
    auto itr = accounts.find(accountNumber);
    return itr->second;
}

// Deposit money 
Account Bank::Deposit(long accountNumber, float amount)
{
    auto itr = accounts.find(accountNumber);
    itr->second.Deposit(amount);
    return itr->second;
}

// Withdraw money
Account Bank::Withdraw(long accountNumber, float amount)
{
    auto itr = accounts.find(accountNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}

// Close an account
void Bank::CloseAccount(long accountNumber)
{
    auto itr = accounts.find(accountNumber);
    cout << "Account Deleted: " << itr->second;
    accounts.erase(accountNumber);
}

// Show all accounts
void Bank::ShowAllAccounts()
{
    for (auto itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        cout << "Account " << itr->first << endl
             << itr->second << endl;
    }
}

Bank::~Bank()
{
    ofstream outfile;
    outfile.open("Bank.data", ios::trunc);
    for (auto itr = accounts.begin(); itr != accounts.end(); itr++)
    {
        outfile << itr->second;
    }
    outfile.close();
}


int main()
{
    Bank b;
    Account acc;
    int choice;
    string fname, lname;
    long accountNumber;
    float balance, amount;

    cout << "****** Banking System *******" << endl;
    cout << "24/7 Service Available" << endl;

    do
    {
        cout << "\n\tSelect one option below ";
        cout << "\n\t1 Open an Account";
        cout << "\n\t2 Balance Enquiry";
        cout << "\n\t3 Deposit";
        cout << "\n\t4 Withdrawal";
        cout << "\n\t5 Close an Account";
        cout << "\n\t6 Show All Accounts";
        cout << "\n\t7 Quit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "Enter First Name: ";
            cin >> fname;
            cout << "Enter Last Name: ";
            cin >> lname;
            cout << "Enter Initial Balance: ";
            cin >> balance;
            acc = b.OpenAccount(fname, lname, balance);
            cout << endl
                 << "Congratulations! Account is Created." << endl;
            cout << acc;
            break;

        case 2:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            acc = b.BalanceEnquiry(accountNumber);
            cout << endl
                 << "Your Account Details" << endl;
            cout << acc;
            break;

        case 3:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Amount: ";
            cin >> amount;
            acc = b.Deposit(accountNumber, amount);
            cout << endl
                 << "Amount Deposited." << endl;
            cout << acc;
            break;

        case 4:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Amount: ";
            cin >> amount;
            acc = b.Withdraw(accountNumber, amount);
            cout << endl
                 << "Amount Withdrawn." << endl;
            cout << acc;
            break;

        case 5:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            b.CloseAccount(accountNumber);
            cout << endl
                 << "Account Closed." << endl;
            break;

        case 6:
            b.ShowAllAccounts();
            break;

        case 7:
            break;

        default:
            cout << "\n Enter correct choice.";
            exit(0);
        }
    } while (choice != 7);

    return 0;
}
