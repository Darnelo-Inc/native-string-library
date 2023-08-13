/*#define _CRT_SECURE_NO_WARNINGS
#include "LinkedList.h"
#include <iostream>;
#include "../../StringWithStatic/String.h";
using namespace std;

Шаблоны в .h
1. Все идентификаторы должны быть определены для сборки в .exe
2. Тело шаблона - не определение функции
3. Генерация определения при обнаружении вызова
4. Для генерации определения тело шаблона должно быть в том же .cpp
5. Если тела в .cpp нет, на месте вызова - ссылкана внешний символ
6. Если символа не окажется в других модулях - ошибка компоновки "неразрешённый символ"

Поэтому определения всех template функций у нас в заголовочном файле

int main() {
	LinkedList<int> lstI;
	lstI.append(1); lstI.append(2);
	LinkedList<const char*> lstC;
	lstC.append("Hello");
	lstC.append("world");
	cout << lstC;
}


struct emp {
	enum emp_type { E, M };
	emp_type type;
	emp() : type(E) {};
	String f_name, l_name;
	short dept;
	void print() const { std::cout << l_name << " " << dept; };
};

class Emp {
	String f_name, l_name;
	short dept;
public:
	Emp(const String& ln, short d) : l_name(ln), dept(d) {};
	virtual void print() const {
		std::cout << l_name << " " << dept << "\n";
	}
};

class Man : public Emp {
	LinkedList<emp*> group;
	short level;
public:
	Man(const String& ln, short d, short l) : Emp(ln, d), level(l) {};
	void print() const {
		Emp::print();
		std::cout << level;
	}
};

struct man : emp {
	man() { type = M; }
	LinkedList<emp*> group;
	short level;
	void print() const { std::cout << l_name  << group; }

};

void print_employee(const emp* e) {
	switch (e->type) {
	case emp::E:
		e->print(); break;
	case emp::M:
		((man*)e)->print(); break;
	}
}

void print_employeeClass(const Emp* e) { e->print(); };

void print_employees(const LinkedList<Emp*>& emps) {
	for (LinkedList<Emp*>::const_iterator cit = emps.constBegin(); cit != emps.constEnd(); ++cit) (*cit)->print();
}


class A {
public:
	int _a;
	A(int a) : _a(a) {};
	virtual void __cdecl fun1() const {
		cout << this << " " << _a << " A::fun1()\n";
	}
};
class B : public A {
public:
	B(int b) : A(b) {};
};
class C : public A {
public:
	C(int c) : A(c) {};
	void __cdecl fun1()const {
		std::cout << this << " " << _a << " C::fun1()\n";
	}
};

typedef void(*pf) (A* const);


void legalCall(const A* a) {
	std::cout << "legalCall\n";
	a->fun1();
}
void illegalCall(const A& a) {
	std::cout << "illegalCall\n";
	cout << "this=" << &a << "_a=" << a._a << "\n";
	cout << "\ncontents of obj:\n";
	int* p = (int*)&a;
	for (unsigned int i = 0; i < sizeof(A) / sizeof(int); ++i)
		cout << (int*)p[i] << "\n";
	cout << "\ncontents of vtbl:\n";
	int* vptr = (int*)p[0];
	cout << (int*)vptr[0] << "\n";
	cout << "\ncalling funcs of obj:\n";
	A* const pa = (A* const)&a;
	((pf)vptr[0])(pa);
}







int main() {
	/*
	LinkedList<String> lst;
	lst << "Mann" << "wer" << "hatte" << "das" << "gedacht" << "dass" << "es" << "einmal" << "so" << "weit" << "kommt";
	LinkedList<String>::const_iterator cit = lst.constBegin();
	for (; cit != lst.constEnd(); ++cit) {
		cout << *cit; 
	}
	cit = lst.constBegin();
	for (; cit != lst.constEnd(); ++cit)
		if (*cit == "einmal") { cout << "einmal found\n"; break; }


	LinkedList<String>::iterator it = lst.begin() + 7; //einmal
	lst.erase(it); //it -> so;
	cout << lst;
	lst.insert(it, "einmal"); //einmal it->so
	cout << lst;
	*it = "also";
	cout << lst;
	lst.erase(it, lst.end()); //без трёх 
	*/
	/*LinkedList<int> lst1;
	LinkedList<int> lst2;
	lst1 << 1 << 2 << 3;
	lst2 << 10 << 20 << 30;
	std::cout << lst1 << "\n";
	std::cout << lst2 << "\n";

	A a(1);
	legalCall(&a);
	illegalCall(a);

	B b(3);
	legalCall(&b);
	illegalCall(b);

	C c(5);
	legalCall(&c);
	illegalCall(c);
}*/
#define _CRT_SECURE_NO_WARNINGS
#include "LinkedList.h"
#include <iostream>                    // ; МЮ: лишняя ;
//#include <StaticStringLib/String.h>                  // МЮ: включение строки из библиотеки.
#include "../../StringWithStatic/String.h" // МЮ: включение строки по относительному пути.
using namespace std;
/*
Шаблоны в .h
1. Все идентификаторы (имена функций и глобальных переменных) должны быть определены для сборки исполняемого файла .exe
2. Тело шаблона - не определение функции
3. Генерация определения функции (для нужного типа - параметра шаблона) выполняется при обнаружении вызова функции в тексте программы
4. Для генерации определения функции тело шаблона должно быть в том же .cpp
5. Если тела шаблона в .cpp нет (полный текст .cpp формируется включением заданных .h), на месте вызова - ссылка на внешний символ
6. Если символа не окажется в других модулях - ошибка компоновки "неразрешённый символ"

Поэтому определения всех template функций (тела шаблонов) у нас в заголовочном файле,
чтобы компилятор смог сгенерировать определения функций, используемых в .cpp, в который включается LinkedList.h.
В частности, в main.cpp будут сгенерированы определения функций лишь для параметров шаблона, используемых в main.cpp (int, String, Emp*...)

int main() {
	LinkedList<int> lstI;
	lstI.append(1); lstI.append(2);
	LinkedList<const char*> lstC;
	lstC.append("Hello");
	lstC.append("world");
	cout << lstC;
}
*/
/*
// 1. Пример с полем типа.
struct emp {
	enum emp_type { E, M };
	emp_type type;
	emp() : type(E) {}                                             // ; МЮ: лишняя ;
	String f_name, l_name;
	short dept;
	void print() const { cout << l_name << " " << dept << "\n"; } // ; МЮ: лишняя ;
};

struct man : emp { // МЮ: перед именем предка не указан спецификатор наследования. Так тоже можно: по умолчанию используется public.
	man() { type = M; }
	LinkedList<emp*> group;
	short level;
	void print() const { emp::print(); cout << level << "\n" << group; } // МЮ: вызвать родительскую print, допечатать свое.
};

void print_employee(const emp* e) {
	switch (e->type) {
	case emp::E:
		e->print(); break;
	case emp::M:
		((man*)e)->print(); break;
	}
}
//

// 2. Пример с использованием виртуальных функций.
class Emp {
	String l_name;
	short dept;
public:
	Emp(const String& ln, short d) : l_name(ln), dept(d) {} // ; МЮ: лишняя ;
	virtual ~Emp() {}
	virtual void print() const {
		cout << l_name << " " << dept << "\n";
	}
};

class Man : public Emp {
	LinkedList<Emp*> group;
	short level;
public:
	Man(const String& ln, short d, short l) : Emp(ln, d), level(l) {} // ; МЮ: лишняя ;
	void print() const {
		Emp::print();
		cout << level << "\n" << group;
	}
};

void print_employeeClass(const Emp* e) { e->print(); } // ; МЮ: лишняя ;

void print_employees(const LinkedList<Emp*>& emps) {
	for (LinkedList<Emp*>::const_iterator cit = emps.constBegin(); cit != emps.constEnd(); ++cit) (*cit)->print();
}


class Shape {
public:
	Shape() {};
	virtual void rotate(int) = 0;
	virtual void draw() = 0;
	virtual bool isClosed() const = 0;
};

class Point {
	short x, y;
public:
	Point(short x, short y): x(x), y(y) {}
};
class Circle : public Shape {
	Point center;
	short radius;
public:
	Circle(const Point& cntr, short rd) : center(cntr), radius(rd) {}
	void rotate(int) {};
	void draw() {};
	bool isClose() { return true; }
};
class Polygon : public Shape {
public:
	bool isClosed() { return true; }
};
class Irregular_Polygon : public Polygon {
	LinkedList<Point> points;
public:
	void rotate(int) {};
	void draw() {};
	Irregular_Polygon(const LinkedList<Point> points) : points(points){}
};






// 3. Вызов виртуальных функций вручную.
class A {
public:
	int _a;
	A(int a) : _a(a) {} // ; МЮ: лишняя ;
						// МЮ: _cdecl с одним подчерком, но, впрочем, собирается и с двумя.
	virtual void _cdecl fun1()                const { cout << "this=" << this << " _a=" << _a << " A::fun1()\n"; }
	virtual void _cdecl fun2(int i)           const { cout << "this=" << this << " _a=" << _a << " A::fun2() " << i << "\n"; }
	virtual void _cdecl fun3(const char* msg) const { cout << "this=" << this << " _a=" << _a << " A::fun3() " << msg << "\n"; }
};
class B : public A {
public:
	B(int b) : A(b) {} // ; МЮ: лишняя ;
};
class C : public A {
public:
	C(int c) : A(c) {} // ; МЮ: лишняя ;
	void _cdecl fun1()                const { cout << "this=" << this << " _a=" << _a << " C::fun1()\n"; }
	void _cdecl fun2(int i)           const { cout << "this=" << this << " _a=" << _a << " C::fun2() " << i << "\n"; }
	void _cdecl fun3(const char* msg) const { cout << "this=" << this << " _a=" << _a << " C::fun3() " << msg << "\n"; }
};

typedef void (*pf) (A* const);
typedef void (*pfi)(A* const, int);
typedef void (*pfc)(A* const, const char*);

void legalCall(const A* a) {
	cout << "legalCall\n";
	a->fun1();
	a->fun2(1);
	a->fun3("H");
	cout << "\n";
}
void illegalCall(const A& a) {
	cout << "illegalCall\n";
	cout << "this=" << &a << "_a=" << a._a << "\n";

	cout << "\ncontents of obj:\n";
	int* p = (int*)&a;
	for (unsigned int i = 0; i < sizeof(A) / sizeof(int); ++i)
		cout << (int*)p[i] << "\n";

	cout << "\ncontents of vtbl:\n";
	int* vptr = (int*)p[0];
	cout << (int*)vptr[0] << "\n";
	cout << (int*)vptr[1] << "\n";
	cout << (int*)vptr[2] << "\n";

	cout << "\ncalling funcs of obj:\n";
	A* const pa = (A* const)&a;
	((pf)vptr[0])(pa);
	((pfi)vptr[1])(pa, 55);
	((pfc)vptr[2])(pa, "Hello");
	cout << "\n";
}

int main() {
	/*
	LinkedList<String> lst;
	lst << "Mann" << "wer" << "hatte" << "das" << "gedacht" << "dass" << "es" << "einmal" << "so" << "weit" << "kommt";
	LinkedList<String>::const_iterator cit = lst.constBegin();
	for (; cit != lst.constEnd(); ++cit) {
		cout << *cit;
	}
	cit = lst.constBegin();
	for (; cit != lst.constEnd(); ++cit)
		if (*cit == "einmal") { cout << "einmal found\n"; break; }


	LinkedList<String>::iterator it = lst.begin() + 7; //einmal
	lst.erase(it); //it -> so;
	cout << lst;
	lst.insert(it, "einmal"); //einmal it->so
	cout << lst;
	*it = "also";
	cout << lst;
	lst.erase(it, lst.end()); //без трёх
	*/
	/*LinkedList<int> lst1;
	LinkedList<int> lst2;
	lst1 << 1 << 2 << 3;
	lst2 << 10 << 20 << 30;
	std::cout << lst1 << "\n";
	std::cout << lst2 << "\n";*/
	
	// 1. Поле типа
	/*cout << "1. Type field\n";
	emp ee; ee.l_name = "Petrov"; ee.dept = 1;
	print_employee(&ee);
	man mm; mm.l_name = "Sidorov"; mm.dept = 2; mm.level = 3;
	print_employee(&mm);
	cout << "\n";*/
	/*
	// 2. Вирт функции
	cout << "2. Virtual functions\n";
	Emp eE("Brown", 1);
	Man mM("Smith", 2, 3);
	print_employeeClass(&eE);
	print_employeeClass(&mM);

	Emp* ep = new Man("Johnson", 4, 5);
	print_employeeClass(&mM);
	delete ep;
	print_employeeClass(&mM);
	std::cout << "\n\n\n\n";
	Man* mn1 = new Man("Mn1", 1, 2), * mn2 = new Man("Mn2", 2, 3), *mn3 = new Man("Mn3", 3, 4);
	print_employeeClass(&mM);
	{
		LinkedList<Man> lst;
		lst << *mn1 << *mn2 << *mn3;
		print_employeeClass(&mM);
	}
	print_employeeClass(&mM);
	std::cout << "\n\n\n\n";
	{
		LinkedList<Man*> lst;
		lst << mn1 << mn2 << mn3;
		print_employeeClass(&mM);
	}
	print_employeeClass(&mM);*/
	/*
	// 3. Вызов вирт функций вручную
	cout << "3. Calling virtual functions by hand\n";
	A a(1);
	legalCall(&a);
	illegalCall(a);

	B b(3);
	legalCall(&b);
	illegalCall(b);

	C c(5);
	legalCall(&c);
	illegalCall(c);
	



	int nn;
	cin >> nn;
	
}
*/

