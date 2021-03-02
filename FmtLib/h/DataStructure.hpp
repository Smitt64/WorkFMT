#ifndef DATASTRUCTURE_HPP
#define DATASTRUCTURE_HPP

#include <type_traits>

/*template<size_t idx, typename T>
struct GetHelper;

template<typename ... T>
struct DataStructure
{
    typedef std::common_type<T...> Type;
};

template<typename T, typename ... Rest>
struct DataStructure<T, Rest ...>
{

    DataStructure(const T& first, const Rest& ... rest)
        : first(first)
        , rest(rest...)
    {}

    T first;
    DataStructure<Rest ... > rest;

    template<size_t idx>
    auto &get()
    {
        return GetHelper<idx, DataStructure<T,Rest...>>::get(*this);
    }
};

template<typename T, typename ... Rest>
struct GetHelper<0, DataStructure<T, Rest ... >>
{
    static T &get(DataStructure<T, Rest...>& data)
    {
        return data.first;
    }
};

template<size_t idx, typename T, typename ... Rest>
struct GetHelper<idx, DataStructure<T, Rest ... >>
{
    static auto &get(DataStructure<T, Rest...>& data)
    {
        return GetHelper<idx-1, DataStructure<Rest ...>>::get(data.rest);
    }
};*/

#endif // DATASTRUCTURE_HPP
