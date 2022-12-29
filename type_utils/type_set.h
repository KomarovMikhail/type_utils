#pragma once

#include<type_universal_set.h>


namespace type_utils
{

    struct Set
    {
    private:
        
        template<typename>
        struct AnyValidator : std::true_type {};

    public:

        template<typename ...T>
        using Container = UniversalSet::Container<AnyValidator, std::is_same, T...>;

        template<typename TSet>
        static void Print(std::ostream& ostream)
        {
            UniversalSet::Print<TSet>(ostream);
        }

        template<typename TSet, typename T>
        using Insert = UniversalSet::Insert<TSet, T>;
    };
    
} // namespace type_utils