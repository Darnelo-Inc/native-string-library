#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "String.h"
using namespace std;

//концепция неявного использования данных эффективна для однопоточных процессов

int main()
{
	String st1("Humpty");
	String st2(st1); //light-copy
	st2[0] = 'D';
	String st4 = st2;
	String st3 = st1 + st2 + st4;
	cout << (st1 + st2) << "\n";
	cout << (st1 + "Dumpty") << "\n";
	cout << ("Humpty" + st2);
	return 0;
}