#define _CRT_SECURE_NO_WARNINGS
#ifndef STRING_H
#define STRING_H

#include <iostream>
    //#include <ostream>

#define PageSize 5u
    typedef unsigned int uint;

class StringRef;
class String {
    struct sRep {
        friend class String;
        char* _data;
        uint _size;
        uint _ref;
        uint _capacity;

        void* operator new (size_t sz, uint n);
        void* operator new (size_t sz, sRep* old, uint n);
        void operator delete(void* b) { free(b); }

        char* data() { return (char*)(this + 1); } 

    private:
        sRep() {}
        ~sRep() {}
        sRep(const sRep&);
        sRep& operator = (const sRep&);
    } *_rep;

    struct FakeRep { static char* sharedEmpty[]; };
    static sRep* sharedEmpty() { return(sRep*)FakeRep::sharedEmpty; }

    static sRep* ref(sRep* rep) { ++rep->_ref;      return rep; }
    static void deref(sRep* rep) { if (!rep->_ref--) delete rep; }

    void getOwnCopy() {
        if (!_rep->_ref) return;
        --_rep->_ref;
        sRep* ret = new(size())sRep;
        strcpy(ret->data(), _rep->data());
        _rep = ret;                               // МЮ !!! не было строки
    }

    class CRef {
        friend class String;
        String& _s;
        uint _i;
        CRef(String& st, uint i) :_s(st), _i(i) { }
    public:
        operator char() const { return _s.read(_i); }
        void operator = (char c) { _s.write(_i, c); }
    };
    //
    //char read(int i)const;       // МЮ: это в открытую часть, эти методы участвуют в реализации operator[]
    //void write(int i, char c);
    //

public:
    String();
    String(const char*);
    String(const char*, uint);
    String(const String&);
    ~String();

    int size()     const { return _rep->_size; }
    int capacity() const { return _rep->_capacity; }

    operator const char* () const { return _rep->data(); }

    String& operator = (const char*);
    String& operator = (const String&);

    String substr(int from, int n) const;
    String left(int n) const;
    String right(int n) const;
    StringRef substrRef(int from, int n) const;

    int indexOf(char ch, uint from) const;

    void grow(uint n);
    void squeeze();
    void clear();

    //                             // МЮ: реализация operator[], которой почему-то не было...
    void checkRange(int i) const;
    char read(int i) const;
    void write(int i, char c);

    char operator[](int i) const;
    CRef operator[](int i);
    //

    String& operator+=(const char* dt);
    String  operator+(const String& st) const { String ret(*this); return ret += st; } // МЮ: и этой строки не было.

    String& insert(int to, const char* dt);
    String& replace(int from, int n, const char* dt);
    String& remove(int from, int n);

    friend bool operator==(const String& st1, const char* st2);
    friend bool operator==(const String& st1, const String& st2);
    friend bool operator!=(const String& st1, const char* st2);
    friend bool operator!=(const String& st1, const String& st2);

    //friend bool operator>=(const String& st1,  const String& st2);                 // МЮ: реализуйте также...
    //friend bool operator>=(const String& st1,  const char* st2);
};

class StringRef {
    String _str;
    uint _pos;
    uint _size;
public:
    StringRef(const String& str, uint pos, uint n) : _str(str), _pos(pos), _size(n) {}//; // МЮ: лишняя ";"
    operator const char* () const { return &((const char*)_str)[_pos]; }
    char operator[](int i) const { return _str[_pos + i]; }
    friend std::ostream& operator<<(std::ostream&, const StringRef&);
    uint pos()  const { return _pos; }
    uint size() const { return _size; }
};

inline String::String() : _rep(sharedEmpty()) { ++_rep->_ref; }
inline String::String(const char* dt) : _rep(new(strlen(dt))sRep) { strcpy(_rep->data(), dt); }
inline String::String(const char* dt, uint n) : _rep(new(n)sRep) { strncpy(_rep->data(), dt, n);   }
inline String::String(const String& other) : _rep(other._rep) { ++_rep->_ref; }
inline String::~String() { deref(_rep); }

#endif