#ifndef DIPLOM_TUPLE_H
#define DIPLOM_TUPLE_H

#include <fstream>
#include <math.h>

template<class _CharT, class _Traits, class Tuple, std::size_t N>
struct TuplePrinter {
    static void print(
            std::basic_ostream<_CharT, _Traits> &out,
            const Tuple &tuple
    ) {
        TuplePrinter<_CharT, _Traits, Tuple, N - 1>::print(out,
                                                           tuple);
        out << " " << std::get<N - 1>(tuple);
    }
};

template<class _CharT, class _Traits, class Tuple>
struct TuplePrinter<_CharT, _Traits, Tuple, 1> {
    static void print(
            std::basic_ostream<_CharT, _Traits> &out,
            const Tuple &tuple
    ) {
        out << std::get<0>(tuple);
    }
};

template<class _CharT, class _Traits, class... Types>
std::basic_ostream<_CharT, _Traits> &operator<<(
        std::basic_ostream<_CharT, _Traits> &out,
        const std::tuple<Types...> &tuple
) {
    TuplePrinter<
            _CharT,
            _Traits,
            decltype(tuple),
            sizeof...(Types)
    >::print(out, tuple);
    return out;
}

template<class _CharT, class _Traits>
std::basic_istream<_CharT, _Traits> &operator>>(
        std::basic_istream<_CharT, _Traits> &in,
        std::tuple<> &tuple
) {
    return in;
}

template<class _CharT, class _Traits, class FirstType, class... OtherTypes>
std::basic_istream<_CharT, _Traits> &operator>>(
        std::basic_istream<_CharT, _Traits> &in,
        std::tuple<FirstType, OtherTypes...> &tuple
) {
    FirstType firstValue;
    std::tuple<OtherTypes...> value;
    in >> firstValue;
    in >> value;
    tuple = std::tuple_cat(std::tuple(firstValue), value);
    return in;
}

template<class Tuple, int... Idx>
class tuple_elements;

template<class... Types, int... Idx>
class tuple_elements<std::tuple<Types...>, Idx...> {
public:
    typedef std::tuple<std::tuple_element_t<Idx, std::tuple<Types...>>...> type;
};

template<class Tuple, int... Idx>
using tuple_elements_t = typename tuple_elements<Tuple, Idx...>::type;

template<class Tuple, int Count>
struct TupleFirst {
    static auto first(Tuple &tuple) {
        return std::tuple_cat(
                TupleFirst<Tuple, Count - 1>::first(tuple),
                std::make_tuple(std::get<Count - 1>(tuple))
        );
    }
};

template<class Tuple>
struct TupleFirst<Tuple, 1> {
    static auto first(Tuple &tuple) {
        return std::make_tuple(std::get<0>(tuple));
    }
};

template<int Count, class Tuple>
auto tuple_first(Tuple &tuple) {
    return TupleFirst<Tuple, Count>::first(tuple);
}

template<class Tuple, std::size_t N>
struct TupleComparator {
    static int compare(const Tuple &a, const Tuple &b) {
        int comp = TupleComparator<Tuple, N - 1>::compare(a, b);
        if (comp == N) {
            comp += std::get<N>(a) == std::get<N>(b) ? 1 : 0;
        }
        return comp;
    }
};

template<class Tuple>
struct TupleComparator<Tuple, 0> {
    static int compare(const Tuple &a, const Tuple &b) {
        return std::get<0>(a) == std::get<0>(b) ? 1 : 0;
    }
};

template<class... Types>
int tuple_compare(std::tuple<Types...> &a, std::tuple<Types...> &b) {
    return TupleComparator<
            std::tuple<Types...>,
            sizeof...(Types) - 1
    >::compare(a, b);
}

#endif //DIPLOM_TUPLE_H
