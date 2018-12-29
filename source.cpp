#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const int N = 50;
struct bank
{
	char name[30];
	double blv;
	double usd;
	double euro;
};

void addDepositors(bank depos[], int &k)
{
	system("cls");
	int test = 0;
	printf("Input name: ");
	cin.ignore();
	cin.getline(depos[k].name, 30);
	do
	{
		printf("Your deposit in BLV: ");
		cin >> depos[k].blv;
		if (cin.fail()) {
			cin.clear();
			cin.ignore();
			cout <<"Invalid" << endl;
			depos[k].blv = -1;
		}
	} while (depos[k].blv < 0);
	do
	{
		printf(" Your deposit in USD: ");
		cin >> depos[k].usd;
	} while (depos[k].usd < 0);
	do
	{
		printf(" Your deposit in EURO: ");
		cin >> depos[k].euro;
	} while (depos[k].euro < 0);
	fstream ofp{ "bank.txt", ios::app | ios::out | ios::binary};
	if (ofp.fail())
	{
		cout << endl << "File cant open.";
		exit(1);
	}
	ofp.write((char *)&depos[k],sizeof(bank));
	ofp.close();
	system("cls");
	cout.setf(ios::showpoint);
	cout.setf(ios::fixed);
	cout.precision(3);
	cout << endl << depos[k].name << " made successful deposit of " << depos[k].blv << " BLV, " << depos[k].usd << " USD and " << depos[k].euro << " EURO.\n";
}

void searchDepositors() {
	system("cls");
	string nameSearch;
	bool flag = true;
	cout << endl << "Input name of Depositor: ";
	cin.ignore();
	getline(cin, nameSearch);
	bank yourDepos;
	fstream ifp{ "bank.txt", ios::in | ios::binary };
	if (ifp.fail())
	{
		cout << endl << "File cant open.";
		exit(1);
	}
	do
	{
		ifp.read((char *)&yourDepos, sizeof(bank));
		if (ifp.eof()) {
			flag = false;
			break;
		}
	} while (yourDepos.name != nameSearch && flag);
	if (flag)
	{
		cout.setf(ios::showpoint);
		cout.setf(ios::fixed);
		cout.precision(3);
		cout << endl << setw(6) << "Name" << setw(15) << "BLV" << setw(15) << "USD"  << setw(15) << "EURO" << endl;
		cout << setw(6) << yourDepos.name << setw(15) << yourDepos.blv << setw(15) << yourDepos.usd << setw(15) << yourDepos.euro << endl;
	}
	else
	{
		printf("Depositors not found.\n");
	}
	ifp.close();
}

void filterDepos() {
	system("cls");
	bank filteDeposit;
	int price;
	double allMoney;
	cout << "Set minimal price limit: ";
	cin >> price;
	fstream ifp{ "bank.txt", ios::in | ios::binary};
	if (ifp.fail())
	{
		cout << endl << "File cant open.";
		exit(1);
	}
	int z = 0;
	cout.setf(ios::showpoint);
	cout.setf(ios::fixed);
	cout.precision(3);
	bool haveDepos{ true };
	while (!ifp.eof())
	{
		ifp.read((char *)&filteDeposit, sizeof(bank));
		allMoney = filteDeposit.blv + (filteDeposit.euro / 1.95583) + (filteDeposit.usd / 1.71189);
		if (allMoney > price)
		{
			if (haveDepos)
			{
				cout << endl << setw(6) << "Name" << setw(15) << "BLV" << setw(15) << "USD" << setw(15) << "EURO" << endl;
				haveDepos = false;
			}
			cout << setw(6) << filteDeposit.name << setw(15) << filteDeposit.blv << setw(15) << filteDeposit.usd << setw(15) << filteDeposit.euro << endl;
		}
		z++;
	}
	ifp.close();
}

void sortDepositors() {
	system("cls");
	bank buf;
	fstream fp{ "bank.txt", ios::in | ios::binary };
	if (fp.fail())
	{
		cout << endl << "File cant open.";
		exit(1);
	}
	bank sortDepos[N];
	int z = 0;
	while (!fp.eof())
	{
		fp.read((char *)&sortDepos[z], sizeof(bank));
		z++;
	}
	z--;
	for (int i = 0; i < z; i++)
	{
		for (int y = i+1; y < z; y++)
		{
			if (sortDepos[i].usd > sortDepos[y].usd)
			{
				buf = sortDepos[i];
				sortDepos[i] = sortDepos[y];
				sortDepos[y] = buf;
			}
		}
	}
	fp.close();
	fp.open("bank.txt", ios::trunc|ios::out|ios::binary);
	if (fp.fail())
	{
		cout << endl << "File cant open.";
		exit(1);
	}
	cout.setf(ios::showpoint);
	cout.setf(ios::fixed);
	cout.precision(3);
	cout << endl << setw(6) << "Name" << setw(15) << "BLV" << setw(15) << "USD" << setw(15) << "EURO" << endl;
	for (int i = 0; i < z; i++)
	{
		fp.write((char *)&sortDepos[i], sizeof(bank));
		cout << setw(6) << sortDepos[i].name << setw(15) << sortDepos[i].blv << setw(15) << sortDepos[i].usd << setw(15) << sortDepos[i].euro << endl;
	}
	fp.close();
}

int main()
{
	ofstream ofp{ "bank.txt", ios::trunc | ios::binary};
	bank depositors[N];
	int choose;
	int j{ 0 };
	do
	{
		do
		{
			cout << "\n1.Add depositor\n2.Searching by name\n3.Filter\n4.Sort\n5.Exit\nChoose from 1 to 5: ";
			cin >> choose;
		} while (choose > 5 || choose < 1);
		
		switch (choose)
		{
		case 1:
			if (j < 50)
			{
				addDepositors(depositors, j);
				j++;
			}
			else
				printf("You cant input more from 50 faculty number.");
			break;
		case 2:
			char stop;
			do
			{
				searchDepositors();
				cout << "Continue searching press Y (for cansel N): ";
				cin.get(stop);
			} while (stop != 'N' && stop != 'n');
			system("cls");
			break;
		case 3:
			filterDepos();
			break;
		case 4:
			sortDepositors();
			break;
		default:
			exit(0);
			break;
		}		
	} while (true);
}
