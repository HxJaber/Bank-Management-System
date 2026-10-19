#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cstdlib>

using namespace std;

const string ClientsFileName = "Clients.txt";

struct stClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
};

enum enMainMenuOptions
{
	eShowClientList = 1,
	eAddNewClient = 2,
	eDeleteClient = 3,
	eUpdateClientInfo = 4,
	eFindClient = 5,
	eTransactionsOptions = 6,
	eExit = 7
};

enum enTransactionsOptions
{
	Deposit = 1,
	Withdraw = 2,
	TotalBalances = 3,
	MainMenu = 4
};

string ReadClientAccountNumber()
{
	string S1 = "";
	cout << "Please Enter Account Number : ";
	getline(cin >> ws, S1);

	return S1;
}

short ReadTransactionOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do? [1 to 4] : ";
	cin >> Choice;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(10000, '\n');

		cout << "\nInvalid input, Please enter a number between [ 1 to 4 ] : ";
		cin >> Choice;
	}
	return Choice;
}

short ReadMainMenuOption()
{
	short Choice = 0;
	cout << "Choose what do you want to do? [1 to 7] : ";
	cin >> Choice;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(10000, '\n');

		cout << "\nInvalid input, Please enter a number between [ 1 to 7 ] : ";
		cin >> Choice;
	}
	return Choice;
}

bool FindClientByAccountNumber(const string& AccountNumber, const vector <stClient>& vClients, stClient& sClient)
{
	for (const stClient& Client : vClients)
	{
		if (Client.AccountNumber == AccountNumber)
		{
			sClient = Client;
			return true;
		}
	}
	return false;
}

bool ClientExistsByAccountNumber(const string& AccountNumber, const vector <stClient>& vClient)
{
	stClient sClient;

	return  FindClientByAccountNumber(AccountNumber, vClient, sClient);
}

stClient ReadNewClient(const vector  <stClient>& vClients)
{
	stClient Client;

	cout << "Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, vClients))
	{
		cout << "\nClient With [" << Client.AccountNumber << "] is already exist";
		cout << "\nPlease enter another number :";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> ws >> Client.AccountBalance;

	return Client;
}

vector <string> SplitString(string S1, string Delim)
{
	vector<string> vWords;

	size_t Pos = 0;
	string sWord = "";

	while ((Pos = S1.find(Delim)) != string::npos)
	{
		sWord = S1.substr(0, Pos);

		if (sWord != "")
		{
			vWords.push_back(sWord);
		}

		S1.erase(0, Pos + Delim.length());
	}

	if (S1 != "")
	{
		vWords.push_back(S1);
	}

	return vWords;
}

string ConvertRecordToLine(const stClient& Client, string Seperator = "#//#")
{
	string ClientRecord = "";

	ClientRecord += Client.AccountNumber + Seperator;
	ClientRecord += Client.PinCode + Seperator;
	ClientRecord += Client.Name + Seperator;
	ClientRecord += Client.Phone + Seperator;
	ClientRecord += to_string(Client.AccountBalance);

	return ClientRecord;
}

void AddClientDataToFile(string FileName, string Line)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << Line << endl;

		MyFile.close();
	}
	else
	{
		cout << "Error: Could not open file.\n";
	}
}

void AddNewClient(vector <stClient>& vClients)
{
	stClient Client = ReadNewClient(vClients);
	vClients.push_back(Client);
	AddClientDataToFile(ClientsFileName, ConvertRecordToLine(Client));
}

stClient ConvertLineToRecord(string Line, string Separator = "#//#")
{
	stClient Client;

	vector<string> vClient = SplitString(Line, Separator);

	if (vClient.size() < 5)
		return Client;

	Client.AccountNumber = vClient[0];
	Client.PinCode = vClient[1];
	Client.Name = vClient[2];
	Client.Phone = vClient[3];
	Client.AccountBalance = stod(vClient[4]);

	return Client;
}

vector<stClient> LoadDataFromFileToVector(string FileName)
{
	fstream MyFile;
	vector<stClient> ClientRecord;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{
			if (Line.empty())
				continue;
			Client = ConvertLineToRecord(Line);

			if (Client.AccountNumber.empty())
				continue;
			ClientRecord.push_back(Client);
		}

		MyFile.close();
	}

	return ClientRecord;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClients)
{
	for (stClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}

	return false;
}

vector<stClient> SaveClientsDataToFile(string FileName, vector<stClient>& vClients)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	string DataLine = "";
	if (MyFile.is_open())
	{
		for (stClient& C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}
	return vClients;
}

void PrintClientCard(const stClient& Client)
{
	cout << "\n\nThe following is the extracted client record:\n";

	cout << "\nAccount Number: " << Client.AccountNumber;
	cout << "\nPin Code: " << Client.PinCode;
	cout << "\nName: " << Client.Name;
	cout << "\nPhone: " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;

	cout << endl;
}

