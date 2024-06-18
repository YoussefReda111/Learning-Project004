#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

enum en_main_menue_options
{
	en_quick_withdraw = 1,
	en_normal_withdraw,
	en_deposit,
	en_check_balance
};

enum en_quick_withdraw_options
{
	en_20 = 1, en_50,
	en_100, en_200,
	en_400, en_600,
	en_800, en_1000,
	en_exit
};

struct st_client
{
	string account_number, PIN_code;
	float account_balance = 0;
};

void save_vector_to_file(string file_name, vector <string>& vec_file_content)
{
	fstream my_file;
	my_file.open(file_name, ios::out);
	if (my_file.is_open())
	{
		for (string& line : vec_file_content)
			if (line != "")
				my_file << line << endl;
	}
	my_file.close();
}

void load_data_from_file_to_vector(string file_name, vector <string>& vec_file_content)
{
	fstream my_file;
	my_file.open(file_name, ios::in);
	if (my_file.is_open())
	{
		string line;
		while (getline(my_file, line))
			vec_file_content.push_back(line);
	}
	my_file.close();
}

void update_record_in_file(string file_name, string record, string update_to)
{
	vector <string> vec_file_content;
	load_data_from_file_to_vector(file_name, vec_file_content);
	for (string& line : vec_file_content)
		if (line == record)
			line = update_to;
	save_vector_to_file(file_name, vec_file_content);
}

string convert_client_record_to_line(st_client client_info, string separator)
{
	string line = client_info.account_number;
	line += separator + client_info.PIN_code;
	line += separator + to_string(client_info.account_balance);
	return line;
}

void get_client_info(st_client& client_data)
{
	cout << "\nEnter Account Number: ";
	cin >> client_data.account_number;
	cout << "Enter PIN Code: ";
	cin >> client_data.PIN_code;
}

void show_login_screen()
{
	cout << "------------------------------------------";
	cout << "\n\t\tLogin Screen";
	cout << "\n------------------------------------------\n";
}

void login(string clients_file, st_client& client)
{
	show_login_screen();
	get_client_info(client);
	vector <string> vec_clients_records;
	load_data_from_file_to_vector(clients_file, vec_clients_records);
	do
	{
		for (string& client_record : vec_clients_records)
			if (client_record.substr(0, 4) == client.account_number && client_record.substr(7, 4) == client.PIN_code)
			{
				client.account_balance = stof(client_record.substr(14));
				system("cls");
				return;
			}
		system("cls");
		show_login_screen();
		cout << "Invalid Username/Password!";
		get_client_info(client);
	} while (true);
}

void show_main_menue()
{
	cout << "==============================================================";
	cout << "\n\t\tATM Main Menue Screen";
	cout << "\n==============================================================";
	cout << "\n\t[1] Quick Withdraw.";
	cout << "\n\t[2] Normal Withdraw.";
	cout << "\n\t[3] Deposit.";
	cout << "\n\t[4] Check Balance.";
	cout << "\n\t[5] Logout.";
	cout << "\n==============================================================";
}

void show_quick_withdraw_screen()
{
	cout << "==========================================";
	cout << "\n\tQuick Withdraw Screen";
	cout << "\n==========================================";
	cout << "\n\t[1] 20\t\t[2] 50";
	cout << "\n\t[3] 100\t\t[4] 200";
	cout << "\n\t[5] 400\t\t[6] 600";
	cout << "\n\t[7] 800\t\t[8] 1000";
	cout << "\n\t[9] Exit";
	cout << "\n==========================================";
}

bool does_exceed_account_balance(en_quick_withdraw_options operation_number, float account_balance)
{
	return ((operation_number == en_20 && account_balance >= 20) ||
		    (operation_number == en_50 && account_balance >= 50) ||
		    (operation_number == en_100 && account_balance >= 100) ||
		    (operation_number == en_200 && account_balance >= 200) ||
		    (operation_number == en_400 && account_balance >= 400) ||
		    (operation_number == en_600 && account_balance >= 600) ||
		    (operation_number == en_800 && account_balance >= 800) ||
		    (operation_number == en_1000 && account_balance >= 1000));
}

