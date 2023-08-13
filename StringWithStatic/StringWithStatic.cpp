#include "String.h"
int main()
{
    String st1 = "a";
    String st2;
    std::cout << st1.size();
    // МЮ: иллюстрации
    //            012345678910
    String st3 = "Hello world";
    st3.replace(0, 5, &((const char*)st3)[6]); // n==dtSz
    std::cout << st3 << "\n";

    st3 = "Hello world";
    st3.replace(0, 5, &((const char*)st3)[9]); // n>dtSz
    std::cout << st3 << "\n";

    st3 = "Hello world";
    st3.replace(0, 2, &((const char*)st3)[6]); // n<dtSz
    std::cout << st3 << "\n";

    st3 = "Hello world";
    std::cout << st3.remove(2, 7) << "\n";

    st3 = "Hello world";
    std::cout << st3.left(5) << "\t" << st3.right(5) << "\n";;

    std::cout << st3.insert(6, String("hello ")) << "\n"; // insert принимает объект String, т.к. класс String имеет оператор приведения к const char*

    st3 = "Hello world";
    st2 = st3;
    st3.grow(28 - st3.size()); // Увеличить емкость для хранения "Hast du etwas Zeit fur mich?"
    st3.clear();             // "Очистить".
    st3 += "Hast ";          // Добавление без увеличения емкости.
    st3 += "du ";
    st3 += "etwas ";
    st3 += "Zeit ";
    st3 += "fur ";
    st3 += "mich?";
    std::cout << st3 << "\n";

    std::cout << st3[5] << "\n"; // Работа оператора [].
    st3[5] = 'r';
    std::cout << st3 << "\n";
    //

    int nn;
    std::cin >> nn;
    return 0;
}
