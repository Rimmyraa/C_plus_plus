#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <functional>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "logig.cpp"

using namespace std;

int main()
{
	login userLogin;
	string userChoice;
	cout << "\t\t\t________________________________________________\n\n\n";
	cout << "\t\t\t               Welcome to the NEMO 2025 Login!               ";
	cout << "\t\t\t_____________			Menu			_____________";
	cout << "\t | Press 1 to LOGIN										 |";
	cout << "\t | Press 2 to REGISTER									 |";
	cout << "\t | Press 3 to if you forgot your PASSWORD				 |";
	cout << "\t | Press 4 to EXIT										 |";
	cout << "\n\t\t\t Please Enter your choice: ";
	cin >> userChoice;
	cout << "";

	if (userChoice == "1")
	{
		userLogin.Login();
		main();
	}
	else if (userChoice == "2")
	{
		userLogin.Registration();
		main();
	}
	else if (userChoice == "3")
	{
		userLogin.ForgotPassword();
		main();
	}
	else if (userChoice == "4")
	{
		cout << "\t\t\t Goodbye! \n\n";
	}
	else
	{
		system("cls");
		cout << "\t\t\t Please select from the options above\n";
		main();
	}
}
