#ifndef DIPLOM_RECORD_H
#define DIPLOM_RECORD_H

#import <tuple>
#import "Key.h"

template<int... Nums>
struct KeyIndex {
};

template<int... Idx, typename Tuple>
decltype(auto) getKey(Tuple &&t) {
    return std::make_tuple(std::get<Idx>(std::forward<Tuple>(t))...);
}

template<typename Tuple, typename Key>
class Record;

template<typename... Types, int... Idx>
class Record<std::tuple<Types...>, KeyIndex<Idx...>> {
public:
    typedef std::tuple<Types...> Tuple;
    typedef KeyIndex<Idx...> KeyIndex;
    typedef tuple_elements_t<Tuple, Idx...> KeyTuple;
    typedef Key<KeyTuple> Key;

    Tuple tuple;
    Key key;

    Record(const Tuple &tuple) : tuple(tuple), key(Key(getKey<Idx...>(tuple))) {}

    bool operator==(const Record<Tuple, KeyIndex> &oth) const {
        return tuple == oth.tuple;
    }
};

#endif //DIPLOM_RECORD_H
