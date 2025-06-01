#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <conio.h> // _getch

const std::string DELIMITER = "#//#";
const std::string FILE_NAME = "Bank.txt";

enum MainMenuOption {
    Add = 1, View, Find, Delete, Update, Transaction
};

enum TransactionMenuOption {
    Deposit = 1, Withdraw, TotalBalance
};

struct ClientData {
    std::string accountNumber;
    short pinCode;
    std::string name;
    std::string phoneNumber;
    float balance;
};

void getValidPositiveNumber(float& number, std::string message) {
    std::cout << message;
    while (!(std::cin >> number) || number < 1) {
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        std::cout << "Enter a valid positive number: ";
    }
}

void getValidPositiveNumber(short& number, std::string message) {
    std::cout << message;
    while (!(std::cin >> number) || number < 1) {
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        std::cout << "Enter a valid positive number: ";
    }
}

bool getUserApproval(std::string message) {
    char answer;
    std::cout << message;
    while (std::cin >> answer && !(toupper(answer) == 'Y' || toupper(answer) == 'N')) {
        std::cin.ignore(INT_MAX, '\n');
        std::cout << "Enter Y/N: ";
    }
    return toupper(answer) == 'Y';
}

std::vector<std::string> serializeClientData(std::vector<ClientData>& clients, std::vector<std::string>& clientDataStr) {
    for (int i = 0; i < clients.size(); i++) {
        clientDataStr.push_back("");
        clientDataStr.at(i) = clients.at(i).accountNumber + DELIMITER;
        clientDataStr.at(i) += std::to_string(clients.at(i).pinCode) + DELIMITER;
        clientDataStr.at(i) += clients.at(i).name + DELIMITER;
        clientDataStr.at(i) += clients.at(i).phoneNumber + DELIMITER;
        clientDataStr.at(i) += std::to_string(clients.at(i).balance);
    }
    return clientDataStr;
}

void displayErrorMessage(std::string message) {
    std::cout << "\n\n" << message << "\n\n";
}

std::vector<std::string> parseClientData(std::string clientLine) {
    std::vector<std::string> parsedData;
    std::string word;
    int pos;
    while ((pos = clientLine.find(DELIMITER)) != std::string::npos) {
        word = clientLine.substr(0, pos);
        if (!word.empty()) {
            parsedData.push_back(word);
        }
        clientLine.erase(0, pos + DELIMITER.length());
    }
    if (!clientLine.empty()) {
        parsedData.push_back(clientLine);
    }
    return parsedData;
}

void printClientData(ClientData& client) {
    std::cout << "\n\n";
    std::cout << std::setw(30) << std::setfill('-') << '-' << std::endl;
    std::cout << "Account Number: " << client.accountNumber << std::endl;
    std::cout << "Pin Code: " << client.pinCode << std::endl;
    std::cout << "Name: " << client.name << std::endl;
    std::cout << "Phone Number: " << client.phoneNumber << std::endl;
    std::cout << "Balance: " << client.balance << std::endl;
    std::cout << std::setw(30) << std::setfill('-') << '-' << "\n\n";
}

std::string getStringInput(std::string message) {
    std::cout << message;
    std::getline(std::cin >> std::ws, message);
    return message;
}

