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

    Tuple value;

    Key(const Tuple &value) : value(value) {}

    int compare(Key<Tuple> &oth) {
        return tuple_compare(value, oth.value);
    }
};

#endif //DIPLOM_KEY_H
