#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <conio.h> // _getch

using namespace std;

// Constants for file names and delimiters
const string DELIMITER = "#//#";
const string CLIENT_FILE_NAME = "Bank.txt";

// Enum for main menu options
enum MainMenuOption { Add = 1, View, Find, Delete, Update, Transaction };

// Enum for transaction menu options
enum TransactionMenuOption { Deposit = 1, Withdraw, TotalBalance };

// Structure to hold user information
struct UserInfo {
    string user_name{};
    string user_passwd{};
};

// Structure to hold client data
struct ClientData {
    string account_number;
    short pin_code;
    string name;
    string phone_number;
    float balance;
};

// Gets a valid positive float from user input
void getValidPositiveNumber(float& number, string message) {
    cout << message;
    while (!(cin >> number) || number < 1) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "Enter a valid positive number: ";
    }
}

// Gets a valid positive short from user input
void getValidPositiveNumber(short& number, string message) {
    cout << message;
    while (!(cin >> number) || number < 1) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "Enter a valid positive number: ";
    }
}

// Gets user approval with Y/N response
bool getUserApproval(string message) {
    char answer;
    cout << message;
    while (cin >> answer && !(toupper(answer) == 'Y' || toupper(answer) == 'N')) {
        cin.ignore(INT_MAX, '\n');
        cout << "Enter Y/N: ";
    }
    return toupper(answer) == 'Y';
}

// Serializes client data into a string
string serializeClientData(ClientData& client, string& client_data_str) {
    client_data_str = client.account_number + DELIMITER;
    client_data_str += to_string(client.pin_code) + DELIMITER;
    client_data_str += client.name + DELIMITER;
    client_data_str += client.phone_number + DELIMITER;
    client_data_str += to_string(client.balance);
    return client_data_str;
}

// Displays an error message
void displayErrorMessage(string message) {
    cout << "\n\n" << message << "\n\n";
}

// Parses client data from a string line
vector<string> parseClientData(string client_line) {
    vector<string> parsed_data;
    string word;
    int pos;
    while ((pos = client_line.find(DELIMITER)) != string::npos) {
        word = client_line.substr(0, pos);
        if (!word.empty()) parsed_data.push_back(word);
        client_line.erase(0, pos + DELIMITER.length());
    }
    if (!client_line.empty()) parsed_data.push_back(client_line);
    return parsed_data;
}

// Prints client data in a formatted way
void printClientData(ClientData& client) {
    cout << "\n\n";
    cout << setw(30) << setfill('-') << '-' << endl;
    cout << "Account Number: " << client.account_number << endl;
    cout << "Pin Code: " << client.pin_code << endl;
    cout << "Name: " << client.name << endl;
    cout << "Phone Number: " << client.phone_number << endl;
    cout << "Balance: " << client.balance << endl;
    cout << setw(30) << setfill('-') << '-' << "\n\n";
}

// Gets a string input from the user
string getStringInput(string message) {
    string input;
    cout << message;
    getline(cin >> ws, input);
    return input;
}

// Finds client index by account number
short findClientIndexByAccountNumber(vector<ClientData>& clients, string account_number) {
    for (size_t i = 0; i < clients.size(); i++) {
        if (clients.at(i).account_number == account_number) return static_cast<short>(i);
    }
    return -1;
}

// Converts a file line to ClientData struct
ClientData convertLineToClientData(string line) {
    ClientData client;
    vector<string> client_fields = parseClientData(line);
    client.account_number = client_fields.at(0);
    client.pin_code = stoi(client_fields.at(1));
    client.name = client_fields.at(2);
    client.phone_number = client_fields.at(3);
    client.balance = stof(client_fields.at(4));
    return client;
}

// Counts the number of lines in a file
int countFileLines(string file_name) {
    ifstream input_file(file_name, ios::in);
    int line_count = 0;
    if (input_file) {
        string line;
        while (getline(input_file, line)) line_count++;
    }
    return line_count;
}

