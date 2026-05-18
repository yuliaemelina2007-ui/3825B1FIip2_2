#include <iostream>
#include <string>
#include <vector>
#include <cmath>

struct Client {
    std::string accountNumber;
    std::string fullName;
    int balance;
    std::string password;
    bool hasCredit;
    double creditAmount;
    double remainingAmount;
    int creditTerm;
    double interestRate;
    int monthsPaid;

    Client() : balance(0), hasCredit(false), creditAmount(0),
        remainingAmount(0), creditTerm(0), interestRate(0), monthsPaid(0) {
    }
};
class Credit {
private:
    Client* currentClient;

    double getInterestRate(int termYears, double amount) {
        if (termYears == 1) {
            if (amount < 100000) return 0.10;
            else if (amount < 500000) return 0.15;
            else if (amount < 1000000) return 0.18;
            else return 0.20;
        }
        else if (termYears == 2) {
            if (amount < 100000) return 0.11;
            else if (amount < 500000) return 0.14;
            else if (amount < 1000000) return 0.16;
            else return 0.18;
        }
        else if (termYears == 3) {
            if (amount < 100000) return 0.12;
            else if (amount < 500000) return 0.13;
            else if (amount < 1000000) return 0.15;
            else return 0.17;
        }
        else if (termYears == 5) {
            if (amount < 100000) return 0.13;
            else if (amount < 500000) return 0.14;
            else if (amount < 1000000) return 0.16;
            else return 0.18;
        }
        else {
            if (amount < 100000) return 0.14;
            else if (amount < 500000) return 0.15;
            else if (amount < 1000000) return 0.17;
            else return 0.19;
        }
    }

    double calculateMonthlyPayment(double amount, int termYears, double rate) {
        int months = termYears * 12;
        double monthlyRate = rate / 12.0;
        double payment = amount * monthlyRate * pow(1 + monthlyRate, months) / (pow(1 + monthlyRate, months) - 1);
        return payment;
    }

public:
    Credit() : currentClient(nullptr) {}

    void authorize(Client* client) {
        currentClient = client;
        std::cout << "Authorization successful! Welcome, " << currentClient->fullName << std::endl;
    }

    void showAvailableCredits() {
        if (currentClient == nullptr) {
            std::cout << "Error: you must authorize first!" << std::endl;
            return;
        }

        std::cout << "\n=== AVAILABLE CREDITS ===" << std::endl;
        std::cout << "Available terms: 1 year, 2 years, 3 years, 5 years, 15 years\n" << std::endl;

        int terms[] = { 1, 2, 3, 5, 15 };
        double amounts[] = { 50000, 300000, 750000, 2000000 };
        std::string ranges[] = { "up to 100k", "100k-500k", "500k-1M", "1M-3M" };

        for (int i = 0; i < 5; i++) {
            std::cout << "Term " << terms[i] << " year(s):" << std::endl;
            for (int j = 0; j < 4; j++) {
                double rate = getInterestRate(terms[i], amounts[j]);
                double payment = calculateMonthlyPayment(amounts[j], terms[i], rate);
                std::cout << "  - Amount " << ranges[j] << " RUB, rate " << rate * 100 << "%, monthly payment ~"
                    << (int)payment << " RUB" << std::endl;
            }
            std::cout << std::endl;
        }
    }

    void checkCreditStatus() {
        if (currentClient == nullptr) {
            std::cout << "Error: you must authorize first!" << std::endl;
            return;
        }

        if (currentClient->hasCredit) {
            std::cout << "\n=== CREDIT INFORMATION ===" << std::endl;
            std::cout << "You have an active credit!" << std::endl;
            std::cout << "Credit amount: " << currentClient->creditAmount << " RUB" << std::endl;
            std::cout << "Remaining debt: " << currentClient->remainingAmount << " RUB" << std::endl;
            std::cout << "Credit term: " << currentClient->creditTerm << " years" << std::endl;
            std::cout << "Interest rate: " << currentClient->interestRate * 100 << "%" << std::endl;
            std::cout << "Months paid: " << currentClient->monthsPaid << std::endl;
        }
        else {
            std::cout << "You have no active credits." << std::endl;
        }
    }

