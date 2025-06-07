
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <conio.h> // _getch


// Constants for file names and delimiters
const string DELIMITER = "#//#";
const string CLIENT_FILE_NAME = "Bank.txt";
const string USER_FILE_NAME = "users.txt";

// Enum for main menu options
enum MainMenuOption { Add = 1, View, Find, Delete, Update, Transaction, Manage, LogOut };
typedef MainMenuOption ManageUserMenuOption; // Fixed naming convention

// Enum for transaction menu options
enum TransactionMenuOption { Deposit = 1, Withdraw, TotalBalance };

// Structure to hold user information
struct UserInfo {
	string user_name{};
	string user_password{};  // Fixed spelling
	short total_access{};
};

// Structure to hold client data
struct ClientData {
	string account_number;
	unsigned short pin_code;
	string name;
	string phone_number;
	float balance;
};

// Gets a valid positive float from user input
float getValidPositiveNumber(string message) {
	float number{};
	cout << message;
	while (!(cin >> number) || number < 1) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Enter a valid positive number: ";
	}
	return number;
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

// Converts client data structure to string format for file storage
string serializeClientData(ClientData& client) {
	string client_data_str{};
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

// Splits a string record into individual fields using delimiter
vector<string> parseRecordData(string client_line) {
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
void printClientData(vector<string>& client) {
	cout << "\n\n";
	cout << setw(30) << setfill('-') << '-' << endl;
	cout << "Account Number: " << client.at(0) << endl;
	cout << "Pin Code: " << client.at(1) << endl;
	cout << "Name: " << client.at(2) << endl;
	cout << "Phone Number: " << client.at(3) << endl;
	cout << "Balance: " << client.at(4) << endl;
	cout << setw(30) << setfill('-') << '-' << "\n\n";
}

// Gets a string input from the user
string getStringInput(string message) {
	string input;
	cout << message;
	getline(cin >> ws, input);
	return input;
}

// Searches for a client by account number and returns the index
short findValidAccount(vector<string>& file_data, string account_number) {
	for (short i{}; i < file_data.size(); i++) {
		vector <string> parsed_data = parseRecordData(file_data.at(i));
		if (parsed_data.front() == account_number) return i; // account_name in first the line
	}
	return -1;
}

// Converts file lines to ClientData structures
vector<ClientData> convertLineToClientData(const vector<string>& file_data) {
	vector<ClientData> clients{};
	for (int i{}; i < file_data.size(); i++) {
		vector<string> client_fields = parseRecordData(file_data.at(i));

		clients.push_back({ client_fields.at(0) ,(unsigned short)stoi(client_fields.at(1)),
		client_fields.at(2), client_fields.at(3), stof(client_fields.at(4)) });// struct members
	}
	return clients;
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
vector<string> loadDataFromFile(const string file_name) {
	ifstream input_file(file_name, ios::in);
	vector<string> clients{};
	if (input_file) {
		string line;
		while (getline(input_file, line)) {
			clients.push_back(line);
		}
	}
	return clients;
}

// Creates a separator line with a character
string createSeparatorLine(char character, int count) {
	return string(count, character);
}

// Gets a valid account number from user
string getValidAccountNumber(vector<string>& file_data) {
	string account_number;
	short required_account{};

	while (true) {
		cout << "Record Account: ";
		getline(cin >> ws, account_number);
		required_account = findValidAccount(file_data, account_number);
		if (required_account != string::npos) {
			break;
		}
		displayErrorMessage("Account Not Found!!");
	}

	return file_data.at(required_account);
}

// Prints a header with a message
void printHeader(const string& message, short number_letter = 1) {
	cout << setw(35) << setfill('-') << '-' << endl;
	cout << createSeparatorLine(' ', number_letter) << message << endl;
	cout << setw(35) << setfill('-') << '-' << "\n\n";
}

// Displays Client data by account number
void displayClientData() {
	printHeader("Find Client Screen", 8);

	vector<string> clients = loadDataFromFile(CLIENT_FILE_NAME);

	string client_str{ getValidAccountNumber(clients) };
	vector<string>parsed_data_client{ parseRecordData(client_str) };

	printClientData(parsed_data_client);
}

// Saves client data to the file
void saveRecordDataToFile(string file_name, string& record_data_str) {
	ofstream output_file(file_name, ios::app);
	if (output_file) output_file << record_data_str << endl;
}

// Clears all contents of a file
void deleteFileContents(const string file_name) {
	ofstream output_file(file_name, ios::out | ios::trunc);
}

// Deletes a client account from the system
void deleteClientAccount() {
	printHeader("Delete Client Screen", 8);

	vector<string> records = loadDataFromFile(CLIENT_FILE_NAME);

	string client_str{ getValidAccountNumber(records) };
	vector<string>parsed_data{ parseRecordData(client_str) };
	printClientData(parsed_data);

	short required_account{ findValidAccount(records, parsed_data.front()) };
	vector<ClientData>clients{ convertLineToClientData(records) };

	if (getUserApproval("Are You Sure You Want Delete?: ")) {
		clients.erase(clients.begin() + required_account);
		deleteFileContents(CLIENT_FILE_NAME);

		for (size_t i = 0; i < clients.size(); i++) {
			string client{ serializeClientData(clients.at(i)) };
			saveRecordDataToFile(CLIENT_FILE_NAME, client);
		}
	}
}

// Edits client data with user input
void editClientData(ClientData& client) {
	client.pin_code = getValidPositiveNumber("Enter Pin Code: ");
	cout << "Enter Name: ";
	cin.ignore(INT_MAX, '\n');
	getline(cin >> ws, client.name);
	cout << "Enter Phone: ";
	getline(cin >> ws, client.phone_number);
	client.balance = getValidPositiveNumber("Enter Account Balance: ");
}

// Updates client data in the system
void updateClientData() {
	printHeader("Update Client Info Screen", 6);

	vector<string> records = loadDataFromFile(CLIENT_FILE_NAME);

	string client_str{ getValidAccountNumber(records) };
	vector<string>parsed_data{ parseRecordData(client_str) };
	printClientData(parsed_data);

	short required_account{ findValidAccount(records, parsed_data.front()) };

	vector<ClientData> clients{ convertLineToClientData(records) };

	if (getUserApproval("Are You Sure You Want Update?: ")) {
		editClientData(clients.at(required_account));
		deleteFileContents(CLIENT_FILE_NAME);
		for (size_t i = 0; i < clients.size(); i++) {
			string client{ serializeClientData(clients.at(i)) };
			saveRecordDataToFile(CLIENT_FILE_NAME, client);
		}
	}
}

// Displays all client records from the file
void displayAllClientRecords(ifstream& input_file) {
	string line;
	for (short i = 1; getline(input_file, line); i++) {
		vector<string> parsed_data = parseRecordData(line);
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
string enterNewAccountNumber(const string file_name) {
	vector<string> records = loadDataFromFile(file_name);
	string account_number{};

	while (true) {
		cout << "Record Account: ";
		getline(cin >> ws, account_number);
		short pos = findValidAccount(records, account_number);
		if (pos == -1) {
			break;
		}
		displayErrorMessage("This account already exists!!");
	}
	return account_number;
}

// Adds new clients to the system
void addNewClients() {
	short number_of_clients = getValidPositiveNumber("\nHow many clients do you want to add: ");
	static int client_counter = 1;

	for (int i = 0; i < number_of_clients; i++) {
		ClientData new_client{};
		cout << "\nPerson." << (client_counter++) << ' ';

		new_client.account_number = enterNewAccountNumber(CLIENT_FILE_NAME);
		new_client.pin_code = getValidPositiveNumber("Enter Pin Code: ");

		cout << "Enter Name: ";
		cin.ignore(INT_MAX, '\n');
		getline(cin >> ws, new_client.name);

		cout << "Enter Phone: ";
		getline(cin >> ws, new_client.phone_number);

		new_client.balance = getValidPositiveNumber("Enter Account Balance: ");
		string client{ serializeClientData(new_client) };
		saveRecordDataToFile(CLIENT_FILE_NAME, client);
	}
	while (getUserApproval("Do You Need To Append More Clients? ")) return addNewClients();
}

// Adds a new client via the interface
void addNewClient() {
	printHeader("Add New Clients Screen", 7);
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
	cout << " [7]. Manage Users" << endl;
	cout << " [8]. Login Out" << endl;
	cout << " [9]. Exit" << endl;
	cout << setw(35) << setfill('=') << '=' << setfill(' ') << "\n\n";
}

// Gets the user's main menu choice
short getMainMenuChoice() {
	short choice = getValidPositiveNumber("Choice: ");
	return choice;
}

// Handles deposit operation
void deposit() {
	printHeader("Deposit Screen", 11);
	vector<string> records = loadDataFromFile(CLIENT_FILE_NAME);

	string client_str{ getValidAccountNumber(records) };
	vector<string>parsed_data{ parseRecordData(client_str) };
	printClientData(parsed_data);

	short required_account{ findValidAccount(records, parsed_data.front()) };
	vector<ClientData>clients = convertLineToClientData(records);

	if (getUserApproval("Are You Sure You Want Deposit?: ")) {
		float amount{ getValidPositiveNumber("Enter Value You need: ") };
		clients.at(required_account).balance += amount;
		deleteFileContents(CLIENT_FILE_NAME);
		for (size_t i = 0; i < clients.size(); i++) {
			string client{ serializeClientData(clients.at(i)) };
			saveRecordDataToFile(CLIENT_FILE_NAME, client);
		}
	}
}

// Handles withdraw operation
void withdraw() {
	printHeader("Withdraw Screen", 10);
	vector<string> records = loadDataFromFile(CLIENT_FILE_NAME);

	string client_str{ getValidAccountNumber(records) };
	vector<string>parsed_data{ parseRecordData(client_str) };
	printClientData(parsed_data);

	short required_account{ findValidAccount(records, parsed_data.front()) };
	vector<ClientData>clients = convertLineToClientData(records);

	if (getUserApproval("Are You Sure You Want Withdraw?: ")) {
		float amount{};
		do {
			amount = getValidPositiveNumber("Withdraw an amount not exceeding the balance: ");
		} while (amount > clients.at(required_account).balance);
		clients.at(required_account).balance -= amount;
		deleteFileContents(CLIENT_FILE_NAME);
		for (size_t i = 0; i < clients.size(); i++) {
			string client{ serializeClientData(clients.at(i)) };
			saveRecordDataToFile(CLIENT_FILE_NAME, client);
		}
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
	displayTransactionMenu();
	short choice = getValidPositiveNumber("Choose One: ");
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
		vector<string> parsed_data = parseRecordData(line);
		cout << "| " << setw(30) << left << to_string(i) + '.' + parsed_data.at(0);
		cout << "| " << setw(44) << left << parsed_data.at(2);
		cout << "| " << setw(39) << left << parsed_data.at(4) << '|' << endl;
	}
}

// Calculates total balance of all clients
float calculateTotalBalance() {
	float total = 0;
	vector<string> records = loadDataFromFile(CLIENT_FILE_NAME);
	vector<ClientData>clients = convertLineToClientData(records);

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

// Converts file data to UserInfo vector
vector<UserInfo> convertToUserVector(vector<string>& file_data) {
	vector<UserInfo> users{};
	for (int i{}; i < file_data.size(); i++) {
		vector<string>parsed_data{ parseRecordData(file_data.at(i)) };
		users.push_back({ parsed_data.at(0), parsed_data.at(1),(short)stoi(parsed_data.at(2)) });
	}
	return users;
}

// Converts string data to UserInfo struct
UserInfo convertToUserStruct(string& user_data) {
	UserInfo user{};
	vector<string>parsed_data{ parseRecordData(user_data) };
	user = { parsed_data.at(0), parsed_data.at(1),(short)stoi(parsed_data.at(2)) };
	return user;
}

// Gets user account from file data
UserInfo getUserAccount(vector <string>& file_data) {
	string user_string{ getValidAccountNumber(file_data) };
	return convertToUserStruct(user_string);
}

// Validates user password input
string getCorrectPassword(UserInfo user) {
	string password{};
	while (true) {
		cout << "Enter password: ";
		getline(cin >> ws, password);
		if (password == user.user_password) {
			break;
		}
		displayErrorMessage("Incorrect Password!!");
	}
	return password;
}

// Handles user login process
UserInfo login() {
	printHeader("Login Secreen", 11);
	vector<string> file_data{ loadDataFromFile(USER_FILE_NAME) };
	UserInfo user{};

	// read user name
	user = getUserAccount(file_data);

	// read user password
	user.user_password = getCorrectPassword(user);

	return user;
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

// Displays the manage users menu
void displayManageMenu() {
	cout << setw(35) << setfill('=') << '=' << setfill(' ') << "\n";
	cout << createSeparatorLine(' ', 6) << "---Manage Users Menu---" << "\n";
	cout << setw(35) << setfill('=') << '=' << setfill(' ') << "\n";
	cout << " [1]. Add New User" << endl;
	cout << " [2]. View User List" << endl;
	cout << " [3]. Find Data Of User" << endl;
	cout << " [4]. Delete User" << endl;
	cout << " [5]. Update User's Data" << endl;
	cout << " [6]. Main Menu" << endl;
	cout << setw(35) << setfill('=') << '=' << setfill(' ') << "\n\n";
}

// Gets access permissions for a new user
short getAccessForUser() {
	short access_total{};

	if (getUserApproval("Do You Want To Give Full Access: "))
		return-1;

	cout << "Do you want to give access to : \n";

	if (getUserApproval("Add Client: "))
		access_total |= 0x01;

	if (getUserApproval("Show Client: "))
		access_total |= 0x02;

	if (getUserApproval("Find Client: "))
		access_total |= 0x04;

	if (getUserApproval("Delete Client: "))
		access_total |= 0x08;

	if (getUserApproval("Update Client: "))
		access_total |= 0x10;

	if (getUserApproval("Transactions: "))
		access_total |= 0x20;

	if (getUserApproval("Manage Users: "))
		access_total |= 0x40;

	return access_total;
}

// Converts UserInfo struct to string format for file storage
string serializeUserToStr(UserInfo& user) {
	user.user_name += DELIMITER;
	user.user_password += DELIMITER;
	return user.user_name + user.user_password + to_string(user.total_access);
}

// Adds a new user to the system
void addUser() {
	UserInfo user{};
	printHeader("Add New User", 14);
	do {
		user.user_name = enterNewAccountNumber(USER_FILE_NAME);

		cout << "Enter Password: ";
		getline(cin >> ws, user.user_password);

		user.total_access = getAccessForUser();

		string userStr{ serializeUserToStr(user) };
		saveRecordDataToFile(USER_FILE_NAME, userStr);

		cout << "User Added Successfully,";
	} while (getUserApproval("Do you want add more: "));
}

// Prints header for user list display
void printUserListHeader() {
	int user_count = countFileLines(USER_FILE_NAME);
	cout << createSeparatorLine('\t', 6) << "User List (" << user_count << ") User(s). \n\n";
	cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
	cout << "| " << setw(40) << left << " User Name";
	cout << "| " << setw(60) << left << " Password";
	cout << "| " << setw(13) << left << " Access" << '|' << endl;
	cout << setw(120) << setfill('-') << '-' << setfill(' ') << endl;
}

// Displays all users records from file
void displayAllUsersRecords(ifstream& input_file) {
	string line;
	for (short i = 1; getline(input_file, line); i++) {
		vector<string> parsed_data = parseRecordData(line);
		cout << "| " << setw(40) << left << to_string(i) + '.' + parsed_data.at(0);
		cout << "| " << setw(60) << left << parsed_data.at(1);
		cout << "| " << setw(13) << left << parsed_data.at(2) << '|' << endl;
	}
}

// Displays the list of all users
void displayUserList() {
	ifstream input_file{ USER_FILE_NAME, ios::in };
	if (input_file) {
		printUserListHeader();
		displayAllUsersRecords(input_file);
		cout << setw(120) << setfill('-') << '-' << "\n";
	}
}

// Prints user data in formatted way
void printUserData(vector<string>& user) {
	cout << "\n\n";
	cout << setw(40) << setfill('-') << '-' << endl;
	cout << "User Name: " << user.front() << endl;
	cout << "Password: " << user.at(1) << endl;
	cout << "Access: " << user.back() << endl;
	cout << setw(40) << setfill('-') << '-' << endl;
	cout << setfill(' '); // for give back the stream letter again
	cout << "\n\n";
}

// Displays User data by username
void displayUserData() {
	printHeader("Find User Screen", 9);

	vector<string> users = loadDataFromFile(USER_FILE_NAME);

	string user_str{ getValidAccountNumber(users) };

	vector<string>parsed_data_client{ parseRecordData(user_str) };

	printUserData(parsed_data_client);
}

// Checks if user is admin
bool checkIsAdmin(UserInfo user) {
	return user.user_name == "Admin";
}

// Deletes a user from the system
void deleteUser() {
	printHeader("Delete User Screen", 8);

	vector<string> records = loadDataFromFile(USER_FILE_NAME);

	string user_str{ getValidAccountNumber(records) };
	vector<string>parsed_data{ parseRecordData(user_str) };

	vector<UserInfo> users{ convertToUserVector(records) };
	short required_account{ findValidAccount(records, parsed_data.front()) };

	if (checkIsAdmin(users.at(required_account))) {
		displayErrorMessage("You cannot delete Admin!!");
		return;
	}
	printUserData(parsed_data);

	if (getUserApproval("Are You Sure You Want Delete?: ")) {
		users.erase(users.begin() + required_account);
		deleteFileContents(USER_FILE_NAME);

		for (size_t i = 0; i < users.size(); i++) {
			string user{ serializeUserToStr(users.at(i)) };
			saveRecordDataToFile(USER_FILE_NAME, user);
		}
	}
}

// Edits user data with new input
UserInfo editUserData(UserInfo& user) {
	cout << "Enter Password: ";
	getline(cin >> ws, user.user_password);

	user.total_access = getAccessForUser();

	return user;
}

// Updates user data in the system
void updateUserData() {
	printHeader("Update User Info Screen", 6);

	vector<string> records = loadDataFromFile(USER_FILE_NAME);

	string user_str{ getValidAccountNumber(records) };

	vector<string>parsed_data{ parseRecordData(user_str) };

	short required_account{ findValidAccount(records, parsed_data.front()) };

	vector<UserInfo> users{ convertToUserVector(records) };

	if (checkIsAdmin(users.at(required_account))) {
		displayErrorMessage("You cannot update Admin!!");
		return;
	}
	printUserData(parsed_data);

	if (getUserApproval("Are You Sure You Want Update?: ")) {
		users.at(required_account) = editUserData(users.at(required_account));
		deleteFileContents(USER_FILE_NAME);
		for (size_t i = 0; i < users.size(); i++) {
			string user{ serializeUserToStr(users.at(i)) };
			saveRecordDataToFile(USER_FILE_NAME, user);
		}
	}
}

// Manages user operations menu
void userManageList() {
	do {
		system("cls");
		displayManageMenu();
		ManageUserMenuOption choice = static_cast<ManageUserMenuOption>(getMainMenuChoice());
		switch (choice) {
		case ManageUserMenuOption::Add:
			system("cls");
			addUser();
			break;
		case ManageUserMenuOption::View:
			system("cls");
			displayUserList();
			break;
		case ManageUserMenuOption::Find:
			system("cls");
			displayUserData();
			break;
		case ManageUserMenuOption::Delete:
			system("cls");
			deleteUser();
			break;
		case ManageUserMenuOption::Update:
			system("cls");
			updateUserData();
			break;
		default:
			return;
		}
		cout << "\n\n";
		cout << setw(50) << setfill('+') << '+' << endl;
		cout << "Press any character to return to manage menu: " << endl;
		cout << setw(50) << setfill('+') << '+' << endl;
	} while (_getch());
}

// Checks if user has access to specific menu option
bool checkIfHasAccess(short total_access, short choice) {
	for (short i{ 0x40 }; i >= pow(2, choice - 1); i /= 2) {
		if (total_access == 0) return false;
		if (total_access >= i) total_access -= i;
	}
	return total_access == 0;
}

// Runs the main banking system
void runBankingSystem(const UserInfo& user) {
	do {
		system("cls");
		displayMainMenu();
		MainMenuOption choice = static_cast<MainMenuOption>(getMainMenuChoice());
		if (!(user.total_access == -1 || checkIfHasAccess(user.total_access, choice))) {
			cout << "You don't have access for this choice!!";
			continue;
		}
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
		case MainMenuOption::Manage:
			system("cls");
			userManageList();
			return runBankingSystem(user);
		case MainMenuOption::LogOut:
			system("cls");
			return runBankingSystem(login());
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

// Starts the banking application
void start() {
	runBankingSystem(login());
}

int main() {
	start();
	return 0;
}
