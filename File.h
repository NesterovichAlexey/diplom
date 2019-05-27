#ifndef DIPLOM_FILE_H
#define DIPLOM_FILE_H

#include <fstream>
#include <tuple>
#include "Tuple.h"
#include "Record.h"
#include "SourceStrategy.h"

template<typename Record>
class File;

template<typename... Types, int... Idx>
class File<Record<std::tuple<Types...>, KeyIndex<Idx...>>>
        : public SourceStrategy<Record<std::tuple<Types...>, KeyIndex<Idx...>>> {
public:
    typedef std::tuple<Types...> Tuple;
    typedef KeyIndex<Idx...> KeyIndex;
    typedef Record<Tuple, KeyIndex> Record;
private:
    std::string fileName;
    std::ifstream fin;
    bool end = false;
    Record *record = nullptr;
    Record *nextRecord = nullptr;
public:
    explicit File(const std::string &fileName) : fileName(fileName) {
        fin.open(fileName);
        next();
        next();
    }

    ~File() { fin.close(); }

    Record *getRecord() { return record; }

    Record *getNextRecord() { return nextRecord; }

    void next() {
        record = nextRecord;
        if (hasNext()) {
            Tuple tuple;
            if (fin >> tuple) {
                nextRecord = new Record(tuple);
            } else {
                end = true;
                nextRecord = nullptr;
            }
        }
    }

    bool hasNext() { return !end; }

    File *copy() { return new File(fileName); }
};

#endif //DIPLOM_FILE_H
