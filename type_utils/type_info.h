#pragma once

#include <iostream>


namespace type_utils
{

    struct Info 
    {
        using THash = uint64_t;

        struct StringView
        {
            const char* data;
            size_t size;
        };

    private:        

        static constexpr THash fnvBasis = 14695981039346656037ull;
        static constexpr THash fnvPrime = 1099511628211ull;

        // FNV-1a 64 bit hash
        static constexpr THash Fnv1aHash(size_t size, const char* data, THash hash = fnvBasis)
        {
            return size > 0 ? Fnv1aHash(size - 1, data + 1, (hash ^ *data) * fnvPrime) : hash;
        }

    public:

        template<class T>
        static constexpr StringView Name()
        {
            const char* pTypeBeginPos = __PRETTY_FUNCTION__;
            
            while(*pTypeBeginPos++ != '=');
            for(; *pTypeBeginPos == ' '; ++pTypeBeginPos);

            const char* pTypeEndPos = pTypeBeginPos;

            int count = 1;
            for(;;++pTypeEndPos)
            {
                if(*pTypeEndPos == '[')
                {
                    ++count;
                }
                else if(*pTypeEndPos == ']')
                {
                    if(!--count)
                    {
                        return {pTypeBeginPos, size_t(pTypeEndPos - pTypeBeginPos)};
                    }
                }
            }
            return {};
        }

        template<typename T>
        static constexpr THash Hash()
        {
            constexpr auto name = Name<T>();
            return Fnv1aHash(name.size, name.data);
        }

    }; // struct Info

    inline std::ostream& operator<<(std::ostream& ostream, const Info::StringView& stringView)
    {
        return ostream.write(stringView.data, stringView.size);
    }

} // namespace type_hash

