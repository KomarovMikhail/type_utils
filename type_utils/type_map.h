#pragma once

#include <concepts>
#include <type_universal_set.h>


namespace type_utils
{

    struct Map
    {
        template<typename TKey, typename TValue>
        struct Pair
        {
            using key = TKey;
            using value = TValue;
        };

    private:

        // PairComparator
        
        template<typename TPair1, typename TPair2>
        struct PairComparator
        {
            static constexpr bool value = std::is_same<typename TPair1::key, typename TPair2::key>::value;
        };

        template<typename>
        struct PairValidator : std::false_type {};

        template<typename TKey, typename TValue>
        struct PairValidator<Pair<TKey, TValue>> : std::true_type {};

    public:

        template<typename ...T>
        using Container = UniversalSet::Container<PairValidator, PairComparator, T...>;

    }; // struct Map
    
} // namespace type_utils