# 🏦 Bank Management System - Advanced C++ Console Application
A comprehensive, secure, and efficient banking system for managing client accounts, user permissions, and financial transactions with enterprise-grade features.

## 📌 Table of Contents
- [Features](#-features)
- [Installation](#️-installation)
- [Usage](#️-usage)
- [System Architecture](#️-system-architecture)
- [Code Structure](#-code-structure)
- [Screenshots](#-screenshots)
- [Security Features](#-security-features)
- [Contributing](#-contributing)
- [License](#-license)

## ✨ Features

### 🔐 Dual Interface System
- **Client ATM Interface**: Self-service banking with PIN authentication
- **User Management Interface**: Administrative control with role-based permissions
- **Secure Login System**: Separate authentication for clients and administrative users
- **Session Management**: Logout and re-authentication capabilities

### 👥 Comprehensive Account Management
- **Full CRUD Operations**: Add/View/Update/Delete client accounts
- **Secure PIN Authentication**: Multi-layer security with PIN codes for clients
- **Unique Account Generation**: Automatic validation for duplicate prevention
- **Smart Data Validation**: Input sanitization and error handling

### 💰 Advanced Transaction Processing
- **Client ATM Operations**: 
  - Quick Withdraw (predefined amounts: $20, $50, $100, $200, $400, $800, $1600, $4000)
  - Normal Withdraw with custom amounts
  - Deposit functionality
  - Balance inquiry
- **Administrative Transactions**: Secure deposit/withdraw for any client
- **Balance Validation**: Real-time overdraft protection
- **Total Balance Calculation**: Aggregate financial reporting across all clients

### 🏧 ATM Interface Features
- **Quick Withdraw Menu**: 8 predefined withdrawal amounts
- **Custom Withdrawal**: User-specified amounts with balance validation
- **Real-time Balance Display**: Current balance shown during transactions
- **Transaction Confirmation**: User approval for all operations

### 👤 User Management System
- **Multi-level User Permissions**: Granular access control using bitwise operations
- **Admin Protection**: Built-in safeguards preventing admin account deletion/modification
- **Custom Permission Settings**: 7 different access levels:
  - Add Client (0x01)
  - Show Client (0x02)
  - Find Client (0x04)
  - Delete Client (0x08)
  - Update Client (0x10)
  - Transactions (0x20)
  - Manage Users (0x40)
  - Full Access (-1)

### 📂 Enterprise Data Management
- **Custom Serialization**: Efficient "#//#" delimiter-based storage
- **Dual File System**: 
  - `Bank.txt` for client data
  - `users.txt` for user credentials and permissions
- **Atomic File Operations**: Data integrity with transaction-safe writes
- **Recovery Support**: Robust error handling and data recovery

### 🎨 Modern Console Interface
- **Professional UI Design**: Clean, formatted console displays with borders
- **Interactive Menus**: Intuitive navigation with numbered options
- **Real-time Feedback**: Progress indicators and status messages
- **Responsive Design**: Different layouts for different menu types

## 🛠️ Installation

### Requirements
- **Compiler**: GCC/G++ (C++17 or later)
- **Platform**: Windows (for `conio.h` support)
- **Memory**: Minimum 512MB RAM
- **Storage**: 50MB free disk space

### Installation Steps
```bash
# Clone repository
git clone https://github.com/yourusername/advanced-bank-system.git
cd advanced-bank-system

# Compile with optimizations
g++ main.cpp -o bank_system -std=c++17 -O2

# Create data files (they will be created automatically on first run)
# Bank.txt - for client data
# users.txt - for user credentials

# Run the application
./bank_system
```

### First-Time Setup
1. Run the application
2. Choose between Client [1] or User [2] interface
3. For User interface, login with default admin credentials:
   - **Username**: `Admin`
   - **Password**: `1234`
4. Create additional user accounts as needed
5. Configure user permissions

## 🖥️ Usage

### Login Page Interface
```
                        ==============================          ==============================
                        ||           Client         ||          ||            User          ||
                        ==============================          ==============================

             ------------------------------------------------------------------------------------------
Choice [1]<-->[2]: 
```

### Client ATM Menu
```
===================================
        ATM Menu Screen
===================================
[1]. Quick Withdraw
[2]. Normal Withdraw
[3]. Deposit
[4]. Check Balance
[5]. LogOut
[6]. Exit
===================================
```

### Quick Withdraw Menu
```
===================================
        Quick Withdraw Screen
===================================
[1].20$                  [2].  50$
[3].100$                 [4]. 200$
[5].400$                 [6]. 800$
[7].1600$                [8].4000$
===================================
```

### User Main Menu
```
===================================

          User Main Menu

===================================
[1]. Add New Client
[2]. View Client List
[3]. Find Data Of Client
[4]. Delete Client
[5]. Update Client's Data
[6]. Transaction
[7]. Manage Users
[8]. Login Out
[9]. Exit
===================================
```

### User Management Menu
```
===================================
      ---Manage Users Menu---
===================================
[1]. Add New User
[2]. View User List
[3]. Find Data Of User
[4]. Delete User
[5]. Update User's Data
[6]. Main Menu
===================================
```

### Transaction Menu
```
========================================
        Transaction Menu Screen
========================================
[1]. Deposit
[2]. Withdraw
[3]. Total Balance
[4]. Exit
========================================
```

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    PRESENTATION LAYER                       │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────────────────┐│
│  │ Login Page  │ │ ATM Menu    │ │ Admin Menu              ││
│  │ Client/User │ │ (Clients)   │ │ (Users)                 ││
│  └─────────────┘ └─────────────┘ └─────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────┐
│                     BUSINESS LOGIC                          │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────────────────┐│
│  │ Client Auth │ │ User Auth   │ │ Permission System       ││
│  │ & ATM Ops   │ │ & Admin Ops │ │ (Bitwise Operations)    ││
│  └─────────────┘ └─────────────┘ └─────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────┐
│                      DATA LAYER                             │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────────────────┐│
│  │ Bank.txt    │ │ users.txt   │ │ Custom Serialization    ││
│  │ (Clients)   │ │ (Users)     │ │ (#//# Delimiter)        ││
│  └─────────────┘ └─────────────┘ └─────────────────────────┘│
└─────────────────────────────────────────────────────────────┘
```

## 📂 Code Structure

```
advanced-bank-system/
├── 📄 main.cpp                    # Complete application source code
├── 📄 Bank.txt                    # Client database file (auto-generated)
├── 📄 users.txt                   # User credentials and permissions (auto-generated)
└── 📄 README.md                   # This documentation
```

### Core Data Structures

| Structure | Description | Fields |
|-----------|-------------|---------|
| **ClientData** | Client account information | account_number, pin_code, name, phone_number, balance |
| **UserInfo** | User credentials and permissions | user_name, user_password, total_access |

### Key Enumerations

| Enum | Values | Purpose |
|------|--------|---------|
| **MainMenuOption** | Add, View, Find, Delete, Update, Transaction, Manage, LogOut | Admin menu choices |
| **TransactionMenuOption** | Deposit, Withdraw, TotalBalance | Transaction operations |
| **ClientMenuOption** | QuickWithdraw, NormalWithdraw, ClientDeposit, ShowBalance, ClientLogOut | ATM operations |
| **Members** | Client, User | Login interface selection |

### Core Functions

| Function | Purpose | Key Features |
|----------|---------|--------------|
| `loginPage()` | Main application entry point | Dual interface selection |
| `clientLogin()` | Client authentication | Account number + PIN verification |
| `userLogin()` | User authentication | Username + password verification |
| `quickWithdraw()` | ATM quick withdrawal | 8 predefined amounts |
| `normalWithdraw()` | ATM custom withdrawal | User-specified amounts |
| `checkIfHasAccess()` | Permission verification | Bitwise operations for access control |
| `serializeClientData()` | Data serialization | Custom delimiter format |
| `parseRecordData()` | Data deserialization | String parsing with delimiter |

## 📸 Screenshots

### Login Interface
```
Choice [1]<-->[2]: 1
```

### Client Account Display
```
------------------------------
Account Number: ACC001
Pin Code: 1234
Name: Ahmed Mohamed
Phone Number: +20123456789
Balance: 15750.50
------------------------------
```

### ATM Balance Display
```
Your Total Balance Now is: 15750.50
```

### Quick Withdraw Interface
```
===================================
        Quick Withdraw Screen
===================================
[1].20$                    [2].50$
[3].100$                  [4].200$
[5].400$                  [6].800$
[7].1600$                [8].4000$
===================================

Your Total Balance Now is: 15750.50

Choice one: 3
Successful, Your Balance Now Is: 15650.50
```

### User List Display
```
      Client List (5) Client(s).

------------------------------------------------------------------------------------------------------------------------
| Account Number          | Pin Code      | Client Name                  | Phone          | Balance                    |
------------------------------------------------------------------------------------------------------------------------
| 1.ACC001                | 1234          | Ahmed Mohamed                | +20123456789   | 15750.50                   |
| 2.ACC002                | 5678          | Fatma Ali                    | +20987654321   | 8200.75                    |
| 3.ACC003                | 9876          | Omar Hassan                  | +20555123456   | 12500.00                   |
------------------------------------------------------------------------------------------------------------------------
```

### Total Balance Summary
```
      Client List (3) Client(s).

------------------------------------------------------------------------------------------------------------------------
| Account Number                | Client Name                          | Balance                                 |
------------------------------------------------------------------------------------------------------------------------
| 1.ACC001                      | Ahmed Mohamed                        | 15750.50                                |
| 2.ACC002                      | Fatma Ali                            | 8200.75                                 |
| 3.ACC003                      | Omar Hassan                          | 12500.00                                |
------------------------------------------------------------------------------------------------------------------------
                                Total Balance = 36451.25
```

## 🔒 Security Features

### Authentication & Authorization
- **Dual Authentication System**: 
  - Client: Account Number + PIN Code
  - User: Username + Password
- **Role-based Access Control**: Bitwise permission system with 7 access levels
- **Admin Protection**: Built-in safeguards preventing admin account deletion/modification
- **Session Management**: Secure logout and re-authentication

### Data Protection
- **Input Validation**: Comprehensive sanitization of all user inputs
- **File Integrity**: Atomic write operations to prevent data corruption
- **Access Control**: Permission checks before sensitive operations
- **Balance Validation**: Overdraft protection in all withdrawal operations

### Permission System Details
```cpp
// Permission bits (can be combined)
0x01 - Add Client
0x02 - Show Client  
0x04 - Find Client
0x08 - Delete Client
0x10 - Update Client
0x20 - Transactions
0x40 - Manage Users
-1   - Full Access (Admin)
```

### Best Practices Implemented
- **Principle of Least Privilege**: Users get only necessary permissions
- **Data Validation**: All inputs validated before processing
- **Secure Storage**: Custom serialization with delimiter protection
- **Transaction Safety**: Real-time balance verification
- **Admin Safety**: Cannot delete or modify admin account

## 🤝 Contributing

We welcome contributions! Please follow these guidelines:

### Getting Started
1. **Fork** the repository
2. **Clone** your fork: `git clone https://github.com/yourusername/advanced-bank-system.git`
3. **Create** a feature branch: `git checkout -b feature/amazing-feature`
4. **Make** your changes
5. **Test** thoroughly with both client and user interfaces
6. **Commit**: `git commit -m 'Add amazing feature'`
7. **Push**: `git push origin feature/amazing-feature`
8. **Submit** a Pull Request

### Code Standards
- Follow C++ best practices and naming conventions
- Add comments for complex logic
- Include error handling for all operations
- Test with various input scenarios
- Update documentation for new features
- Maintain compatibility with Windows console

### Areas for Contribution
- 🔐 Enhanced security features
- 📊 Transaction history and reporting
- 🎨 UI/UX improvements
- 🗄️ Database integration (replace file storage)
- 🔧 Performance optimizations
- 📱 Cross-platform compatibility (remove conio.h dependency)
- 🌐 Network banking features
- 📧 Email notifications
- 📈 Analytics and insights

### Testing Guidelines
- Test both client and user login flows
- Verify permission system works correctly
- Test all ATM operations with various amounts
- Verify data persistence across sessions
- Test error handling with invalid inputs

---

**🔄 Last Updated**: December 2024 | **Status**: Active Development

**⭐ Star this repository if you find it helpful!**