class A {
public:
	int _a;
	A(int a) : _a(a) {} // ; МЮ: лишняя ;
						// МЮ: _cdecl с одним подчерком, но, впрочем, собирается и с двумя.
	virtual void _cdecl fun1()                const { cout << "this=" << this << " _a=" << _a << " A::fun1()\n"; }
	virtual void _cdecl fun2(int i)           const { cout << "this=" << this << " _a=" << _a << " A::fun2() " << i << "\n"; }
	virtual void _cdecl fun3(const char* msg) const { cout << "this=" << this << " _a=" << _a << " A::fun3() " << msg << "\n"; }
};
class B : public A {
public:
	B(int b) : A(b) {} // ; МЮ: лишняя ;
};
class C : public A {
public:
	C(int c) : A(c) {} // ; МЮ: лишняя ;
	void _cdecl fun1()                const { cout << "this=" << this << " _a=" << _a << " C::fun1()\n"; }
	void _cdecl fun2(int i)           const { cout << "this=" << this << " _a=" << _a << " C::fun2() " << i << "\n"; }
	void _cdecl fun3(const char* msg) const { cout << "this=" << this << " _a=" << _a << " C::fun3() " << msg << "\n"; }
};

typedef void (*pf) (A* const);
typedef void (*pfi)(A* const, int);
typedef void (*pfc)(A* const, const char*);