// Loads clients from a file into a vector
vector<ClientData> loadClientsFromFile(const string file_name) {
    ifstream input_file(file_name, ios::in);
    vector<ClientData> clients;
    if (input_file) {
        ClientData client;
        string line;
        while (getline(input_file, line)) {
            client = convertLineToClientData(line);
            clients.emplace_back(client);
        }
    }
    return clients;
}

// Creates a separator line with a character
string createSeparatorLine(char character, int count) {
    return string(count, character);
}

// Gets a valid account number from user
void getValidAccountNumber(vector<ClientData>& clients, short& index) {
    string account_number;
    cout << "Client Account: ";
    getline(cin >> ws, account_number);
    index = findClientIndexByAccountNumber(clients, account_number);
    while (index == -1) {
        displayErrorMessage("Account Not Found!!");
        cout << "Client Account: ";
        getline(cin >> ws, account_number);
        index = findClientIndexByAccountNumber(clients, account_number);
    }
}

// Prints a header with a message
void printHeader(string message) {
    cout << setw(35) << setfill('-') << '-' << endl;
    cout << createSeparatorLine('\t', 1) << message << endl;
    cout << setw(35) << setfill('-') << '-' << "\n\n";
}

// Displays client data by account number
void displayClientData() {
    printHeader("Find Client Screen");
    vector<ClientData> clients = loadClientsFromFile(CLIENT_FILE_NAME);
    short index;
    getValidAccountNumber(clients, index);
    printClientData(clients.at(index));
}

// Saves client data to the file
void saveClientDataToFile(ClientData& client) {
    string client_data_str;
    serializeClientData(client, client_data_str);
    ofstream output_file(CLIENT_FILE_NAME, ios::app);
    if (output_file) output_file << client_data_str << endl;
}

// Deletes the contents of a file
void delete_file_contents(const string file_name) {
    ofstream o_file(file_name, ios::out | ios::trunc);
}

// Deletes a client account from the system
void deleteClientAccount() {
    printHeader("Delete Client Screen");
    vector<ClientData> clients = loadClientsFromFile(CLIENT_FILE_NAME);
    short index;
    getValidAccountNumber(clients, index);
    printClientData(clients.at(index));
    if (getUserApproval("Are You Sure You Want Delete?: ")) {
        clients.erase(clients.begin() + index);
        delete_file_contents(CLIENT_FILE_NAME);
        for (size_t i = 0; i < clients.size(); i++) saveClientDataToFile(clients.at(i));
    }
}

// Edits client data with user input
void editClientData(ClientData& client) {
    getValidPositiveNumber(client.pin_code, "Enter Pin Code: ");
    cout << "Enter Name: ";
    cin.ignore(INT_MAX, '\n');
    getline(cin >> ws, client.name);
    cout << "Enter Phone: ";
    getline(cin >> ws, client.phone_number);
    getValidPositiveNumber(client.balance, "Enter Account Balance: ");
}

// Updates client data in the system
void updateClientData() {
    printHeader("Update Client Info Screen");
    vector<ClientData> clients = loadClientsFromFile(CLIENT_FILE_NAME);
    short index;
    getValidAccountNumber(clients, index);
    printClientData(clients.at(index));
    if (getUserApproval("Are You Sure You Want Update?: ")) {
        editClientData(clients.at(index));
        delete_file_contents(CLIENT_FILE_NAME);
        for (size_t i = 0; i < clients.size(); i++) saveClientDataToFile(clients.at(i));
    }
}

// Displays all client records from the file
void displayAllClientRecords(ifstream& input_file) {
    string line;
    for (short i = 1; getline(input_file, line); i++) {
        vector<string> parsed_data = parseClientData(line);
        cout << "| " << setw(20) << left << to_string(i) + '.' + parsed_data.at(0);
        cout << "| " << setw(15) << left << parsed_data.at(1);
        cout << "| " << setw(30) << left << parsed_data.at(2);
        cout << "| " << setw(15) << left << parsed_data.at(3);
        cout << "| " << setw(29) << left << parsed_data.at(4) << '|' << endl;
    }
}

