#define _CRT_SECURE_NO_WARNINGS
#include "String.h"
#include <iostream>
#include <ostream>
using namespace std;

void fatalMsg(const char* msg) {
	cout << msg << "\n", abort();
}


/*    String constructors and destructor   */
String::String() {
	_rep = new sRep(0, "");
}
String::String(const char* dt) {
	_rep = new sRep(strlen(dt), dt);
}
String::String(const String& s) {
	s._rep->_ref++;
	_rep = s._rep;
}
String::~String() { if (!--_rep->_ref) delete _rep; };


/*    String operators    */
String& String::operator=(const char* dt) {
	int sz = strlen(dt);
	if (_rep->_ref == 1)
		_rep->assign(sz, dt);
	else {
		_rep->_ref--;
		_rep = new sRep(sz, dt);
	} return *this;
}
String& String::operator=(const String& st) {
	st._rep->_ref++;
	if (!--_rep->_ref) delete _rep;
	_rep = st._rep;
	return *this;
}
char String::operator[](int i) const {
	checkRange(i); return read(i);
}
std::ostream& operator<<(std::ostream& os, const String& st) {
	os << st._rep->_data;
	return os;
}
String& String::operator+=(const char* dt) {
	--_rep->_ref;
	int dtSz = strlen(dt);
	sRep* rep = new sRep(_rep->_size + dtSz, _rep->_data);
	strcpy(&rep->_data[_rep->_size], dt);
	if (!_rep->_ref) delete _rep;
	_rep = rep;
	return *this;
}
String& String::operator+=(const String& st) {
	--_rep->_ref;
	sRep* rep = new sRep(st._rep->_size + _rep->_size, _rep->_data);
	strcpy(&rep->_data[_rep->_size], st._rep->_data);
	if (!_rep->_ref)delete _rep;
	_rep = rep;
	return *this;
}
String operator+(const String& st, const char* dt) {
	String ret(st); return ret += dt;
}
String operator+(const String& st1, const String& st2) {
	String ret(st1); return (ret += st2);
}
String::operator const char* () const { return _rep->_data; }


/*    String functions    */
void String::checkRange(int i) const {
	if (i < 0 || i >= _rep->_size) fatalMsg("Index out of range");
}
char String::read(int i) const {
	return _rep->_data[i];
}
void String::write(int i, char c) {
	_rep = _rep->getOwnCopy(); // _rep->getOwnCopy();
	_rep->_data[i] = c;
}


/*    CRef operators   */
String::CRef String::operator[](int i) {
	checkRange(i);
	return CRef(*this, i);
}


/*    sRep functions   */
void String::sRep::assign(int sz, const char* dt) {
	if (_size != sz) {
		delete[] _data;
		_data = new char[(_size = sz) + 1];
	}
	strcpy(_data, dt);
}