    bool checkCreditEligibility(double amount, int termYears) {
        if (currentClient == nullptr) {
            std::cout << "Error: you must authorize first!" << std::endl;
            return false;
        }

        if (currentClient->hasCredit) {
            std::cout << "Error: you already have an active credit!" << std::endl;
            return false;
        }

        if (termYears != 1 && termYears != 2 && termYears != 3 && termYears != 5 && termYears != 15) {
            std::cout << "Error: unsupported credit term! Available: 1, 2, 3, 5, 15 years" << std::endl;
            return false;
        }

        if (amount <= 0) {
            std::cout << "Error: credit amount must be positive!" << std::endl;
            return false;
        }

        double rate = getInterestRate(termYears, amount);
        double monthlyPayment = calculateMonthlyPayment(amount, termYears, rate);
        double requiredForSix = monthlyPayment * 6;

        std::cout << "\n=== CREDIT CHECK ===" << std::endl;
        std::cout << "Credit amount: " << amount << " RUB" << std::endl;
        std::cout << "Term: " << termYears << " years" << std::endl;
        std::cout << "Monthly payment: " << monthlyPayment << " RUB" << std::endl;
        std::cout << "Required for 6 payments: " << requiredForSix << " RUB" << std::endl;
        std::cout << "Your balance: " << currentClient->balance << " RUB" << std::endl;

        if (currentClient->balance >= requiredForSix) {
            std::cout << "Credit can be approved!" << std::endl;
            return true;
        }
        else {
            std::cout << "Credit NOT approved! Insufficient funds for 6 monthly payments." << std::endl;
            return false;
        }
    }

    bool takeCredit(double amount, int termYears) {
        if (!checkCreditEligibility(amount, termYears)) {
            return false;
        }

        double rate = getInterestRate(termYears, amount);

        currentClient->hasCredit = true;
        currentClient->creditAmount = amount;
        currentClient->remainingAmount = amount;
        currentClient->creditTerm = termYears;
        currentClient->interestRate = rate;
        currentClient->monthsPaid = 0;

        currentClient->balance += amount;

        std::cout << "\nCredit successfully issued!" << std::endl;
        std::cout << "Amount " << amount << " RUB transferred to your account." << std::endl;
        std::cout << "Your new balance: " << currentClient->balance << " RUB" << std::endl;

        return true;
    }

    void showCurrentCreditState() {
        if (currentClient == nullptr) {
            std::cout << "Error: you must authorize first!" << std::endl;
            return;
        }

        if (!currentClient->hasCredit) {
            std::cout << "You have no active credit!" << std::endl;
            return;
        }

        double monthlyPayment = calculateMonthlyPayment(currentClient->creditAmount, currentClient->creditTerm, currentClient->interestRate);
        int totalMonths = currentClient->creditTerm * 12;
        int monthsLeft = totalMonths - currentClient->monthsPaid;

        std::cout << "\n=== CREDIT STATUS ===" << std::endl;
        std::cout << "Credit amount: " << currentClient->creditAmount << " RUB" << std::endl;
        std::cout << "Remaining debt: " << currentClient->remainingAmount << " RUB" << std::endl;
        std::cout << "Monthly payment: " << monthlyPayment << " RUB" << std::endl;
        std::cout << "Months paid: " << currentClient->monthsPaid << " out of " << totalMonths << std::endl;
        std::cout << "Months left: " << monthsLeft << std::endl;
        std::cout << "Interest rate: " << currentClient->interestRate * 100 << "% per year" << std::endl;
    }

