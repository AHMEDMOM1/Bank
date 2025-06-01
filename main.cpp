#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <conio.h> // _getch

using namespace std;

const string DELIMITER = "#//#";
const string FILE_NAME = "Bank.txt";

enum MainMenuOption {
	Add = 1, View, Find, Delete, Update, Transaction
};

enum TransactionMenuOption {
	Deposit = 1, Withdraw, TotalBalance
};

struct ClientData {
	string accountNumber;
	short pinCode;
	string name;
	string phoneNumber;
	float balance;
};

void getValidPositiveNumber(float& number, string message) {
	cout << message;
	while (!(cin >> number) || number < 1) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Enter a valid positive number: ";
	}
}

void getValidPositiveNumber(short& number, string message) {
	cout << message;
	while (!(cin >> number) || number < 1) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Enter a valid positive number: ";
	}
}

bool getUserApproval(string message) {
	char answer;
	cout << message;
	while (cin >> answer && !(toupper(answer) == 'Y' || toupper(answer) == 'N')) {
		cin.ignore(INT_MAX, '\n');
		cout << "Enter Y/N: ";
	}
	return toupper(answer) == 'Y';
}

vector<string> serializeClientData(vector<ClientData>& clients, vector<string>& clientDataStr) {
	for (int i = 0; i < clients.size(); i++) {
		clientDataStr.push_back("");
		clientDataStr.at(i) = clients.at(i).accountNumber + DELIMITER;
		clientDataStr.at(i) += to_string(clients.at(i).pinCode) + DELIMITER;
		clientDataStr.at(i) += clients.at(i).name + DELIMITER;
		clientDataStr.at(i) += clients.at(i).phoneNumber + DELIMITER;
		clientDataStr.at(i) += to_string(clients.at(i).balance);
	}
	return clientDataStr;
}

void displayErrorMessage(string message) {
	cout << "\n\n" << message << "\n\n";
}

