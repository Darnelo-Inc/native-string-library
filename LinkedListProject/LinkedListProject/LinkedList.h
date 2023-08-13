#pragma once
#include <iostream>
typedef unsigned int uint;

struct lRep {
	lRep* _next, * _prev;
	uint _ref;
	uint _size;
	static lRep _sharedEmpty;
};

template <class T>
struct lNode {
	lNode(const T& val) : _data(val) { _next = _prev = this; }
	lNode* _next, * _prev;
	T _data;
};

template <class T>
class LinkedList {
	typedef lNode<T> node;
	union { lRep* _rep; node* _e; };
	void getOwnCopy();
	lRep* ref(lRep* rep) { ++rep->_ref; return rep; }
	void deref(lRep* rep);
	void linkNodes(node* n1, node* n2) { if (n1) n1->_next = n2; if (n2) n2->_prev = n1; }
public:
	LinkedList(): _rep(ref(&lRep::_sharedEmpty)) {}
	LinkedList(const LinkedList<T> &lst): _rep(ref(lst._rep)) {}
	~LinkedList() { deref(_rep); };
	void append(const T&);
	void printList(const char* lId = 0) const;
	LinkedList<T>& operator=(const LinkedList<T>&);
	bool operator==(const LinkedList<T>&) const;
	void clear();
	int count(const T&) const;
	void prepend(const T&);
	int removeOne(const T&);
	int removeAll();
	int size() const { return _rep->_size; };
	friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& lst) { lst.printList(); return os; };
	LinkedList<T>& operator+=(const LinkedList<T>&);
	LinkedList<T> operator+(const LinkedList<T>&) const;
	LinkedList<T>& operator+=(const T& dt) { append(dt); return *this; };
	LinkedList<T>& operator<<(const T& dt) { append(dt); return *this; };
	LinkedList<T>& operator<<(const LinkedList<T>& lst) { *this += lst; return *this; };

	class iterator {
		iterator();
	public:
		node* _i;
		iterator(node* n) : _i(n) {};
		iterator(const iterator& o) : _i(o._i) {};
                iterator& operator=(const iterator& o) { _i = o._i; return *this; };
		T& operator*()const { return _i->_data; };
		T* operator->()const { return &_i->_data; };
		bool operator==(const iterator& o)const { return _i == o._i; };
		bool operator!=(const iterator& o)const { return _i != o._i; };
		iterator& operator++() { _i = _i->_next; return *this; }
		iterator operator++(int) { node* n = _i; _i = _i->_next; return n; };
		iterator& operator--() { _i = _i->_prev; return *this; }
		iterator operator--(int) { node* n = _i; _i = _i->_prev; return n; };
		iterator operator+(int j) const { node* n = _i; if (j > 0) while (j--) n = n->_next; else while (j++) n = n->_prev; return n; };
		iterator operator-(int j)const { return operator+(-j); };
		iterator& operator+=(int j) { return *this = *this + j; };
		iterator& operator-=(int j) { return *this = *this - j; };
	};
	class const_iterator {
		const_iterator();
	public:
		node* _i;
		const_iterator(node* n) : _i(n) {};
		const_iterator(const iterator& o) : _i(o._i) {};
                const_iterator& operator=(const const_iterator& o) { _i = o._i; return *this; };
		const T& operator*()const { return _i->_data; };
		const T* operator->()const { return &_i->_data; };
		bool operator==(const const_iterator& o)const { return _i == o._i; };
		bool operator!=(const const_iterator& o)const { return _i != o._i; };
		const_iterator& operator++() { _i = _i->_next; return *this; }
		const_iterator operator++(int) { node* n = _i; _i = _i->_next; return n; };
		const_iterator& operator--() { _i = _i->_prev; return *this; }
		const_iterator operator--(int) { node* n = _i; _i = _i->_prev; return n; };
		const_iterator operator+(int j) const { node* n = _i; if (j > 0) while (j--) n = n->_next; else while (j++) n = n->_prev; return n; };
		const_iterator operator-(int j)const { return operator+(-j); };
		const_iterator& operator+=(int j) { return *this = *this + j; };
		const_iterator& operator-=(int j) { return *this = *this - j; };
	};

	iterator begin() { getOwnCopy(); return _e->_next; };
	const_iterator constBegin() const { return _e->_next; }
	iterator end() { getOwnCopy(); return _e; };
	const_iterator constEnd() const { return _e; };
	iterator insert(iterator before, const T& dt);
	iterator erase(iterator&);
	iterator erase(iterator& from, iterator to);

};