    bool payMonthlyPayment(double amount) {
        if (currentClient == nullptr) {
            std::cout << "Error: you must authorize first!" << std::endl;
            return false;
        }

        if (!currentClient->hasCredit) {
            std::cout << "Error: you have no active credit!" << std::endl;
            return false;
        }

        double monthlyPayment = calculateMonthlyPayment(currentClient->creditAmount, currentClient->creditTerm, currentClient->interestRate);

        if (amount < monthlyPayment) {
            std::cout << "Error: payment amount (" << amount << " RUB) is less than accrued (" << monthlyPayment << " RUB)!" << std::endl;
            return false;
        }

        if (amount > currentClient->balance) {
            std::cout << "Error: insufficient funds! Your balance: " << currentClient->balance << " RUB" << std::endl;
            return false;
        }

        currentClient->balance -= amount;

        double monthlyRate = currentClient->interestRate / 12.0;
        double interest = currentClient->remainingAmount * monthlyRate;
        double principal = monthlyPayment - interest;

        if (amount > monthlyPayment) {
            double extraPayment = amount - monthlyPayment;
            principal += extraPayment;
        }

        currentClient->remainingAmount -= principal;
        currentClient->monthsPaid++;

        std::cout << "Payment successful!" << std::endl;
        std::cout << "Charged: " << amount << " RUB" << std::endl;
        std::cout << "Remaining debt: " << currentClient->remainingAmount << " RUB" << std::endl;
        std::cout << "Your balance: " << currentClient->balance << " RUB" << std::endl;

        if (currentClient->remainingAmount <= 0.01) {
            currentClient->hasCredit = false;
            currentClient->creditAmount = 0;
            currentClient->remainingAmount = 0;
            std::cout << "Congratulations! Credit fully paid off!" << std::endl;
        }

        return true;
    }

    bool earlyRepayment() {
        if (currentClient == nullptr) {
            std::cout << "Error: you must authorize first!" << std::endl;
            return false;
        }

        if (!currentClient->hasCredit) {
            std::cout << "Error: you have no active credit!" << std::endl;
            return false;
        }

        if (currentClient->balance < currentClient->remainingAmount) {
            std::cout << "Error: insufficient funds for early repayment!" << std::endl;
            std::cout << "Required: " << currentClient->remainingAmount << " RUB" << std::endl;
            std::cout << "Your balance: " << currentClient->balance << " RUB" << std::endl;
            return false;
        }

        currentClient->balance -= currentClient->remainingAmount;

        std::cout << "\n=== EARLY REPAYMENT ===" << std::endl;
        std::cout << "Repaid amount: " << currentClient->remainingAmount << " RUB" << std::endl;
        std::cout << "Remaining balance: " << currentClient->balance << " RUB" << std::endl;

        currentClient->hasCredit = false;
        currentClient->creditAmount = 0;
        currentClient->remainingAmount = 0;
        currentClient->creditTerm = 0;
        currentClient->interestRate = 0;
        currentClient->monthsPaid = 0;

        std::cout << "Credit fully repaid early!" << std::endl;

        return true;
    }

    void logout() {
        if (currentClient != nullptr) {
            std::cout << "Client " << currentClient->fullName << " logged out." << std::endl;
            currentClient = nullptr;
        }
    }

    bool isAuthorized() {
        return currentClient != nullptr;
    }
};

class ProcessingCenter {
private:
    std::vector<Client> clients;
    Credit creditSystem;

    int findClientByAccount(const std::string& account) {
        for (size_t i = 0; i < clients.size(); i++) {
            if (clients[i].accountNumber == account) {
                return i;
            }
        }
        return -1;
    }

public:
    void addClient(const std::string& account, const std::string& name, int balance, const std::string& pass) {
        if (findClientByAccount(account) != -1) {
            std::cout << "Error: client with account number " << account << " already exists!" << std::endl;
            return;
        }

        if (pass.length() <= 3) {
            std::cout << "Error: password must be longer than 3 characters!" << std::endl;
            return;
        }

        if (account.length() != 4) {
            std::cout << "Error: account number must consist of 4 digits!" << std::endl;
            return;
        }

        for (size_t i = 0; i < account.length(); i++) {
            if (account[i] < '0' || account[i] > '9') {
                std::cout << "Error: account number must contain only digits!" << std::endl;
                return;
            }
        }

        if (account < "0001" || account > "9999") {
            std::cout << "Error: account number must be from 0001 to 9999!" << std::endl;
            return;
        }

        Client newClient;
        newClient.accountNumber = account;
        newClient.fullName = name;
        newClient.balance = balance;
        newClient.password = pass;
        newClient.hasCredit = false;

        clients.push_back(newClient);
        std::cout << "Client " << name << " successfully added!" << std::endl;
    }

