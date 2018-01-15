#include <assert.h>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>


struct Node {
    int    n;      // value of node
    Node*  left;   // left subtree
    Node*  right;  // right subtree
    Node*  level;  // level pointer (node “to the right”)
};




/**
* Соединяет узлы одного уровня.
*/
void linkLevel( Node* current,  Node* parent = nullptr );


std::ostream& operator<<( std::ostream& out, const Node& );




/**
* Тасует элементы.
* Элементы со значением -1 остаются на своих местах.
*/
template< size_t N >
void shuffle( int a[ N ] ) {
    // возвращает случайный индекс с элементом, который можно тасовать
    const auto randomIndex = [ &a ] () -> size_t {
        size_t k = rand() % (N - 1);
        for ( ; (k < N) && (a[ k ] == -1); ++k) {
            // пропускаем элементы == -1
        }
        return k;
    };
    
    for (size_t i = 0; i < N; ++i) {
        if (a[ i ] != -1) {
            const size_t k = randomIndex();
            std::swap( a[ i ], a[ k ] );
        }
    }
}




/**
* Печатает все перестановки для списка 'a'.
*/
template< size_t N >
void permutation( int a[ N ],  int k = 0,  int level = -1 ) {
    static size_t p[ N ] = {};

    ++level;
    p[ k ] = level;

    if (level == N) {
        // перестановки завершены
        for (int i = 0; i < N; i++) {
            const int index = p[ i ] - 1;
            std::cout << a[ index ] << " ";
        }
        std::cout << std::endl;

    } else {
        for (int i = 0; i < N; i++) {
            if (p[ i ] == 0) {
                permutation< N >( a, i, level );
            }
        }
    }

    --level;
    p[ k ] = 0;
}




/**
* Функтор для сортировки пузырьком.
* Сложность: O( n * n )
* Число проходов ~ вдвое меньше, чем у стандартной сортировки.
*/
class BubbleSort {
public:
    template< typename T, size_t N >
    inline void operator()( T list[ N ] ) const {
        for (size_t j = 1; j < (N - 1); ++j) {
            for (size_t i = 1; i < (N - j); ++i) {
                if (list[ i ] < list[ i + 1 ]) {
                    std::swap( list[ i ],  list[ i + 1 ] );
                }
            }
        }
    }
};


/**
* Читает из файла массив строк.
* Упорядочивает строки по алфавиту.
*
* @template T Тип данных, который будет собран в 'result'.
* @template N Максимальное кол-во элементов, которое может быть прочитано
*             из файла.
* @template Sort Функтор сортировки.
* @template delimiter Разделитель для данных. Например, '\n', ';' ':'.
*/

template< typename T, size_t N, class Sort, char delimiter >
void readAndSort(
    T result[ N ],
    const std::string& fileName
) {
    // читаем файл, парсим данные
    std::ifstream file( fileName );
    std::string line;
    for (size_t k = 0; std::getline( file, line, delimiter ) && (k < N); ++k ) {
        result[ k ] = line;
    }
    // сортируем результат
    Sort s;
    s.operator()< T, N >( result );
}




/**
* Инвертирует однонаправленный список.
*/
struct List {
    int value;
    List* next;
};

void inverse( List* list ) {

    List* prev = nullptr;
    List* current = list;
    List* next = nullptr;
    do {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    } while ( next );
}





int main( int argc, char** argv ) {

#if 1
    // создаём тестовое дерево
    // # Согласно примеру из задания.
    
    // 2-й уровень
    Node n20 = { 1 };
    Node n21 = { 3 };
    Node n22 = { 5 };
    Node n23 = { 7 };

    // 1-й уровень
    Node n10 = { 2, &n20, &n21 };
    Node n11 = { 6, &n22, &n23 };

    // 0-й уровень (корень)
    Node root = { 4, &n10, &n11 };
   

    // соединяем узлы одного уровня
    linkLevel( &root );


    // печатаем результат
    std::cout << root;
#endif




#if 0
    // Перетасовка элементов
    /*
    static const size_t N = 20;
    int a[ N ] = {
        0,  2, -1, -1,  3,  4,  5, -1,  6, -1,
        0, 20, -1, -1, 30, 40, 50, -1, 60, 70
    };
    srand( 1234 );
    shuffle< N >( a );
    */




    // Все перестановки элементов
    /*
    static const size_t N = 3;
    int a[ N ] = { 100, 200, 300 };
    permutation< N >( a );
    */




    // Чтение файла, упорядочивание информации из него
    /*
    static const size_t BUFFER_SIZE = 100;
    std::string result[ BUFFER_SIZE ] = {};
    readAndSort<
        std::string,
        BUFFER_SIZE,
        BubbleSort,
        '\n'
    >( result, "1.data" );
    std::cout << "\nResult:\n";
    for (size_t i = 0; (i < BUFFER_SIZE) && !result[ i ].empty(); ++i) {
        std::cout << result[ i ] << std::endl;
    }
    */




    // Инвертировать однонаправленный список
    List c = { 30, nullptr };
    List b = { 20, &c };
    List a = { 10, &b };
    inverse( &a );
#endif


    std::cout << "\n^\n";
    std::cin.ignore();

    return 0;
}








inline void linkLevel( Node* current,  Node* parent ) {

    assert( ((current->left && current->right) || (!current->left && !current->right))
        && "Элемент списка должен содержать 2 потомка или ни одного." );

    if ( !current->left ) {
        // глубже узлов нет
        return;
    }

    // соединяем соседей в этом узле
    current->left->level = current->right;

    // спускаемся
    linkLevel( current->left,  current );
    linkLevel( current->right, current );

    // ищем соседа справа
    if ( !parent ) {
        // это корневой узел
        return;
    }

    // # Родитель узла знает о своём правом соседе.
    const auto neighbourParent = parent->level;
    current->level = neighbourParent->left;
}




std::ostream& operator<<( std::ostream& out, const Node& ) {

    // @todo ...
    out << "@todo" << std::endl;

    return out;
}
