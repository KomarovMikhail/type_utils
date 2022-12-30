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


        // GetHelper

        template<typename TList, typename TKey>
        struct GetHelper
        {
            using result = typename std::conditional<
                std::is_same<typename TList::head::key, TKey>::value,
                typename TList::head::value,
                typename GetHelper<typename TList::tail, TKey>::result
            >::type;
        };

        template<typename TKey>
        struct GetHelper<List::Container<>, TKey>
        {   
            // static_assert(false, "type_utils::Map::Get: key not found");
            using result = void;
        };

    public:

        // Container 

        template<typename ...T>
        using Container = UniversalSet::Container<PairValidator, PairComparator, T...>;


        // Get

        template<typename TMap, typename TKey>
        struct Get
        {
            using result = typename GetHelper<typename TMap::list, TKey>::result;
        };

        // Print

        template<typename TMap>
        static void Print(std::ostream& ostream)
        {
            ostream << "[";
            PrintHelper<typename TMap::list>(ostream);
            ostream << "]" << std::endl;
        }

    private:

        // PrintHelper
        template<typename TList>
        static void PrintHelper(std::ostream& ostream)
        {
            ostream << "(" << Info::Name<typename TList::head::key>() << ", " << Info::Name<typename TList::head::value>() << "), ";
            PrintHelper<typename TList::tail>(ostream);
        }

    }; // struct Map

    template<>
    void Map::PrintHelper<List::Container<>>(std::ostream& ostream)
    {
        return;
    }
    
} // namespace type_utils