vector<string> parseClientData(string clientLine) {
	vector<string> parsedData;
	string word;
	int pos;
	while ((pos = clientLine.find(DELIMITER)) != string::npos) {
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
	cout << "\n\n";
	cout << setw(30) << setfill('-') << '-' << endl;
	cout << "Account Number: " << client.accountNumber << endl;
	cout << "Pin Code: " << client.pinCode << endl;
	cout << "Name: " << client.name << endl;
	cout << "Phone Number: " << client.phoneNumber << endl;
	cout << "Balance: " << client.balance << endl;
	cout << setw(30) << setfill('-') << '-' << "\n\n";
}

string getStringInput(string message) {
	cout << message;
	getline(cin >> ws, message);
	return message;
}

short findClientIndexByAccountNumber(vector<ClientData>& clients, string accountNumber) {
	for (int i = 0; i < clients.size(); i++) {
		if (clients.at(i).accountNumber == accountNumber) {
			return i;
		}
	}
	return -1;
}

ClientData convertLineToClientData(string line) {
	ClientData client;
	vector<string> clientData = parseClientData(line);
	client.accountNumber = clientData.at(0);
	client.pinCode = stoi(clientData.at(1));
	client.name = clientData.at(2);
	client.phoneNumber = clientData.at(3);
	client.balance = stof(clientData.at(4));
	return client;
}

int countFileLines(string fileName) {
	ifstream inputFile(fileName, ios::in);
	int lineCount = 0;
	if (inputFile) {
		string line;
		while (getline(inputFile, line)) {
			lineCount++;
		}
	}
	return lineCount;
}

vector<ClientData> loadClientsFromFile() {
	ifstream inputFile(FILE_NAME, ios::in);
	vector<ClientData> clients;
	if (inputFile) {
		ClientData client;
		string line;
		while (getline(inputFile, line)) {
			client = convertLineToClientData(line);
			clients.emplace_back(client);
		}
	}
	return clients;
}

string createSeparatorLine(char character, int count) {
	return string(count, character);
}

void getValidAccountNumber(vector<ClientData>& clients, short& index) {
	string accountNumber;
	cout << "Client Account: ";
	while (getline(cin >> ws, accountNumber) && (index = findClientIndexByAccountNumber(clients, accountNumber)) == string::npos) {
		displayErrorMessage("Account Not Found!!");
		cout << "Client Account: ";
	}
}

void printHeader(string message) {
	cout << setw(35) << setfill('-') << '-' << endl;
	cout << createSeparatorLine('\t', 1) << message << endl;
	cout << setw(35) << setfill('-') << '-' << "\n\n";
}

void displayClientData() {
	printHeader("Find Client Screen");
	vector<ClientData> clients = loadClientsFromFile();
	short index;
	getValidAccountNumber(clients, index);
	printClientData(clients.at(index));
}

void saveClientDataToFile(vector<ClientData>& clients, ios_base::openmode mode = ios::out) {
	vector<string> clientDataStr;
	serializeClientData(clients, clientDataStr);
	ofstream outputFile("Bank.txt", mode);
	if (outputFile) {
		for (auto& client : clientDataStr) {
			outputFile << client << endl;
		}
	}
}

void deleteClientAccount() {
	printHeader("Delete Client Screen");
	vector<ClientData> clients = loadClientsFromFile();
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
	cout << "Enter Name: ";
	cin.ignore(INT_MAX, '\n');
	getline(cin >> ws, client.name);
	cout << "Enter Phone: ";
	getline(cin >> ws, client.phoneNumber);
	getValidPositiveNumber(client.balance, "Enter Account Balance: ");
}

void updateClientData() {
	printHeader("Update Client Info Screen");
	vector<ClientData> clients = loadClientsFromFile();
	short index;
	getValidAccountNumber(clients, index);
	printClientData(clients.at(index));
	if (getUserApproval("Are You Sure You Want Update?: ")) {
		editClientData(clients.at(index));
		saveClientDataToFile(clients);
	}
}

void displayAllClientRecords(ifstream& inputFile) {
	string line;
	for (short i = 1; getline(inputFile, line); i++) {
		vector<string> parsedData = parseClientData(line);
		cout << "| " << setw(20) << left << to_string(i) + '.' + parsedData.at(0);
		cout << "| " << setw(15) << left << parsedData.at(1);
		cout << "| " << setw(30) << left << parsedData.at(2);
		cout << "| " << setw(15) << left << parsedData.at(3);
		cout << "| " << setw(29) << left << parsedData.at(4) << '|' << endl;
	}
}

void printClientListHeader() {
	int clientCount = countFileLines(FILE_NAME);
	cout << createSeparatorLine('\t', 6) << "Client List (" << clientCount << ") Client(s). \n\n";
	cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
	cout << "| " << setw(20) << left << " Account Number";
	cout << "| " << setw(15) << left << " Pin Code";
	cout << "| " << setw(30) << left << " Client Name";
	cout << "| " << setw(15) << left << " Phone";
	cout << "| " << setw(29) << left << " Balance" << '|' << endl;
	cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
}

void displayClientList() {
	ifstream inputFile(FILE_NAME, ios::in);
	if (inputFile) {
		printClientListHeader();
		displayAllClientRecords(inputFile);
		cout << setw(120) << setfill('-') << '-' << "\n";
	}
}

string enterNewAccountNumber() {
	vector<ClientData> clients = loadClientsFromFile();
	string accountNumber;
	while (getline(cin >> ws, accountNumber) && findClientIndexByAccountNumber(clients, accountNumber) != string::npos) {
		displayErrorMessage("This account already exists!!");
		cout << "Enter another account Number: ";
	}
	return accountNumber;
}

void addNewClients() {
	short numberOfClients;
	getValidPositiveNumber(numberOfClients, "\n How many clients do you want to add? \n");
	static int clientCounter = 1;
	for (int i = 0; i < numberOfClients; i++) {
		vector<ClientData> newClients(1, { "", 0, "", "", 0.0 });
		cout << "\n Enter Data of " << (clientCounter++) << ".Person \n\n";
		cout << "Enter Account Number: ";
		newClients.at(0).accountNumber = enterNewAccountNumber();
		getValidPositiveNumber(newClients.at(0).pinCode, "Enter Pin Code: ");
		cout << "Enter Name: ";
		cin.ignore(INT_MAX, '\n');
		getline(cin >> ws, newClients.at(0).name);
		cout << "Enter Phone: ";
		getline(cin >> ws, newClients.at(0).phoneNumber);
		getValidPositiveNumber(newClients.at(0).balance, "Enter Account Balance: ");
		saveClientDataToFile(newClients, ios::app);
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

short getMainMenuChoice() {
	short choice;
	displayMainMenu();
	getValidPositiveNumber(choice, "Choice: ");
	return choice;
}

void deposit() {
	printHeader("Deposit Screen");
	vector<ClientData> clients = loadClientsFromFile();
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
	vector<ClientData> clients = loadClientsFromFile();
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
	cout << setw(40) << setfill('=') << '=' << setfill(' ') << "\n";
	cout << createSeparatorLine('\t', 1) << "Transaction Menu Screen" << "\n";
	cout << setw(40) << setfill('=') << '=' << setfill(' ') << "\n";
	cout << " [1]. Deposit" << endl;
	cout << " [2]. Withdraw" << endl;
	cout << " [3]. Total Balance" << endl;
	cout << " [4]. Exit" << endl;
	cout << setw(40) << setfill('=') << '=' << setfill(' ') << "\n";
}

short getTransactionChoice() {
	short choice;
	displayTransactionMenu();
	getValidPositiveNumber(choice, "Choose One: ");
	return choice;
}

void printTotalBalanceHeader() {
	int clientCount = countFileLines(FILE_NAME);
	cout << createSeparatorLine('\t', 6) << "Client List (" << clientCount << ") Client(s). \n\n";
	cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
	cout << "| " << setw(30) << left << " Account Number";
	cout << "| " << setw(44) << left << " Client Name";
	cout << "| " << setw(39) << left << " Balance" << '|' << endl;
	cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
}

void displayClientBalances() {
	string line;
	ifstream inputFile(FILE_NAME, ios::in);
	for (short i = 1; getline(inputFile, line); i++) {
		vector<string> parsedData = parseClientData(line);
		cout << "| " << setw(30) << left << to_string(i) + '.' + parsedData.at(0);
		cout << "| " << setw(44) << left << parsedData.at(2);
		cout << "| " << setw(39) << left << parsedData.at(4) << '|' << endl;
	}
}

float calculateTotalBalance() {
	float total = 0;
	vector<ClientData> clients = loadClientsFromFile();
	for (int i = 0; i < clients.size(); i++) {
		total += clients.at(i).balance;
	}
	return total;
}

void printTotalBalance() {
	float total = calculateTotalBalance();
	cout << setw(64) << ' ' << "Total Balance = " << total << endl;
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

int main() {
	runBankingSystem();
	return 0;
}
