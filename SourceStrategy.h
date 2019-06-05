#ifndef DIPLOM_SOURCESTRATEGY_H
#define DIPLOM_SOURCESTRATEGY_H

#include <fstream>
#include <tuple>
#include "Tuple.h"
#include "Record.h"

template<typename Record>
class SourceStrategy;

template<typename... Types, int... Idx>
class SourceStrategy<Record<std::tuple<Types...>, KeyIndex<Idx...>>> {
public:
    typedef std::tuple<Types...> Tuple;
    typedef KeyIndex<Idx...> KeyIndex;
    typedef Record<Tuple, KeyIndex> Record;

    virtual Record *getRecord() = 0;

    virtual Record *getNextRecord() = 0;

    virtual void next() = 0;

    virtual bool hasNext() = 0;

    virtual std::shared_ptr<SourceStrategy> clone() = 0;
};

#endif //DIPLOM_SOURCESTRATEGY_H
