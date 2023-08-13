#include <iostream>
#include <typeinfo>
using namespace std;

/*
Файл описаний:
    - инструменты
    - перечень источников (SOURCE,OBJECTS, LIBS)
    - цель сборки проекта
    - зависимости



*/



class A {
public:
    int _a;
    A(int a): _a(a) { }
    virtual ~A() { }
    virtual void fun1() {cout << "Hello\n";}
    int fun2(); //ne inline
    int fun3() {return 2;} //inline
};

int A::fun2() {return 1;}

typedef void(*pvf)(A* const);

inline int fun4() {return 4;}

A aa(1);
int a;
int b = 1;
int arr[] = {1, 2, 3};

int main() {
    int* p = (int*)&aa;
    cout << "vptr= " << (int*)p[0] << "\n";

    int* pp = (int*)p[0];
    ----pp;
    cout << "content of vtbl:\n";
    cout << (int*)pp << " [0] " << (int*)pp[0] << "\n";
    cout << (int*)(pp+1) << " [1] " << (int*)pp[1]<<"\n";
    cout << (int*)(pp+2) << " [2] " << (int*)pp[2]<<"\n";
    cout << (int*)(pp+3) << " [3] " << (int*)pp[3]<<"\n";
    cout << (int*)(pp+4) << " [4] " << (int*)pp[4]<<"\n";

    cout << "&typeid(A)= " << &typeid(A) << "\n";
    cout << "typeid(A).name()= " << typeid(A).name() << "\n";

    printf("%p", &fun4);
    cout << "\n";

    ((pvf)(pp[4]))(&aa);

    arr[0] = aa.fun2(); //ne inline
    arr[1] = aa.fun3(); //inline

    pp = (int*)0x004438e4;
    cout << "typename by hand= " << (char*)pp << "\n";


    A* da = new A(2);
    da->fun1();
    delete da;

    int nn;
    cin>>nn;
}
