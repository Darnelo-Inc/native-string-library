#include "String.h"
#define _CRT_SECURE_NO_WARNINGS

void fatalMsg(const char* msg) {
    std::cout << msg << "\n", abort();
}
char String::read(int i)const {
    return _rep->_data[i];
}

void String::write(int i, char c) {
    getOwnCopy();
    _rep->_data[i] = c;
}

static inline char* mlc(size_t n) {
    char* s;
    if ((s = (char*)malloc(n)) == NULL)
        fatalMsg("out of memory");
    return s;
}

static inline char* rlc(void* old, size_t n) {
    char* s;
    if ((s = (char*)realloc(old, n)) == NULL)
        fatalMsg("out of memory");
    return s;
}

char* String::FakeRep::sharedEmpty[sizeof(sRep) / sizeof(char*) + 1] = { (char*)sharedEmpty + sizeof(sRep) };

inline uint mallocsize(uint dtSz, size_t repSz) {
    return (dtSz + (uint)repSz + 1 + ((1u << PageSize) - 1u)) & ~((1u << PageSize) - 1u);
}   
inline uint maxSize(size_t repSz) {
    return (~0u & ~((1u << PageSize) - 1u)) - (uint)repSz - 1; 
}
inline uint capSize(uint allocSz, size_t repSz) {
    return allocSz - (uint)repSz - 1;
}

void* String::sRep::operator new(size_t sz, uint n) {
    if (!n) return ref(sharedEmpty());
    if (n > maxSize(sz)) fatalMsg("too long"); 
    uint mallocSz = mallocsize(n, sz); 
    sRep* rep = (sRep*)mlc(mallocSz); 
    rep->_capacity = capSize(mallocSz, sz); 
    (rep->_data = rep->data())[rep->_size = n] = 0;
    rep->_ref = 0; 
    return rep;
}

void* String::sRep::operator new(size_t sz, sRep* old, uint n) {
    sRep* rep;
    if (!n) {
        rep = ref(sharedEmpty());
        deref(old); //
        return rep;
    }
    if (n > maxSize(sz)) fatalMsg("too long");
    uint allocSz = mallocsize(n, sz), capSz = capSize(allocSz, sz);//
    if (capSz != (rep = old)->_capacity)//
        (rep = (sRep*)rlc(old, allocSz))->_capacity = capSz; //
    (rep->_data = rep->data())[rep->_size = n] = 0;
    return rep;
}

String& String::operator=(const char* dt) {
    if (dt == _rep->data()) return *this;
    int dtSz = strlen(dt);
    if (_rep->_ref) {
        deref(_rep);
        strcpy((_rep = new(dtSz)sRep)->data(), dt);
        return *this;
    }
    if (_rep->data() < dt && (_rep->data() + size() >= dt + dtSz)) { //если пересекаются
        memmove(_rep->data(), dt, dtSz);
        _rep = new(_rep, dtSz)sRep;
    }
    else strcpy((_rep = new(_rep, dtSz)sRep)->data(), dt);
    return *this;
}
String& String::operator=(const String& st) {
    sRep* tmp = _rep;
    _rep = ref(st._rep);
    deref(tmp);
    return *this;
}

String String::substr(int from, int n) const {
    int sz = size();
    if (!sz || !from && n == sz) return *this;
    if (from < 0 || from >= sz) from = 0;
    if (n<0 || from + n > sz) n = sz - from;
    return String(&_rep->data()[from], n);
}

StringRef String::substrRef(int from, int n) const {
    int sz = size();
    if (from < 0 || from >= sz) from = 0;
    if (n < 0 || from + n > sz) n = sz - from;
    return StringRef(*this, from, n);
}

String String::left(int n) const {
    if (n >= size() || n < 0) return *this;
    return String(_rep->data(), n);
}
String String::right(int n) const {
    if (n >= size() || n < 0) return *this;
    return String(&_rep->data()[size() - n], n);
}

std::ostream& operator<<(std::ostream& os, const StringRef& rf) {
    for (uint i = 0; i < rf.size(); ++i) os << rf[i];
    return os;
}

