#define _CRT_SECURE_NO_WARNINGS
#include "StringHeader.h"
#include <cstring>
#include <iostream>
void fatalMsg(const char* msg) {
	std::cout << msg << "\n";
	abort();
}


String::String() {
	_data = new char[(_size = 0) + 1];
	strcpy(_data, "");
	std::cout << "String()";
	std::cout << "\n";
}
String::String(const char* dt) {
	_data = new char[(_size = strlen(dt)) + 1];
	strcpy(_data, dt);
	std::cout << "String (char)";
	std::cout << "\n";
}
String::String(const String& _st) {
	_data = new char[(_size = _st.size()) + 1];
	strcpy(_data, _st._data);
	std::cout << "String(String)";
	std::cout << "\n";
}
String& String::operator=(const String& st) {
	if (_data == st._data) return *this;
	if (_size != st._size) {
		delete[] _data;
		_data = new char[(_size = st._size) +1];
	}
	strcpy(_data, st._data);
	std::cout << "= string";
	std::cout << "\n";
	return *this;
}
String& String::operator=(const char* dt) {
	int sz = strlen(dt);
	if (_size != sz) {
		delete [] _data;
		_data = new char[(_size = sz) + 1];
	}
	strcpy(_data, dt);
	std::cout << "= char";
	std::cout << "\n";
	return *this;
}
char String::operator[](int i) const {
	if (i < 0 || i > size()) fatalMsg("Index out of range");
	std::cout << "[] read";
	std::cout << "\n";
	return _data[i];
}
char& String::operator[](int i) {
	if (i < 0 || i > size()) fatalMsg("Index out of range");
	std::cout << "[] write";
	std::cout << "\n";
	return _data[i];
}