void PrintClientRecord(const stClient& Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintClientBalanceRecord(const stClient& Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(30) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

stClient ChangeBalance(double Amount, stClient Client)
{
	Client.AccountBalance += Amount;
	return Client;
}

stClient ChangeClientRecord(string AccountNumber)
{
	stClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "Enter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> ws >> Client.AccountBalance;

	return Client;
}

bool UpdateClientByAccountNumber(const string& AccountNumber, vector <stClient>& vClients)
{
	stClient Client;
	char answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		cout << "\nAre you sure you want to Update this client? ";
		cin >> answer;
		if (answer == 'y' || answer == 'Y')
		{
			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientsDataToFile(ClientsFileName, vClients);
			cout << "\nClient Updated Successfully\n";
			return true;
		}
		else
		{
			cout << "\nUpdate Canceled!\n";
			return false;
		}
	}
	else
	{
		cout << "\nClient with account number (" << AccountNumber << ") is NOT found\n";
		return false;
	}
}

bool DeleteClientByAccountNumber(const string& AccountNumber, vector <stClient>& vClients)
{
	stClient Client;
	char answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		cout << "\nAre you sure you want to delete this client? ";
		cin >> answer;
		if (answer == 'y' || answer == 'Y')
		{
			if (MarkClientForDeleteByAccountNumber(AccountNumber, vClients))
			{
				SaveClientsDataToFile(ClientsFileName, vClients);
				vClients = LoadDataFromFileToVector(ClientsFileName);

				cout << "\n\nClient Deleted Successfully\n";
				return true;
			}
		}
		else
		{
			cout << "\nDeletion Canceled!\n";
			return false;
		}

		return false;
	}
	else
	{
		cout << "\nClient with account number (" << AccountNumber << ") is NOT found\n";
		return false;
	}
}

void ShowMainMenu()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "===========================================\n";
}

void GoBackToMenu(const string& MenuName)
{
	cout << "\nPress any key to go back to " << MenuName << " menu ... ";
	system("pause > nul");
}

void PrintAllClientsData(const vector <stClient>& vClients)
{
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In The System!";
	}
	else
	{
		for (const stClient& Client : vClients)
		{
			PrintClientRecord(Client);
			cout << endl;
		}
		cout <<
			"\n_______________________________________________________";
		cout << "_________________________________________\n" << endl;
	}
}

void FindClient(const string& AccountNumber, const vector<stClient>& vClients)
{
	stClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nClient with account number (" << AccountNumber << ") is NOT found\n";
	}
}

void ShowEndScreen()
{
	system("cls");
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-)\n";
	cout << "-----------------------------------\n";
}