short findClientIndexByAccountNumber(std::vector<ClientData>& clients, std::string accountNumber) {
    for (int i = 0; i < clients.size(); i++) {
        if (clients.at(i).accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

ClientData convertLineToClientData(std::string line) {
    ClientData client;
    std::vector<std::string> clientData = parseClientData(line);
    client.accountNumber = clientData.at(0);
    client.pinCode = std::stoi(clientData.at(1));
    client.name = clientData.at(2);
    client.phoneNumber = clientData.at(3);
    client.balance = std::stof(clientData.at(4));
    return client;
}

int countFileLines(std::string fileName) {
    std::ifstream inputFile(fileName, std::ios::in);
    int lineCount = 0;
    if (inputFile) {
        std::string line;
        while (std::getline(inputFile, line)) {
            lineCount++;
        }
    }
    return lineCount;
}

std::vector<ClientData> loadClientsFromFile() {
    std::ifstream inputFile(FILE_NAME, std::ios::in);
    std::vector<ClientData> clients;
    if (inputFile) {
        ClientData client;
        std::string line;
        while (std::getline(inputFile, line)) {
            client = convertLineToClientData(line);
            clients.emplace_back(client);
        }
    }
    return clients;
}

std::string createSeparatorLine(char character, int count) {
    return std::string(count, character);
}

void getValidAccountNumber(std::vector<ClientData>& clients, short& index) {
    std::string accountNumber;
    std::cout << "Client Account: ";
    while (std::getline(std::cin >> std::ws, accountNumber) && (index = findClientIndexByAccountNumber(clients, accountNumber)) == std::string::npos) {
        displayErrorMessage("Account Not Found!!");
        std::cout << "Client Account: ";
    }
}

void printHeader(std::string message) {
    std::cout << std::setw(35) << std::setfill('-') << '-' << std::endl;
    std::cout << createSeparatorLine('\t', 1) << message << std::endl;
    std::cout << std::setw(35) << std::setfill('-') << '-' << "\n\n";
}

void displayClientData() {
    printHeader("Find Client Screen");
    std::vector<ClientData> clients = loadClientsFromFile();
    short index;
    getValidAccountNumber(clients, index);
    printClientData(clients.at(index));
}

void saveClientDataToFile(std::vector<ClientData>& clients, std::ios_base::openmode mode = std::ios::out) {
    std::vector<std::string> clientDataStr;
    serializeClientData(clients, clientDataStr);
    std::ofstream outputFile("Bank.txt", mode);
    if (outputFile) {
        for (auto& client : clientDataStr) {
            outputFile << client << std::endl;
        }
    }
}

void deleteClientAccount() {
    printHeader("Delete Client Screen");
    std::vector<ClientData> clients = loadClientsFromFile();
    short index;
    getValidAccountNumber(clients, index);
    printClientData(clients.at(index));
    if (getUserApproval("Are You Sure You Want Delete?: ")) {
        clients.erase(clients.begin() + index);
        saveClientDataToFile(clients);
    }
}

void editClientData(ClientData& client) {
    getValidPositiveNumber(client.pinCode, "Enter Pin Code: ");
    std::cout << "Enter Name: ";
    std::cin.ignore(INT_MAX, '\n');
    std::getline(std::cin >> std::ws, client.name);
    std::cout << "Enter Phone: ";
    std::getline(std::cin >> std::ws, client.phoneNumber);
    getValidPositiveNumber(client.balance, "Enter Account Balance: ");
}

void updateClientData() {
    printHeader("Update Client Info Screen");
    std::vector<ClientData> clients = loadClientsFromFile();
    short index;
    getValidAccountNumber(clients, index);
    printClientData(clients.at(index));
    if (getUserApproval("Are You Sure You Want Update?: ")) {
        editClientData(clients.at(index));
        saveClientDataToFile(clients);
    }
}

void displayAllClientRecords(std::ifstream& inputFile) {
    std::string line;
    for (short i = 1; std::getline(inputFile, line); i++) {
        std::vector<std::string> parsedData = parseClientData(line);
        std::cout << "| " << std::setw(20) << std::left << std::to_string(i) + '.' + parsedData.at(0);
        std::cout << "| " << std::setw(15) << std::left << parsedData.at(1);
        std::cout << "| " << std::setw(30) << std::left << parsedData.at(2);
        std::cout << "| " << std::setw(15) << std::left << parsedData.at(3);
        std::cout << "| " << std::setw(29) << std::left << parsedData.at(4) << '|' << std::endl;
    }
}

void printClientListHeader() {
    int clientCount = countFileLines(FILE_NAME);
    std::cout << createSeparatorLine('\t', 6) << "Client List (" << clientCount << ") Client(s). \n\n";
    std::cout << std::setw(120) << std::setfill('-') << '-' << std::setfill(' ') << std::endl;
    std::cout << "| " << std::setw(20) << std::left << " Account Number";
    std::cout << "| " << std::setw(15) << std::left << " Pin Code";
    std::cout << "| " << std::setw(30) << std::left << " Client Name";
    std::cout << "| " << std::setw(15) << std::left << " Phone";
    std::cout << "| " << std::setw(29) << std::left << " Balance" << '|' << std::endl;
    std::cout << std::setw(120) << std::setfill('-') << '-' << std::setfill(' ') << std::endl;
}

void displayClientList() {
    std::ifstream inputFile(FILE_NAME, std::ios::in);
    if (inputFile) {
        printClientListHeader();
        displayAllClientRecords(inputFile);
        std::cout << std::setw(120) << std::setfill('-') << '-' << "\n";
    }
}

std::string enterNewAccountNumber() {
    std::vector<ClientData> clients = loadClientsFromFile();
    std::string accountNumber;
    while (std::getline(std::cin >> std::ws, accountNumber) && findClientIndexByAccountNumber(clients, accountNumber) != std::string::npos) {
        displayErrorMessage("This account already exists!!");
        std::cout << "Enter another account Number: ";
    }
    return accountNumber;
}

void addNewClients() {
    short numberOfClients;
    getValidPositiveNumber(numberOfClients, "\n How many clients do you want to add? \n");
    static int clientCounter = 1;
    for (int i = 0; i < numberOfClients; i++) {
        std::vector<ClientData> newClients(1, {"", 0, "", "", 0.0});
        std::cout << "\n Enter Data of " << (clientCounter++) << ".Person \n\n";
        std::cout << "Enter Account Number: ";
        newClients.at(0).accountNumber = enterNewAccountNumber();
        getValidPositiveNumber(newClients.at(0).pinCode, "Enter Pin Code: ");
        std::cout << "Enter Name: ";
        std::cin.ignore(INT_MAX, '\n');
        std::getline(std::cin >> std::ws, newClients.at(0).name);
        std::cout << "Enter Phone: ";
        std::getline(std::cin >> std::ws, newClients.at(0).phoneNumber);
        getValidPositiveNumber(newClients.at(0).balance, "Enter Account Balance: ");
        saveClientDataToFile(newClients, std::ios::app);
    }
    while (getUserApproval("Do You Need To Append More Clients? ")) {
        return addNewClients();
    }
}

void addNewClient() {
    printHeader("Add New Clients Screen");
    addNewClients();
}

void displayMainMenu() {
    std::cout << std::setw(35) << std::setfill('=') << '=' << std::setfill(' ') << "\n\n";
    std::cout << createSeparatorLine('\t', 1) << "Welcome To Our Bank" << "\n\n";
    std::cout << std::setw(35) << std::setfill('=') << '=' << std::setfill(' ') << "\n";
    std::cout << " [1]. Add New Client" << std::endl;
    std::cout << " [2]. View Client List" << std::endl;
    std::cout << " [3]. Find Data Of Client" << std::endl;
    std::cout << " [4]. Delete Client" << std::endl;
    std::cout << " [5]. Update Client's Data" << std::endl;
    std::cout << " [6]. Transaction" << std::endl;
    std::cout << " [7]. Exit" << std::endl;
    std::cout << std::setw(35) << std::setfill('=') << '=' << std::setfill(' ') << "\n\n";
}

short getMainMenuChoice() {
    short choice;
    displayMainMenu();
    getValidPositiveNumber(choice, "Choice: ");
    return choice;
}

void deposit() {
    printHeader("Deposit Screen");
    std::vector<ClientData> clients = loadClientsFromFile();
    short index;
    getValidAccountNumber(clients, index);
    printClientData(clients.at(index));
    if (getUserApproval("Are You Sure You Want Deposit?: ")) {
        float amount;
        getValidPositiveNumber(amount, "Enter Value You need: ");
        clients.at(index).balance += amount;
        saveClientDataToFile(clients);
    }
}

void withdraw() {
    printHeader("Withdraw Screen");
    std::vector<ClientData> clients = loadClientsFromFile();
    short index;
    getValidAccountNumber(clients, index);
    printClientData(clients.at(index));
    if (getUserApproval("Are You Sure You Want Withdraw?: ")) {
        float amount;
        do {
            getValidPositiveNumber(amount, "Withdraw an amount not exceeding the balance: ");
        } while (amount > clients.at(index).balance);
        clients.at(index).balance -= amount;
        saveClientDataToFile(clients);
    }
}

void displayTransactionMenu() {
    std::cout << std::setw(40) << std::setfill('=') << '=' << std::setfill(' ') << "\n";
    std::cout << createSeparatorLine('\t', 1) << "Transaction Menu Screen" << "\n";
    std::cout << std::setw(40) << std::setfill('=') << '=' << std::setfill(' ') << "\n";
    std::cout << " [1]. Deposit" << std::endl;
    std::cout << " [2]. Withdraw" << std::endl;
    std::cout << " [3]. Total Balance" << std::endl;
    std::cout << " [4]. Exit" << std::endl;
    std::cout << std::setw(40) << std::setfill('=') << '=' << std::setfill(' ') << "\n";
}

short getTransactionChoice() {
    short choice;
    displayTransactionMenu();
    getValidPositiveNumber(choice, "Choose One: ");
    return choice;
}

void printTotalBalanceHeader() {
    int clientCount = countFileLines(FILE_NAME);
    std::cout << createSeparatorLine('\t', 6) << "Client List (" << clientCount << ") Client(s). \n\n";
    std::cout << std::setw(120) << std::setfill('-') << '-' << std::setfill(' ') << std::endl;
    std::cout << "| " << std::setw(30) << std::left << " Account Number";
    std::cout << "| " << std::setw(44) << std::left << " Client Name";
    std::cout << "| " << std::setw(39) << std::left << " Balance" << '|' << std::endl;
    std::cout << std::setw(120) << std::setfill('-') << '-' << std::setfill(' ') << std::endl;
}

void displayClientBalances() {
    std::string line;
    std::ifstream inputFile(FILE_NAME, std::ios::in);
    for (short i = 1; std::getline(inputFile, line); i++) {
        std::vector<std::string> parsedData = parseClientData(line);
        std::cout << "| " << std::setw(30) << std::left << std::to_string(i) + '.' + parsedData.at(0);
        std::cout << "| " << std::setw(44) << std::left << parsedData.at(2);
        std::cout << "| " << std::setw(39) << std::left << parsedData.at(4) << '|' << std::endl;
    }
}

float calculateTotalBalance() {
    float total = 0;
    std::vector<ClientData> clients = loadClientsFromFile();
    for (int i = 0; i < clients.size(); i++) {
        total += clients.at(i).balance;
    }
    return total;
}

void printTotalBalance() {
    float total = calculateTotalBalance();
    std::cout << std::setw(64) << ' ' << "Total Balance = " << total << std::endl;
}

void displayTotalBalances() {
    printTotalBalanceHeader();
    displayClientBalances();
}

void handleTransactions();

void handleTransactions() {
    do {
        system("cls");
        TransactionMenuOption choice = static_cast<TransactionMenuOption>(getTransactionChoice());
        switch (choice) {
            case TransactionMenuOption::Deposit:
                system("cls");
                deposit();
                break;
            case TransactionMenuOption::Withdraw:
                system("cls");
                withdraw();
                break;
            case TransactionMenuOption::TotalBalance:
                system("cls");
                displayTotalBalances();
                std::cout << std::setw(120) << std::setfill('-') << '-' << std::setfill(' ') << std::endl;
                printTotalBalance();
                break;
            default:
                return;
        }
        std::cout << "\n\n";
        std::cout << std::setw(50) << std::setfill('+') << '+' << std::endl;
        std::cout << "Press any character to return to menu: " << std::endl;
        std::cout << std::setw(50) << std::setfill('+') << '+' << std::endl;
    } while (_getch());
}

void runBankingSystem();

void runBankingSystem() {
    do {
        system("cls");
        MainMenuOption choice = static_cast<MainMenuOption>(getMainMenuChoice());
        switch (choice) {
            case MainMenuOption::Add:
                system("cls");
                addNewClient();
                break;
            case MainMenuOption::View:
                system("cls");
                displayClientList();
                break;
            case MainMenuOption::Find:
                system("cls");
                displayClientData();
                break;
            case MainMenuOption::Delete:
                system("cls");
                deleteClientAccount();
                break;
            case MainMenuOption::Update:
                system("cls");
                updateClientData();
                break;
            case MainMenuOption::Transaction:
                handleTransactions();
                break;
            default:
                system("cls");
                std::cout << std::setw(48) << std::setfill('~') << '~' << std::endl;
                std::cout << "Thank you for using our banking system. Goodbye!" << std::endl;
                std::cout << std::setw(48) << std::setfill('~') << '~' << std::endl;
                return;
        }
        std::cout << "\n\n";
        std::cout << std::setw(50) << std::setfill('+') << '+' << std::endl;
        std::cout << "Press any character to return to menu: " << std::endl;
        std::cout << std::setw(50) << std::setfill('+') << '+' << std::endl;
    } while (_getch());
}

int main() {
    runBankingSystem();
    return 0;
}
