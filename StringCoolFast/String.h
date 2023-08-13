#ifndef STRING_H
#define STRING_H    
#include <cstring>
#include <ostream>

typedef unsigned int uint;

class String {
public:
    struct sRep;
    sRep* _rep;
    struct FakeRep { static char* sharedEmpty[]; };
    static sRep* sharedEmpty() { return  (sRep*)FakeRep::sharedEmpty; }
    static sRep* ref(sRep* rep);
    static void deref(sRep* rep);
    class CRef;
    String();
    String(const char* dt);
    String(const char*, uint);
    String(const String& s);
    ~String();
    int size() const;
    int capacity() const;
    String& operator=(const char* dt);
    String& operator=(const String& st);
    char operator[](int i) const;
    void checkRange(int i) const;
    char read(int i) const;
    void write(int i, char c);
    operator const char* () const;
    CRef operator[](int i);

    String& operator+=(const char*);
    String& operator+=(const String&);
    friend std::ostream& operator<<(std::ostream&, const String&);
};
String operator+(const String& st, const char* dt);
String operator+(const String& st1, const String& st2);

inline String::String() : _rep(sharedEmpty()) { ++_rep->_ref; }

inline String::String(const char* dt) : _rep(new(strlen(dt))sRep) { strcpy(_rep->data(), dt); }

inline String::String(const char* dt, uint n) : _rep(new(n)sRep) { strncpy(_rep->data(), dt, n); }

inline String::String(const String& other) : _rep(other._rep) { ++_rep->_ref; }

inline String::~String() { deref(_rep); }



struct String::sRep {
public:
    friend class String;
    char* _data;
    uint _size;
    uint _ref;
    uint _capacity;
    sRep(int sz, const char* dt) :_size(sz), _ref(1) {
        _data = new char[_size + 1];
        strcpy(_data, dt);
    }
    void* operator new(size_t sz, uint n);
    void* operator new(size_t sz, sRep* old, uint n);
    void operator delete(void* b) { free(b); }
    char* data() { return (char*)(this + 1); }
    ~sRep() { delete[] _data; };
    void getOwnCopy() {
        if (!_rep->_ref) return;
        --_rep->_ref;
        sRep* ret = new(size())sRep;
        strcpy(ret->data(), _rep->data());
        _rep = ret;
    }
    void assign(int sz, const char* dt);
private:
    sRep() {};
    ~sRep() {};
    sRep(const sRep&);
    sRep& operator=(const sRep&);
};



class String::CRef {
    friend class String;
    String& _s;
    uint _i;
    CRef(String& st, uint i) : _s(st), _i(i) {};
public:
    operator char() const { return _s.read(_i); }
    void operator=(char c) { _s.write(_i, c); }
};

#endif