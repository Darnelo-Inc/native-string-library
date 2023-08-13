#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

double* iter(double** a, double* y, int n)
{
	double* res = new double[n];
	int i, j;


	for (i = 0; i < n; i++)
	{
		res[i] = y[i] / a[i][i];
	}

	double eps;
	cout << "Введите точность вычисления: \n";
	cin >> eps;
	double* Xn = new double[n];

	do {
		for (i = 0; i < n; i++) {
			Xn[i] = y[i] / a[i][i];
			for (j = 0; j < n; j++) {
				if (i == j)
					continue;
				else {
					Xn[i] -= a[i][j] / a[i][i] * res[j];
				}
			}
		}

		bool flag = true;
		for (i = 0; i < n - 1; i++) {
			if (abs(Xn[i] - res[i]) > eps) {
				flag = false;
				break;
			}
		}

		for (i = 0; i < n; i++) {
			res[i] = Xn[i];
		}

		if (flag)
			break;
	} while (1);
	delete[] Xn;
	return res;
}



int main()
{
	setlocale(LC_ALL, "Russian");
	double** a;
	double* y;
	double* x;
	ifstream Infile;
	int n; //объявление i j можно убрать, for сам их создаст
	Infile.open("tartagalicious.txt");
	Infile >> n;

	y = new double[n];
	a = new double* [n];
	for (int i = 0; i < n; i++)
	{
		a[i] = new double[n];
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			if (j != n)
			{
				Infile >> a[i][j];
			}
			else
			{
				Infile >> y[i];
			}
		}
	}
	Infile.close();

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			if (j != n)
			{
				cout << a[i][j] << "\t";
			}
			else
			{
				cout << y[i] << "\t";
			}
		}
		cout << endl;
	}

	x = iter(a, y, n);
	// открываем тут
	string name;
	ofstream output;
	cout << "Введите название файла: - \n";
	cin >> name;
	output.open(name + ".txt"); //расширение файла .txt
	for (int i = 0; i < n; i++)
	{
		cout << x[i] << "\t";
		output << x[i] << "\t";
	}

	cout << endl;
	// не забываем память освобождать
	delete[] y;
	delete[] x;
	for (int i = 0; i < n; i++) {
		delete[] a[i];
	}
	return 0;
}
