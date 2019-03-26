#ifndef DIPLOM_TUPLE_IO_H
#define DIPLOM_TUPLE_IO_H

#include <fstream>
#include <math.h>

template<class _CharT, class _Traits>
inline std::basic_istream<_CharT, _Traits> &operator>>(std::basic_istream<_CharT, _Traits> &in, std::tuple<> &tuple) {
    return in;
}

template<class _CharT, class _Traits, typename FirstType, typename... OtherTypes>
inline std::basic_istream<_CharT, _Traits> &operator>>(std::basic_istream<_CharT, _Traits> &in, std::tuple<FirstType, OtherTypes...> &tuple) {
    FirstType firstValue;
    std::tuple<OtherTypes...> value;
    in >> firstValue;
    in >> value;
    tuple = std::tuple_cat(std::tuple(firstValue), value);
    return in;
}

template<class _CharT, class _Traits, typename... Types, std::size_t ...Indices>
inline void print(std::basic_ostream<_CharT, _Traits> &out, std::tuple<Types...> &tuple, std::index_sequence<Indices...>) {
    using swallow = int[];
    (void) swallow{0, (void(out << std::get<Indices>(tuple) << " "), 0)...};
}

template<class _CharT, class _Traits, typename Tuple>
inline std::basic_ostream<_CharT, _Traits> &operator<<(std::basic_ostream<_CharT, _Traits> &out, Tuple &tuple) {
    print(out, tuple, std::make_index_sequence<std::tuple_size<Tuple>::value>{});
    return out;
}

template<typename... Types, std::size_t ...Indices>
inline int compare(std::tuple<Types...> &a, std::tuple<Types...> &b, std::index_sequence<Indices...>) {
    std::size_t tupleSize = std::tuple_size<std::tuple<Types...>>::value;
    std::size_t ans = tupleSize;
    using swallow = int[];
    (void) swallow{0, (void(ans = std::min(ans,
                                           std::get<Indices>(a) != std::get<Indices>(b) ? Indices : tupleSize)), 0)...};
    return ans;
}

template<typename... Types>
inline int compare(std::tuple<Types...> &a, std::tuple<Types...> &b) {
    return compare(a, b, std::make_index_sequence<std::tuple_size<std::tuple<Types...>>::value>{});
}

#endif //DIPLOM_TUPLE_IO_H
