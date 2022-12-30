#pragma once

#include <iostream>
#include <type_info.h>


namespace type_utils
{

    struct List
    {
        // Container

        template<typename...> struct Container;

        template<typename H, typename... T>
        struct Container<H, T...> 
        {
            using head = H;
            using tail = Container<T...>;
        };


        // Length

        template<typename TL>
        struct Length;

        template<typename ...T>
        struct Length<Container<T...>> 
        {
            static constexpr size_t value = sizeof...(T);
        };


        // At

        template<typename TL, size_t index>
        struct At;

        template<typename... T>
        struct At<Container<T...>, 0> 
        {
            using result = typename Container<T...>::head;
        };

        template<typename ...T, size_t index>
        struct At<Container<T...>, index>
        {
            static_assert(index < sizeof...(T), "type_list::At: index out of range");
            using result = At<typename Container<T...>::tail, index - 1>::result;
        };


        // Append

        template<typename TL, typename T>
        struct Append;

        template<typename ...H, typename T>
        struct Append<Container<H...>, T> 
        {
            using result = Container<H..., T>;
        };

        template<typename H, typename ...T>
        struct Append<H, Container<T...>> 
        {
            using result = Container<H, T...>;
        };

        template<typename ...T1, typename ...T2>
        struct Append<Container<T1...>, Container<T2...>> 
        {
            using result = Container<T1..., T2...>;
        };


        // Erase

        template<typename TL, size_t index>
        struct Erase;

        template <typename H, typename ...T>
        struct Erase<Container<H, T...>, 0> 
        {
            static_assert(0 < sizeof...(T) + 1, "type_list::Erase: index out of range");
            using result = Container<T...>;
        };

        template <typename H, typename ...T, size_t index>
        struct Erase<Container<H, T...>, index> 
        {
            static_assert(index < sizeof...(T) + 1, "type_list::Erase: index out of range");
            using result = typename Append<H, typename Erase<Container<T...>, index - 1>::result>::result;
        };


        // Print

        template<typename TList>
        static void Print(std::ostream& ostream)
        {
            ostream << "[";
            PrintHelper<TList>(ostream);
            ostream << "]" << std::endl;
        }

    private:

        // PrintHelper
        
        template<typename TList>
        static void PrintHelper(std::ostream& ostream)
        {
            ostream << Info::Name<typename TList::head>() << ", ";
            PrintHelper<typename TList::tail>(ostream);
        }

    }; // struct List

    template<>
    void List::PrintHelper<List::Container<>>(std::ostream& ostream)
    {
        return;
    }


} // namespace type_utils