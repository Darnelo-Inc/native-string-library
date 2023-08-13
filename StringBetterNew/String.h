#ifndef STRING_H
#define STRING_H
#include <cstring>
#include <ostream>
class String {
public:
    struct sRep;
    sRep* _rep;
    class CRef;
    String();
    String(const char* dt);
    String(const String& s);
    ~String();
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

struct String::sRep {
public:
    char* _data;
    int _size;
    int _ref;
    sRep(int sz, const char* dt) :_size(sz), _ref(1) {
        _data = new char[_size + 1];
        strcpy(_data, dt);
    }
    ~sRep() { delete[] _data; };
    sRep* getOwnCopy() {
        if (_ref == 1) return this;
        --_ref;
        return new sRep(_size, _data);
    }
    void assign(int sz, const char* dt);
private:
    sRep(const sRep&);
    sRep& operator=(const sRep&);
};



class String::CRef {
    friend class String;
    String& _s;
    int _i;
    CRef(String& st, int i) : _s(st), _i(i) {};
public:
    operator char() const { return _s.read(_i); }
    void operator=(char c) { _s.write(_i, c); }
};

#endif