// Prints the header for the client list
void printClientListHeader() {
    int client_count = countFileLines(CLIENT_FILE_NAME);
    cout << createSeparatorLine('\t', 6) << "Client List (" << client_count << ") Client(s). \n\n";
    cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
    cout << "| " << setw(20) << left << " Account Number";
    cout << "| " << setw(15) << left << " Pin Code";
    cout << "| " << setw(30) << left << " Client Name";
    cout << "| " << setw(15) << left << " Phone";
    cout << "| " << setw(29) << left << " Balance" << '|' << endl;
    cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
}

// Displays the list of all clients
void displayClientList() {
    ifstream input_file(CLIENT_FILE_NAME, ios::in);
    if (input_file) {
        printClientListHeader();
        displayAllClientRecords(input_file);
        cout << setw(120) << setfill('-') << '-' << "\n";
    }
}

// Enters a new unique account number
string enterNewAccountNumber() {
    vector<ClientData> clients = loadClientsFromFile(CLIENT_FILE_NAME);
    string account_number;
    getline(cin >> ws, account_number);
    short pos = findClientIndexByAccountNumber(clients, account_number);
    while (pos != -1) {
        displayErrorMessage("This account already exists!!");
        cout << "Enter another account Number: ";
        getline(cin >> ws, account_number);
        pos = findClientIndexByAccountNumber(clients, account_number);
    }
    return account_number;
}

// Adds new clients to the system
void addNewClients() {
    short number_of_clients;
    getValidPositiveNumber(number_of_clients, "\n How many clients do you want to add? \n");
    static int client_counter = 1;
    for (int i = 0; i < number_of_clients; i++) {
        ClientData new_client{};
        cout << "\n Enter Data of " << (client_counter++) << ".Person \n\n";
        cout << "Enter Account Number: ";
        new_client.account_number = enterNewAccountNumber();
        getValidPositiveNumber(new_client.pin_code, "Enter Pin Code: ");
        cout << "Enter Name: ";
        cin.ignore(INT_MAX, '\n');
        getline(cin >> ws, new_client.name);
        cout << "Enter Phone: ";
        getline(cin >> ws, new_client.phone_number);
        getValidPositiveNumber(new_client.balance, "Enter Account Balance: ");
        saveClientDataToFile(new_client);
    }
    while (getUserApproval("Do You Need To Append More Clients? ")) addNewClients();
}

// Adds a new client via the interface
void addNewClient() {
    printHeader("Add New Clients Screen");
    addNewClients();
}

// Displays the main menu options
void displayMainMenu() {
    cout << setw(35) << setfill('=') << '=' << setfill(' ') << "\n\n";
    cout << createSeparatorLine('\t', 1) << "Welcome To Our Bank" << "\n\n";
    cout << setw(35) << setfill('=') << '=' << setfill(' ') << "\n";
    cout << " [1]. Add New Client" << endl;
    cout << " [2]. View Client List" << endl;
    cout << " [3]. Find Data Of Client" << endl;
    cout << " [4]. Delete Client" << endl;
    cout << " [5]. Update Client's Data" << endl;
    cout << " [6]. Transaction" << endl;
    cout << " [7]. Exit" << endl;
    cout << setw(35) << setfill('=') << '=' << setfill(' ') << "\n\n";
}

// Gets the user's main menu choice
short getMainMenuChoice() {
    short choice;
    displayMainMenu();
    getValidPositiveNumber(choice, "Choice: ");
    return choice;
}

// Handles deposit operation
void deposit() {
    printHeader("Deposit Screen");
    vector<ClientData> clients = loadClientsFromFile(CLIENT_FILE_NAME);
    short index;
    getValidAccountNumber(clients, index);
    printClientData(clients.at(index));
    if (getUserApproval("Are You Sure You Want Deposit?: ")) {
        float amount;
        getValidPositiveNumber(amount, "Enter Value You need: ");
        clients.at(index).balance += amount;
        delete_file_contents(CLIENT_FILE_NAME);
        for (size_t i = 0; i < clients.size(); i++) saveClientDataToFile(clients.at(i));
    }
}

