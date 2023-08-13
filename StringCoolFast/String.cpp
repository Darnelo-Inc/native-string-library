#define _CRT_SECURE_NO_WARNINGS
#define PageSize 5u
#include "String.h"
#include <iostream>
#include <ostream>
using namespace std;

void fatalMsg(const char* msg) {
	cout << msg << "\n", abort();
}

static inline char* mlc(size_t n) {
	char* s;
	if ((s = (char*)malloc(n)) == NULL) fatalMsg("out of memory");
	return s;
}

static inline char* rlc(void* old, size_t n) {
	char* s;
	if ((s = (char*)realloc(old, n)) == NULL) fatalMsg("out of memory");
	return s;
}

char* String::FakeRep::sharedEmpty[sizeof(sRep) / sizeof(char*) + 1] = { (char*) sharedEmpty + sizeof(sRep) };

inline uint allocSize(uint dtSz, size_t repSz) {
	return  (dtSz + (uint)repSz + 1 + ((1u << PageSize) - 1u)) & ~((1u << PageSize - 1u));
}

inline uint maxSize(size_t repSz) {
	return (~0u & ~((1u << PageSize) - 1u)) - (uint)repSz - 1;
}

inline uint capSize(uint allocSz, size_t repSz) {
	return allocSz - (uint)repSz - 1;
}

/*    String operators    */
void* String::sRep::operator new(size_t sz, uint n) {
	if (!n) return ref(sharedEmpty());
	if (n > maxSize(sz)) fatalMsg("too long");
	uint allocSz = allocSize(n, sz);
	sRep* rep = (sRep*)mlc(allocSz);
	rep->_capacity = capSize(allocSz, sz);
	(rep->_data = rep->data())[rep->_size = n] = 0;
	rep->_ref = 0;
	return rep;
}
void* String::sRep::operator new(size_t sz, sRep* old, uint n) {
	sRep* rep;
	if (!n) { rep = ref(sharedEmpty()); deref(old); return rep; }
	if (n > maxSize(sz))fatalMsg("too long");
	uint allocSz = allocSize(n, sz), capSz = capSize(allocSz, sz);
	if (capSz != (rep = old)->_capacity)
		(rep = (sRep*)rlc(old, allocSz))->_capacity = capSz;
	(rep->_data = rep->data())[rep->_size = n] = 0;
	return rep;
}


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
int String::size() const { return _rep->_size; }
int String::capacity() const { return _rep->_capacity; }


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
String::sRep* String::ref(sRep* rep) { ++rep->_ref; return rep; };
void String::deref(sRep* rep) { if (!rep->_ref--) delete rep; };