int String::indexOf(char ch, uint from) const {
    char* b = _rep->data(), * p = b + from;
    while (*p && *p != ch)++p;
    return *p ? (p - b) : -1;
}

void String::grow(uint n) {
    int sz = size();
    if (!n || capacity() - sz >= n) return;
    if (_rep->_ref) {
        deref(_rep);
        sRep* tmp = new(sz + n)sRep;
        strcpy(tmp->data(), _rep->data());
        _rep = tmp;
    }
    else _rep = new(_rep, sz + n)sRep;
    _rep->_size = sz;
}

void String::squeeze() {
    int sz = size();
    if (mallocsize(sz, sizeof(sRep)) - sizeof(sRep) - 1 == (uint)capacity()) return;
    if (_rep->_ref) {
        deref(_rep);
        sRep* tmp = new(sz) sRep;
        strcpy(tmp->data(), _rep->data());
        _rep = tmp;
    }
    else _rep = new(_rep, sz)sRep;
    _rep->_size = sz;
}

void String::clear() {
    if (!size()) return;
    getOwnCopy();
    _rep->data()[0] = 0;
    _rep->_size = 0;
}

String& String::insert(int to, const char* dt) {
    int dtSz = strlen(dt), sz = size();
    if (to < 0 || to >= sz) to = 0;
    if (!dtSz) return *this;
    grow(dtSz);
    int remSz = sz - to;
    memmove(&_rep->data()[to + dtSz], &_rep->data()[to], remSz);
    if (dt >= (_rep->data() + to) && dt < (_rep->data() + sz)) dt += dtSz;
    memmove(&_rep->data()[to], dt, dtSz);
    return *this;
}
String& String::replace(int from, int n, const char* dt) {
    if (!n) return *this;
    int dtSz = strlen(dt), sz = size();
    if (from < 0 || from >= sz) from = 0;
    if (n<0 || from + n > sz) n = sz - from;
    int remSz = sz - from - n;
    getOwnCopy();
    if (n == dtSz) memmove(&_rep->data()[from], dt, dtSz);
    else if (n > dtSz) {
        memmove(&_rep->data()[from], dt, dtSz);
        memmove(&_rep->data()[from + dtSz], &_rep->data()[from + n], remSz);
        _rep = new(_rep, from + dtSz + remSz)sRep;
    }
    else {
        _rep = new(_rep, from + dtSz + remSz)sRep;
        memmove(&_rep->data()[from + dtSz], &_rep->data()[from + n], remSz);
        if (dt >= (_rep->data() + from + n) && dt < (_rep->data() + sz)) dt += dtSz - n;
        memmove(&_rep->data()[from], dt, dtSz);
    }
    return *this;
}

String& String::remove(int from, int n) {
    if (!n) return *this;
    int sz = size();
    if (from < 0 || from >= sz) from = 0;
    if (n<0 || from + n > sz) n = sz - from;
    if (!sz || !from && n == sz) {
        sRep* tmp = _rep;
        _rep = ref(sharedEmpty());
        deref(tmp);
    }
    else {
        int remSz = sz - from - n;
        getOwnCopy();
        memmove(&_rep->data()[from], &_rep->data()[from + n], remSz);
        _rep = new(_rep, from + remSz)sRep;
    }
    return *this;
}

bool operator==(const String& st1, const char* st2) {
    return strcmp(st1._rep->data(), st2) == 0;
}
bool operator==(const String& st1, const String& st2) {
    return strcmp(st1._rep->data(), st2._rep->data()) == 0;
}
bool operator!=(const String& st1, const char* st2) {
    return strcmp(st1._rep->data(), st2) != 0;
}
bool operator!=(const String& st1, const String& st2) {
    return strcmp(st1._rep->data(), st2._rep->data()) != 0;
}

String& String::operator+=(const char* dt) {
    int oldSz = size();
    int dtSz = strlen(dt);
    grow(dtSz);
    strcpy(&_rep->data()[oldSz], dt);
    _rep->_size += dtSz;
    _rep->data()[_rep->_size] = 0;
    return *this;

}