// Handles withdraw operation
void withdraw() {
    printHeader("Withdraw Screen");
    vector<ClientData> clients = loadClientsFromFile(CLIENT_FILE_NAME);
    short index;
    getValidAccountNumber(clients, index);
    printClientData(clients.at(index));
    if (getUserApproval("Are You Sure You Want Withdraw?: ")) {
        float amount;
        do {
            getValidPositiveNumber(amount, "Withdraw an amount not exceeding the balance: ");
        } while (amount > clients.at(index).balance);
        clients.at(index).balance -= amount;
        delete_file_contents(CLIENT_FILE_NAME);
        for (size_t i = 0; i < clients.size(); i++) saveClientDataToFile(clients.at(i));
    }
}

// Displays the transaction menu
void displayTransactionMenu() {
    cout << setw(40) << setfill('=') << '=' << setfill(' ') << "\n";
    cout << createSeparatorLine('\t', 1) << "Transaction Menu Screen" << "\n";
    cout << setw(40) << setfill('=') << '=' << setfill(' ') << "\n";
    cout << " [1]. Deposit" << endl;
    cout << " [2]. Withdraw" << endl;
    cout << " [3]. Total Balance" << endl;
    cout << " [4]. Exit" << endl;
    cout << setw(40) << setfill('=') << '=' << setfill(' ') << "\n";
}

// Gets the user's transaction choice
short getTransactionChoice() {
    short choice;
    displayTransactionMenu();
    getValidPositiveNumber(choice, "Choose One: ");
    return choice;
}

// Prints header for total balance display
void printTotalBalanceHeader() {
    int client_count = countFileLines(CLIENT_FILE_NAME);
    cout << createSeparatorLine('\t', 6) << "Client List (" << client_count << ") Client(s). \n\n";
    cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
    cout << "| " << setw(30) << left << " Account Number";
    cout << "| " << setw(44) << left << " Client Name";
    cout << "| " << setw(39) << left << " Balance" << '|' << endl;
    cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
}

// Displays client balances
void displayClientBalances() {
    string line;
    ifstream input_file(CLIENT_FILE_NAME, ios::in);
    for (short i = 1; getline(input_file, line); i++) {
        vector<string> parsed_data = parseClientData(line);
        cout << "| " << setw(30) << left << to_string(i) + '.' + parsed_data.at(0);
        cout << "| " << setw(44) << left << parsed_data.at(2);
        cout << "| " << setw(39) << left << parsed_data.at(4) << '|' << endl;
    }
}

// Calculates total balance of all clients
float calculateTotalBalance() {
    float total = 0;
    vector<ClientData> clients = loadClientsFromFile(CLIENT_FILE_NAME);
    for (size_t i = 0; i < clients.size(); i++) total += clients.at(i).balance;
    return total;
}

// Prints the total balance
void printTotalBalance() {
    float total = calculateTotalBalance();
    cout << setw(64) << ' ' << "Total Balance = " << total << endl;
}

// Displays total balances of all clients
void displayTotalBalances() {
    printTotalBalanceHeader();
    displayClientBalances();
}

// Handles transaction operations
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
            cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
            printTotalBalance();
            break;
        default:
            return;
        }
        cout << "\n\n";
        cout << setw(50) << setfill('+') << '+' << endl;
        cout << "Press any character to return to menu: " << endl;
        cout << setw(50) << setfill('+') << '+' << endl;
    } while (_getch());
}

// Runs the banking system
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
            cout << setw(48) << setfill('~') << '~' << endl;
            cout << "Thank you for using our banking system. Goodbye!" << endl;
            cout << setw(48) << setfill('~') << '~' << endl;
            return;
        }
        cout << "\n\n";
        cout << setw(50) << setfill('+') << '+' << endl;
        cout << "Press any character to return to menu: " << endl;
        cout << setw(50) << setfill('+') << '+' << endl;
    } while (_getch());
}

// Main function to start the program
int main() {
    runBankingSystem();
    return 0;
}