void legalCall(const A* a) {
	cout << "legalCall\n";
	a->fun1();
	a->fun2(1);
	a->fun3("H");
	cout << "\n";
}
void illegalCall(const A& a) {
	cout << "illegalCall\n";
	cout << "this=" << &a << "_a=" << a._a << "\n";

	cout << "\ncontents of obj:\n";
	int* p = (int*)&a;
	for (unsigned int i = 0; i < sizeof(A) / sizeof(int); ++i)
		cout << (int*)p[i] << "\n";

	cout << "\ncontents of vtbl:\n";
	int* vptr = (int*)p[0];
	cout << (int*)vptr[0] << "\n";
	cout << (int*)vptr[1] << "\n";
	cout << (int*)vptr[2] << "\n";

	cout << "\ncalling funcs of obj:\n";
	A* const pa = (A* const)&a;
	((pf)vptr[0])(pa);
	((pfi)vptr[1])(pa, 55);
	((pfc)vptr[2])(pa, "Hello");
	cout << "\n";
}


int main() {
	cout << "3. Calling virtual functions by hand\n";
	A a(1);
	legalCall(&a);
	illegalCall(a);

	B b(3);
	legalCall(&b);
	illegalCall(b);

	C c(5);
	legalCall(&c);
	illegalCall(c);
	return 0;
}