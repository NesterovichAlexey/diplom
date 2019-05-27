#ifndef DIPLOM_KEY_H
#define DIPLOM_KEY_H

#include "tuple"
#include "Tuple.h"

template<typename Tuple>
class Key;

template<typename... Types>
class Key<std::tuple<Types...>> {
public:
    typedef std::tuple<Types...> Tuple;

    int size = sizeof...(Types);
    Tuple value;

    Key(const Tuple &value) : value(value) {}

    int compare(Key<Tuple> &oth) {
        return tuple_compare(value, oth.value);
    }

    template<typename... OtherTypes>
    int compare(Key<OtherTypes...> &oth) {
        if (oth.size < size) {
            return tuple_compare(tuple_first<oth.size>(value), oth.value);
        }
        return tuple_compare(value, tuple_first<size>(oth.value));
    }
};

#endif //DIPLOM_KEY_H
