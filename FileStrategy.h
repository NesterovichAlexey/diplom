#ifndef DIPLOM_FILESTRATEGY_H
#define DIPLOM_FILESTRATEGY_H

#include <fstream>
#include <tuple>
#include "Tuple.h"
#include "Record.h"
#include "SourceStrategy.h"

template<typename Record>
class FileStrategy;

template<typename... Types, int... Idx>
class FileStrategy<Record<std::tuple<Types...>, KeyIndex<Idx...>>>
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

    void del(Record *rec) {
        if (record != nullptr) {
            delete rec;
        }
    }

public:
    explicit FileStrategy(const std::string &fileName) :
            fileName(fileName) {
        fin.open(fileName);
        next();
        next();
    }

    ~FileStrategy() {
        del(record);
        del(nextRecord);
        fin.close();
    }

    Record *getRecord() { return record; }

    Record *getNextRecord() { return nextRecord; }

    void next() {
        del(record);
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

    std::shared_ptr<SourceStrategy<Record>> clone() {
        return std::make_shared<FileStrategy>(fileName);
    }
};

#endif //DIPLOM_FILESTRATEGY_H