void ShowDeleteClientScreen(vector<stClient>& vClients)
{
	system("cls");
	cout << "========================================\n";
	cout << "\tDelete Client Screen\n";
	cout << "========================================\n";
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen(vector<stClient>& vClients)
{
	system("cls");
	cout << "========================================\n";
	cout << "\tUpdate Client Screen\n";
	cout << "========================================\n";
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowAddNewClientsScreen(vector <stClient>& vClients)
{
	char AddMore = 'Y';
	do
	{
		system("cls");
		cout << "========================================\n";
		cout << "\tAdding Client Screen\n";
		cout << "========================================\n";

		cout << "Adding New Client:\n";
		AddNewClient(vClients);

		cout << "Client Added Successfully, Do you want to add more clients? (Y/N) :";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

void ShowClientListScreen(vector<stClient>& vClients)
{
	system("cls");
	cout << "========================================\n";
	cout << "\tClient List Screen\n";
	cout << "========================================\n";

	PrintAllClientsData(vClients);
}

void ShowTotalBalancesScreen(const vector <stClient>& vClients)
{
	system("cls");
	cout << "\n\t\t\tBalances List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "________\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(30) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "________\n" << endl;

	if (vClients.size() == 0)
	{
		cout << "\t\tNo Clients Available In The System!";
	}
	else
	{
		double SumTotalBalances = 0;

		for (const stClient& Client : vClients)
		{
			PrintClientBalanceRecord(Client);
			cout << endl;
			SumTotalBalances += Client.AccountBalance;
		}
		cout << "\n_______________________________________________________";
		cout << "________\n" << endl;

		cout << "\t\t\tTotalBalances = " << SumTotalBalances << endl;
	}
}

void ShowFindClientScreen(vector<stClient>& vClients)
{
	system("cls");
	cout << "========================================\n";
	cout << "\tFind Client Screen\n";
	cout << "========================================\n";
	string AccountNumber = ReadClientAccountNumber();
	FindClient(AccountNumber, vClients);
}

double ReadPositiveAmount(const string& Message)
{
	double Amount = 0;

	cout << Message;
	cin >> Amount;

	while (cin.fail() || Amount <= 0)
	{
		cin.clear();
		cin.ignore(10000, '\n');

		cout << "\nInvalid amount, Please enter a number greater than 0 : ";
		cin >> Amount;
	}

	return Amount;
}

bool TransactionByAccountNumber(const string& AccountNumber, vector<stClient>& vClients, enTransactionsOptions Type)
{
	stClient Client;
	char answer = 'n';
	double Amount = 0;

	string OperationName = (Type == enTransactionsOptions::Deposit) ? "Deposit" : "Withdraw";

	if (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with account number (" << AccountNumber << ") is NOT found\n";
		return false;
	}

	PrintClientCard(Client);

	if (Type == enTransactionsOptions::Withdraw && Client.AccountBalance <= 0)
	{
		cout << "\nYour balance is 0, you can't withdraw.\n";
		return false;
	}

	Amount = ReadPositiveAmount("\nPlease Enter " + OperationName + " Amount : ");


	if (Type == enTransactionsOptions::Withdraw)
	{
		while (Amount > Client.AccountBalance)
		{
			cout << "\nAmount exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
			Amount = ReadPositiveAmount("Please  enter another  amount : ");
		}

		Amount = Amount * -1;
	}

	cout << "\nAre you sure you want to perform this transaction ? (Y/N) : ";
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{

		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C = ChangeBalance(Amount, C);
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\n" << OperationName << " Done successfully, New balance is : " << C.AccountBalance << endl;
				return true;
			}
		}

	}

	cout << "\n" << OperationName << " Canceled!\n";
	return false;

}

void ShowTransactionScreen(vector<stClient>& vClients, enTransactionsOptions Type)
{
	system("cls");
	cout << "========================================\n";
	cout << "\t" << ((Type == enTransactionsOptions::Deposit) ? "Deposit" : "Withdraw") << " Screen\n";
	cout << "========================================\n";

	string AccountNumber = ReadClientAccountNumber();
	TransactionByAccountNumber(AccountNumber, vClients, Type);
}

void ShowTransactionMenu()
{
	system("cls");
	cout << "========================================\n";
	cout << "\tTransactions Menu Screen\n";
	cout << "========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] MainMenu.\n";
	cout << "===========================================\n";

}

void PerformTransactionsMenuOption(enTransactionsOptions Option, vector<stClient>& vClients)
{
	switch (Option)
	{
	case enTransactionsOptions::Deposit:
		ShowTransactionScreen(vClients, enTransactionsOptions::Deposit);
		GoBackToMenu("Transactions");
		break;

	case enTransactionsOptions::Withdraw:
		ShowTransactionScreen(vClients, enTransactionsOptions::Withdraw);
		GoBackToMenu("Transactions");
		break;

	case enTransactionsOptions::TotalBalances:
		ShowTotalBalancesScreen(vClients);
		GoBackToMenu("Transactions");
		break;

	default:
		break;
	}
}


void ShowTransactionsScreen(vector<stClient>& vClients)
{
	short Choice = 0;

	do
	{
		ShowTransactionMenu();
		Choice = ReadTransactionOption();
		PerformTransactionsMenuOption((enTransactionsOptions)Choice, vClients);

	} while (Choice != enTransactionsOptions::MainMenu);
}


void PerformMainMenuOption(enMainMenuOptions Option, vector<stClient>& vClients)
{
	switch (Option)
	{
	case enMainMenuOptions::eShowClientList:
		ShowClientListScreen(vClients);
		GoBackToMenu("Main");
		break;

	case enMainMenuOptions::eAddNewClient:
		ShowAddNewClientsScreen(vClients);
		GoBackToMenu("Main");
		break;

	case enMainMenuOptions::eDeleteClient:
		ShowDeleteClientScreen(vClients);
		GoBackToMenu("Main");
		break;

	case enMainMenuOptions::eUpdateClientInfo:
		ShowUpdateClientScreen(vClients);
		GoBackToMenu("Main");
		break;

	case enMainMenuOptions::eFindClient:
		ShowFindClientScreen(vClients);
		GoBackToMenu("Main");
		break;

	case enMainMenuOptions::eTransactionsOptions:
		ShowTransactionsScreen(vClients);
		break;

	case enMainMenuOptions::eExit:
		ShowEndScreen();
		break;

	default:
		break;
	}
}

void Start()
{
	vector<stClient> vClients;
	vClients = LoadDataFromFileToVector(ClientsFileName);

	short Choice = 0;

	do
	{
		ShowMainMenu();
		Choice = ReadMainMenuOption();

		PerformMainMenuOption((enMainMenuOptions)Choice, vClients);

	} while (Choice != eExit);
}

int main()
{
	Start();

	return 0;
}
