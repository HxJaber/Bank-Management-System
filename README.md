# Bank Management System (C++)

A console-based bank client management system written in C++, with persistent file storage and a transactions module.

## 🌟 Features

- **Client Management:** Show, add, delete, update, and find clients.
- **Transactions:** Deposit, withdraw, and view the total balances of all clients.
- **File Persistence:** All data is saved to `Clients.txt` and loaded on startup.
- **Input Validation:** Rejects non-numeric menu input, negative amounts, and withdrawals that exceed the balance.
- **Confirmation Prompts:** Asks for confirmation before update, delete, and transactions.

## 🧠 Concepts & Architecture Practiced

- **Data Encapsulation:** Client data grouped into a `struct` (`stClient`).
- **Strong Typing:** `enum` types for main menu and transactions menu options.
- **File Handling:** Reading and writing records with `fstream`, using a custom delimiter (`#//#`).
- **String Parsing:** Custom `SplitString` function to convert file lines into records.
- **Modular Design:** Separated menu display, option reading, and option execution (`Show...` / `Read...` / `Perform...`).
- **Code Reuse:** A single `TransactionByAccountNumber` function handles both deposit and withdraw.
- **Pass by Reference:** Used `const &` to avoid unnecessary copies of the clients vector.
- **Soft Delete:** Clients are marked for deletion, then removed when the file is saved.

## 🛠️ Technologies

- **Language:** C++
- **Environment:** Windows Console CLI

> ⚠️ **Platform Note:** This project uses Windows-specific commands (`system("cls")` and `system("pause")`). It will compile on other platforms, but screen clearing and the pause prompt will not work outside of Windows.

## 🚀 How to Build & Run

1. Make sure you have a C++ compiler installed (e.g., **MinGW / g++**, or **Visual Studio**).
2. Compile the source file:

```
g++ main.cpp -o bank.exe
```

3. Run the generated executable from the same folder as `Clients.txt`:

```
bank.exe
```

## 📄 Data Format

Each client is stored on one line, separated by `#//#`:

```
AccountNumber#//#PinCode#//#Name#//#Phone#//#AccountBalance
```

A sample `Clients.txt` with dummy data is included.

## ⚠️ Known Limitations

This is a learning project. PIN codes are stored as plain text, and there is no login screen.

## 🎯 Purpose

Created as part of my C++ learning journey to practice file handling, modular design, and writing clean, maintainable console applications.