template<class T>
void LinkedList<T>::deref(lRep* rep) {
	if (rep->_ref--) return;
	node* e = (node*)rep, * i = e->_next;
	while (i != e) {
		node* n = i;
		i = i->_next;
		delete n;
	}
	delete rep;
}

template<class T>
void LinkedList<T>::getOwnCopy() {
	if (!_rep->_ref) return;
	union { lRep* rep; node* e; } ret;
	ret.rep = new lRep;
	ret.rep->_ref = 0;
	ret.rep->_size = _rep->_size;
	node* i = _e, * j = ret.e;
	while ((i = i->_next) != _e) {
		linkNodes(j, new node(i->_data));
		j = j->_next;
	}
	linkNodes(j, ret.e);
	deref(_rep);
	_rep = ret.rep;
}

template<class T>
void LinkedList<T>::append(const T& dt) {
	getOwnCopy();
	node* nd = new node(dt);
	linkNodes(_e->_prev, nd);
	linkNodes(nd, _e);
	++_rep->_size;
}

template <class T>
void LinkedList<T>::printList(const char* lId) const {
	node* i = _e;
	if (lId) std::cout << lId << "\n";
	std::cout << "   size:" << _rep->_size << "\n";
	std::cout << "   ref:" << _rep->_ref << "\n";
	if (!_rep->_size) return;
	std::cout << "   elements:\tthis\t\tnext\t\tprev\n";
	int idx = 0;
	while ((i = i->_next) != _e) std::cout << "   " << idx++ << " " << i->_data << "\t" << i << "\t" << i->_next << "\t" << i->_prev << "\n";
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& lst) {
	lRep* tmp = _rep;
	_rep = ref(lst._rep);
	deref(tmp);
	return *this;
}

template <class T>
void LinkedList<T>::clear() {
	*this = LinkedList<T>();
}

template <class T>
bool LinkedList<T>::operator==(const LinkedList<T>& lst) const {
	if (_rep == lst._rep) return true;
	if (size() != lst.size()) return false;
	node* i = _e->_next;
	node* il = lst._e->_next;
	while (i != _e) {
		if (!(i->_data == il->_data)) return false;
		i = i->_next;
		il = il->_next;
	}
	return true;
}

template <class T>
int LinkedList<T>::count(const T& dt) const {
	node* i = _e;
	int c = 0;
	while ((i = i->_next) != _e)
		if (i->_data == dt) c++;
	return c;
}

template<class T>
void LinkedList<T>::prepend(const T& dt) {
	getOwnCopy();
	node* nd = new node(dt);
	linkNodes(nd, _e->_next);
	linkNodes(_e, nd);
	++_rep->_size;
}

template <class T>
int LinkedList<T>::removeOne(const T& dt) {
	getOwnCopy();
	node* i = _e->_next;
	while (i != _e) {
		if (i->_data == dt) {
			node* n = i;
			linkNodes(i->_prev, i->_next);
			i = i->_next;
			delete n;
			--_rep->_size;
			return 1;
		}
		else i = i->_next;
	}
	return 0;
}

template <class T>
int LinkedList<T>::removeAll() {
	getOwnCopy();
	node* i = _e->_next;
	while (i != _e) {
		node* n = i;
		linkNodes(i->_prev, i->_next);
		i = i->_next;
		delete n;
		--_rep->_size;
	}
	return 1;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator+=(const LinkedList<T>& lst) {
	getOwnCopy();
	int sz = lst.size();
	_rep->_size += sz;
	node* o = lst._e->_next;
	while (sz--) {
		node* i = new node(o->_data);
		o = o->_next;
		linkNodes(_e->_prev, i);
		linkNodes(i, _e);
	}
	return *this;
}

template <class T>
LinkedList<T> LinkedList<T>:: operator+(const LinkedList<T>& lst) const {
	LinkedList<T> n = *this;
	return n+=lst;
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::insert(iterator before, const T& dt) {
	getOwnCopy();
	node* i = before._i;
	node* m = new node(dt);
	linkNodes(i->_prev, m);
	linkNodes(m, i);
	_rep->_size++;
	return m;
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::erase(iterator& pos) {
	getOwnCopy();
	node* i = pos._i;
	if (i != _e) {
		linkNodes(i->_prev, i->_next);
		_rep->_size--;
		pos = i->_next;
		delete i;
	} return pos;
}

template<class T>
typename LinkedList<T>::iterator LinkedList<T>::erase(iterator& from, iterator to) {
	while (from != to) {
		erase(from);
	}
	return from;
}