void quick_withdraw(string clients_file, st_client& client) 
{
	short operation_number;
	char perform_or_not;
	string client_record = convert_client_record_to_line(client, "/#/");
	show_quick_withdraw_screen();
	cout << "\nYour Balance is " << client.account_balance;
	cout << "\nChoose what to withdraw from [1] to [8]: ";
	cin >> operation_number;
	if (operation_number >= 1 && operation_number <= 9)
	{
		if (does_exceed_account_balance((en_quick_withdraw_options)operation_number, client.account_balance))
		{
			cout << "\nAre you sure you want perform this transaction (Y/N)? ";
			cin >> perform_or_not;
			if (toupper(perform_or_not) == 'Y')
			{
				switch (operation_number)
				{
				case en_quick_withdraw_options::en_20: client.account_balance -= 20; break;
				case en_quick_withdraw_options::en_50: client.account_balance -= 50; break;
				case en_quick_withdraw_options::en_100: client.account_balance -= 100; break;
				case en_quick_withdraw_options::en_200: client.account_balance -= 200; break;
				case en_quick_withdraw_options::en_400: client.account_balance -= 400; break;
				case en_quick_withdraw_options::en_600: client.account_balance -= 600; break;
				case en_quick_withdraw_options::en_800: client.account_balance -= 800; break;
				case en_quick_withdraw_options::en_1000: client.account_balance -= 1000;
				}
				cout << "\nDone Successfully. New balance is: " << client.account_balance;
				string updated_client_record = convert_client_record_to_line(client, "/#/");
				update_record_in_file(clients_file, client_record, updated_client_record);
			}
			else cout << "\nThe transaction has not been performed.";

		}
		else if (operation_number == en_exit) return;
		else
		{
			cout << "\nThe amount exceeds your balance, make another choice.";
			cout << "\nPress any key to continue...";
			system("pause>0");
			system("cls");
			quick_withdraw(clients_file, client);
		}
	}
	else
	{
		system("cls");
		quick_withdraw(clients_file, client);
	}
}

void normal_withdraw(string clients_file, st_client& client)
{
	int withdraw_amount;
	char perform_or_not;
	string client_record = convert_client_record_to_line(client, "/#/");
	cout << "==========================================";
	cout << "\n\tNormal Withdraw Screen";
	cout << "\n==========================================\n";
	do
	{
		cout << "\nEnter an amount multiple of 5's: ";
		cin >> withdraw_amount;
	} while (withdraw_amount % 5 != 0 || withdraw_amount == 0);
	if (client.account_balance >= withdraw_amount)
	{
		cout << "\nAre you sure you want perform this transaction (Y/N)? ";
		cin >> perform_or_not;
		if (tolower(perform_or_not) == 'y')
		{
			client.account_balance -= withdraw_amount;
			cout << "\nDone Successfully. New balance is: " << client.account_balance;
			string updated_client_record = convert_client_record_to_line(client, "/#/");
			update_record_in_file(clients_file, client_record, updated_client_record);
		}
		else cout << "\nThe transaction has not been performed.";
	}
	else
	{
		cout << "\nThe amount exceeds your balance, make another choice.";
		cout << "\nPress any key to continue...";
		system("pause>0");
		system("cls");
		normal_withdraw(clients_file, client);
	}
}

void deposit(string clients_file, st_client& client)
{
	int deposit_amount;
	char perform_or_not;
	string client_record = convert_client_record_to_line(client, "/#/");
	cout << "==========================================";
	cout << "\n\t\tDeposit Screen";
	cout << "\n==========================================\n";
	do
	{
		cout << "\nEnter a positive deposit amount: ";
		cin >> deposit_amount;
	} while (deposit_amount <= 0);
	cout << "\nAre you sure you want perform this transaction (Y/N)? ";
	cin >> perform_or_not;
	if (tolower(perform_or_not) == 'y')
	{
		client.account_balance += deposit_amount;
		cout << "\nDone Successfully. New balance is: " << client.account_balance;
		string updated_client_record = convert_client_record_to_line(client, "/#/");
		update_record_in_file(clients_file, client_record, updated_client_record);
	}
	else cout << "\nThe transaction has not been performed.";
}

void check_balance(st_client client)
{
	cout << "==========================================";
	cout << "\n\tCheck Balance Screen";
	cout << "\n==========================================\n";
	cout << "\nYour Balance is " << client.account_balance;
}

void main_menue(string clients_file, st_client client)
{
	short operation_number;
	do
	{
		show_main_menue();
		cout << "\nChoose what do you want to do? [1 to 5]? ";
		cin >> operation_number;
		system("cls");
		if (operation_number >= 1 && operation_number <= 4)
		{
			switch (operation_number)
			{
			case en_main_menue_options::en_quick_withdraw: quick_withdraw(clients_file, client); break;
			case en_main_menue_options::en_normal_withdraw: normal_withdraw(clients_file, client); break;
			case en_main_menue_options::en_deposit: deposit(clients_file, client); break;
			case en_main_menue_options::en_check_balance: check_balance(client);
			}
			cout << "\nPress any key to go back to main menue...";
			system("pause>0");
			system("cls");
		}
	} while (operation_number != 5);
}

void ATM_system(string clients_file)
{
	do
	{
		st_client client;
		login(clients_file, client);
		main_menue(clients_file, client);
	} while (true);
}

int main()
{
	ATM_system("clients.txt");
}