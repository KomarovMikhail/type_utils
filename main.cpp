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

    void bindInt64(int64_t value) {
        std::cout << "bindInt(" << value << ")" << std::endl;
    }

    void bindDouble(double value) {
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

using TypeBindingsMap = type_utils::Map::Container<
    type_utils::Map::Pair<int,                FunctionWrapper<&Statement::bindInt>>,
    type_utils::Map::Pair<int&,               FunctionWrapper<&Statement::bindInt>>,
    type_utils::Map::Pair<const int,          FunctionWrapper<&Statement::bindInt>>,
    type_utils::Map::Pair<const int&,         FunctionWrapper<&Statement::bindInt>>,
    type_utils::Map::Pair<int64_t,            FunctionWrapper<&Statement::bindInt64>>,
    type_utils::Map::Pair<int64_t&,           FunctionWrapper<&Statement::bindInt64>>,
    type_utils::Map::Pair<const int64_t,      FunctionWrapper<&Statement::bindInt64>>,
    type_utils::Map::Pair<const int64_t&,     FunctionWrapper<&Statement::bindInt64>>,
    type_utils::Map::Pair<std::string,        FunctionWrapper<&Statement::bindString>>,
    type_utils::Map::Pair<std::string&,       FunctionWrapper<&Statement::bindString>>,
    type_utils::Map::Pair<const std::string&, FunctionWrapper<&Statement::bindString>>,
    type_utils::Map::Pair<const char*,        FunctionWrapper<&Statement::bindString>>,
    type_utils::Map::Pair<const char*&,       FunctionWrapper<&Statement::bindString>>,
    type_utils::Map::Pair<double,             FunctionWrapper<&Statement::bindDouble>>,
    type_utils::Map::Pair<double&,            FunctionWrapper<&Statement::bindDouble>>,
    type_utils::Map::Pair<const double,       FunctionWrapper<&Statement::bindDouble>>,
    type_utils::Map::Pair<const double&,      FunctionWrapper<&Statement::bindDouble>>,
    type_utils::Map::Pair<float,              FunctionWrapper<&Statement::bindDouble>>,
    type_utils::Map::Pair<float&,             FunctionWrapper<&Statement::bindDouble>>,
    type_utils::Map::Pair<const float,        FunctionWrapper<&Statement::bindDouble>>,
    type_utils::Map::Pair<const float&,       FunctionWrapper<&Statement::bindDouble>>
>;

template<typename T>
void bindValue(Statement& statement, T&& value) 
{
    using wrapper = typename type_utils::Map::Get<TypeBindingsMap, T>::result;
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
    Statement statement;
    int a1 = 1;
    const int a2 = 2;
    int a3 = 3; 
    std::string s1("s1");
    std::string s2("s2");
    const char* s3 = "s3";


    bindValues(statement, a1, a2, std::move(a3), 4, s1, std::move(s2), s3);

    type_utils::Map::Print<TypeBindingsMap>(std::cout);

    // type_utils::Set::Print<set2>(std::cout);


    return 0;
}