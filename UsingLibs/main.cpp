#include <iostream>
#include <LibLinkedList/LinkedList.h>
#include <LibString/String.h>

int main() {
    LinkedList<String> ll;
    ll << "Cry" << "Cry" << "Let" << "Em" << "Cry";
    std::cout << ll;
    std::cout << '\n';
    int nn;
    std::cin >> nn;
}
