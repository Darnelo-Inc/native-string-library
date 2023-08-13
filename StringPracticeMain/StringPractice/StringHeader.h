#ifndef STRINGHEAD
#define STRINGHEAD
class String {
	char* _data;
	int _size;
public:
	String();
	String(const char* dt);
	String(const String& st);
	~String() { delete[] _data; };
	int size() const { return _size; };
	String& operator=(const String& st);
	String& operator=(const char* st);
	char operator[](int i)const;
	char& operator[](int i);
};


#endif