    bool authorizeClient(const std::string& account, const std::string& password) {
        int index = findClientByAccount(account);
        if (index != -1 && clients[index].password == password) {
            creditSystem.authorize(&clients[index]);
            return true;
        }
        std::cout << "Authorization failed!" << std::endl;
        return false;
    }

    Credit& getCreditSystem() {
        return creditSystem;
    }

    void addTestClients() {
        addClient("0001", "Ivanov Ivan Ivanovich", 150000, "pass1234");
        addClient("0002", "Petrov Petr Petrovich", 50000, "mypassword");
        addClient("0003", "Sidorova Anna Sergeevna", 300000, "securepass");
    }

    void run() {
        int choice;
        std::string account, password;
        double amount;
        int term;

        std::cout << "=== BANK PROCESSING CENTER ===" << std::endl;

        while (true) {
            std::cout << "\n1 - Authorization" << std::endl;
            std::cout << "2 - Exit" << std::endl;
            std::cout << "Choose action: ";
            std::cin >> choice;

            if (choice == 1) {
                std::cout << "Enter account number: ";
                std::cin >> account;
                std::cout << "Enter password: ";
                std::cin >> password;

                if (authorizeClient(account, password)) {
                    int userChoice;
                    do {
                        std::cout << "\n=== CLIENT MENU ===" << std::endl;
                        std::cout << "1 - Show available credits" << std::endl;
                        std::cout << "2 - Check credit status" << std::endl;
                        std::cout << "3 - Check credit eligibility" << std::endl;
                        std::cout << "4 - Take credit" << std::endl;
                        std::cout << "5 - Show current credit state" << std::endl;
                        std::cout << "6 - Make payment" << std::endl;
                        std::cout << "7 - Early repayment" << std::endl;
                        std::cout << "8 - Logout" << std::endl;
                        std::cout << "Choose action: ";
                        std::cin >> userChoice;

                        switch (userChoice) {
                        case 1:
                            creditSystem.showAvailableCredits();
                            break;
                        case 2:
                            creditSystem.checkCreditStatus();
                            break;
                        case 3:
                            std::cout << "Enter credit amount: ";
                            std::cin >> amount;
                            std::cout << "Enter term (1,2,3,5,15 years): ";
                            std::cin >> term;
                            creditSystem.checkCreditEligibility(amount, term);
                            break;
                        case 4:
                            std::cout << "Enter credit amount: ";
                            std::cin >> amount;
                            std::cout << "Enter term (1,2,3,5,15 years): ";
                            std::cin >> term;
                            creditSystem.takeCredit(amount, term);
                            break;
                        case 5:
                            creditSystem.showCurrentCreditState();
                            break;
                        case 6:
                            std::cout << "Enter payment amount: ";
                            std::cin >> amount;
                            creditSystem.payMonthlyPayment(amount);
                            break;
                        case 7:
                            creditSystem.earlyRepayment();
                            break;
                        case 8:
                            creditSystem.logout();
                            break;
                        default:
                            std::cout << "Invalid choice!" << std::endl;
                        }
                    } while (userChoice != 8);
                }
            }
            else if (choice == 2) {
                std::cout << "Goodbye!" << std::endl;
                break;
            }
        }
    }
};

int main() {
    ProcessingCenter center;
    center.addTestClients();
    center.run();

    return 0;
}