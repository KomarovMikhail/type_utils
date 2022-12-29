#pragma once

#include <iostream>
#include <type_traits>
#include <type_list.h>


namespace type_utils
{
    struct UniversalSet
    {
        // Container

        template<
            template<typename> typename TValidator,
            template<typename, typename> typename TComparator, 
            typename... T> 
        struct Container;

        template<
            template<typename> typename TValidator,
            template<typename, typename> typename TComparator>
        struct Container<TValidator, TComparator> 
        {
            using list = List::Container<>;
        };

    private:

        // InsertHelper

        template<
            template<typename> typename TValidator,
            template<typename, typename> typename TComparator,
            typename TList,
            typename T>
        struct InsertHelper;

        template<
            template<typename> typename TValidator,
            template<typename, typename> typename TComparator,
            typename T>
        struct InsertHelper<TValidator, TComparator, List::Container<>, T> 
        {
            static_assert(TValidator<T>::value, "Wrong item type");

            using result = List::Container<T>;
        };

        template<
            template<typename> typename TValidator,
            template<typename, typename> typename TComparator,
            typename THead,
            typename ...TTail,
            typename T>
        struct InsertHelper<TValidator, TComparator, List::Container<THead, TTail...>, T>
        {
            static_assert(TValidator<T>::value, "Wrong item type");

            using result = typename std::conditional<
                TComparator<THead, T>::value,
                List::Container<THead, TTail...>,
                typename List::Append<THead, typename InsertHelper<TValidator, TComparator, List::Container<TTail...>, T>::result>::result
            >::type;
        };

        // ConstructListHelper

        template<
            template<typename> typename TValidator,
            template<typename, typename> typename TComparator, 
            typename TList,
            typename ...T>
        struct ConstructListHelper;

        template<
            template<typename> typename TValidator,
            template<typename, typename> typename TComparator, 
            typename TList, 
            typename T>
        struct ConstructListHelper<TValidator, TComparator, TList, T>
        {
            static_assert(TValidator<T>::value, "Wrong item type");

            using result = typename InsertHelper<TValidator, TComparator, TList, T>::result;
        };

        template <
            template<typename> typename TValidator,
            template<typename, typename> typename TComparator, 
            typename TList, 
            typename THead, 
            typename ...TTail>
        struct ConstructListHelper<TValidator, TComparator, TList, THead, TTail...>
        {
            static_assert(TValidator<THead>::value, "Wrong item type");

            using result = typename InsertHelper<
                TValidator,
                TComparator,
                typename ConstructListHelper<TValidator, TComparator, TList, TTail...>::result,
                THead
            >::result;
        };


    public:

        // Container

        template<
            template<typename> typename TValidator,
            template<typename, typename> typename TComparator,
            typename THead,
            typename... TTail>
        struct Container<TValidator, TComparator, THead, TTail...> 
        {
            using list = typename ConstructListHelper<TValidator, TComparator, List::Container<>, THead, TTail...>::result;

            template<typename T1, typename T2>
            using comparator = TComparator<T1, T2>;
        };


        // Length

        template<typename TSet>
        struct Length
        {
            static constexpr size_t value = List::Length<typename TSet::list>::value;
        };


        // Insert

        template<typename TSet, typename T>
        struct Insert;

        template<
            template<typename> typename TValidator,
            template<typename, typename> typename TComparator,
            typename TNew,
            typename ...T>
        struct Insert<Container<TValidator, TComparator, T...>, TNew>
        {
            using result = Container<TValidator, TComparator, TNew, T...>;
        };


        // Print

        template<typename TSet>
        static void Print(std::ostream& ostream)
        {
            List::Print<typename TSet::list>(ostream);
        }

    }; // struct UniversalSet

} // namespace type_utils
