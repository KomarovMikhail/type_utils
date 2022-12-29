#include <iostream>
#include <type_list.h>
#include <type_tree.h>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <type_info.h>
#include <type_set.h>
#include <type_map.h>


struct Statement 
{
    void bindInt(int value) {
        std::cout << "bindInt(" << value << ")" << std::endl;
    }

    void bindString(const std::string& value) {
        std::cout << "bindString(" << value << ")" << std::endl;
    }
};
template<auto f>
struct FunctionWrapper
{
    static constexpr decltype(f) function = f;
};

using tree = type_utils::Map::Container<
    type_utils::Map::Pair<int,                FunctionWrapper<&Statement::bindInt>>,
    type_utils::Map::Pair<int&,               FunctionWrapper<&Statement::bindInt>>,
    type_utils::Map::Pair<const int&,         FunctionWrapper<&Statement::bindInt>>,
    type_utils::Map::Pair<std::string,        FunctionWrapper<&Statement::bindString>>,
    type_utils::Map::Pair<std::string&,       FunctionWrapper<&Statement::bindString>>,
    type_utils::Map::Pair<const std::string&, FunctionWrapper<&Statement::bindString>>,
    type_utils::Map::Pair<const char*,        FunctionWrapper<&Statement::bindString>>,
    double,
    type_utils::Map::Pair<const char*&,       FunctionWrapper<&Statement::bindString>>,
    int
>;

template<typename T>
void bindValue(Statement& statement, T&& value) 
{
    using wrapper = typename type_utils::Tree::Get<tree, T>::result;
    (statement.*wrapper::function)(std::forward<T>(value));
}

template<typename ...T>
void bindValues(Statement& statement, T&&... values)
{
    (bindValue(statement, std::forward<T>(values)),...);
}

using set = type_utils::Set::Container<int, double, float, int&, double, float, std::string>;
using set2 = typename type_utils::Set::Insert<set, const double>::result;


int main()
{
    // std::cout << type_utils::UniversalSet::Length<set>::value << std::endl;
    // std::cout << std::type_index(typeid(int)).hash_code() << std::endl;
    // std::cout << std::type_index(typeid(int)).hash_code() << std::endl;
    // std::cout << typeid(node::value).name() << std::endl;
    // std::cout << typeid(node::leftChild).name() << std::endl;
    // std::cout << typeid(node::rightChild).name() << std::endl;

    // std::cout << std::is_same<type_utils::_private_helper::Name<const char*>>::value << std::endl;

    Statement statement;
    int a1 = 1;
    const int a2 = 2;
    int a3 = 3; 
    std::string s1("s1");
    std::string s2("s2");
    const char* s3 = "s3";


    // std::cout << type_utils::_private_helper::Name<const char*>() << std::endl;
    // std::cout << type_utils::_private_helper::Name<decltype(s3)>() << std::endl;
    // std::cout << std::is_same<decltype(s3), const char*>::value << std::endl;
    // bindValues(statement, a1, a2, std::move(a3), 4, s1, std::move(s2), s3);

    // type_utils::Tree::Print<tree>(std::cout);

    type_utils::Set::Print<set2>(std::cout);


    return 0;
}