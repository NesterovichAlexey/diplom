#ifndef DIPLOM_TEST_H
#define DIPLOM_TEST_H

#include <iostream>
#include <fstream>
#include <tuple>
#include <sstream>

#include "FileStrategy.h"
#include "Tuple.h"
#include "Tree.h"
#include "ManySourceStrategy.h"

using TupleTest = std::tuple<int, int, double>;
using KeyIndexTest = KeyIndex<0, 1>;
using RecordTest = Record<TupleTest, KeyIndexTest>;
using FileTest = FileStrategy<RecordTest>;
using TreeTest = Tree<RecordTest>;

template<typename Record>
class MockSourceStrategy;

template<typename... Types, int... Idx>
class MockSourceStrategy<Record<std::tuple<Types...>, KeyIndex<Idx...>>>
        : public SourceStrategy<Record<std::tuple<Types...>, KeyIndex<Idx...>>> {
public:
    typedef std::tuple<Types...> Tuple;
    typedef KeyIndex<Idx...> KeyIndex;
    typedef Record<Tuple, KeyIndex> Record;
private:
    int n;
    Record *records;
    int index = 0;
public:
    MockSourceStrategy(int n, Record *records) : n(n), records(records) {}

    Record *getRecord() {
        if (index >= n) {
            return nullptr;
        } else {
            return &records[index];
        }
    }

    Record *getNextRecord() {
        if (index + 1 >= n) {
            return nullptr;
        } else {
            return &records[index + 1];
        }
    }

    void next() {
        ++index;
    }

    bool hasNext() { return index < n - 1; }

    std::shared_ptr<SourceStrategy<Record>> clone() {
        return std::make_shared<MockSourceStrategy>(n, records);
    }
};


void testTupleRead() {
    std::cout << "Test Tuple read\n";
    std::stringstream ss;
    ss << "1 a 5.4";
    std::tuple<int, std::string, double> tuple;
    ss >> tuple;
    assert(tuple == std::make_tuple(1, "a", 5.4));
}

void testTuplePrint() {
    std::cout << "Test Tuple print\n";
    std::stringstream ss;
    ss << std::make_tuple(2, 8.6, "b");
    int a;
    double b;
    std::string c;
    ss >> a >> b >> c;
    assert(a == 2);
    assert(b == 8.6);
    assert(c == "b");
}

void testTupleFirst() {
    std::cout << "Test Tuple first\n";
    std::tuple<int, double, std::string, int> tuple(
            std::make_tuple(1, 5.2, "hi", 10));
    assert(tuple_first<1>(tuple) == std::make_tuple(1));
    assert(tuple_first<2>(tuple) == std::make_tuple(1, 5.2));
    assert(tuple_first<3>(tuple) == std::make_tuple(1, 5.2, "hi"));
    assert(tuple_first<4>(tuple) == std::make_tuple(1, 5.2, "hi", 10));
}

void testTupleCompare() {
    std::cout << "Test Tuple compare\n";
    std::tuple<int, int, int> t1(1, 2, 3);
    std::tuple<int, int, int> t2(1, 2, 3);
    std::tuple<int, int, int> t3(1, 2, 4);
    std::tuple<int, int, int> t4(1, 3, 3);
    std::tuple<int, int, int> t5(2, 2, 3);
    assert(tuple_compare(t1, t2) == 3);
    assert(tuple_compare(t1, t3) == 2);
    assert(tuple_compare(t1, t4) == 1);
    assert(tuple_compare(t1, t5) == 0);
}

void testKeyCompare() {
    std::cout << "Test Key.compare\n";
    Key<std::tuple<int, int, int>> k1(std::make_tuple(1, 2, 3));
    Key<std::tuple<int, int, int>> k2(std::make_tuple(1, 2, 3));
    Key<std::tuple<int, int, int>> k3(std::make_tuple(1, 2, 4));
    Key<std::tuple<int, int, int>> k4(std::make_tuple(1, 3, 3));
    Key<std::tuple<int, int, int>> k5(std::make_tuple(2, 2, 3));
    assert(k1.compare(k2) == 3);
    assert(k1.compare(k3) == 2);
    assert(k1.compare(k4) == 1);
    assert(k1.compare(k5) == 0);
}

void testRecord() {
    std::cout << "Test Record\n";
    typedef std::tuple<int, double, std::string> Tuple;
    Tuple tuple(1, 3.4, "hello");
    Record<Tuple, KeyIndex<0, 2>> r1(tuple);
    assert(r1.tuple == tuple);
    assert(r1.key.value == std::make_tuple(1, "hello"));
    Record<Tuple, KeyIndex<1>> r2(tuple);
    assert(r2.tuple == tuple);
    assert(r2.key.value == std::make_tuple(3.4));
}


void testRecordEqual() {
    std::cout << "Test Record.equal\n";
    typedef std::tuple<int, double, std::string> Tuple;
    Record<Tuple, KeyIndex<0, 1, 2>> r1(std::make_tuple(1, 2.4, "a"));
    Record<Tuple, KeyIndex<0, 1, 2>> r2(std::make_tuple(1, 2.4, "a"));
    Record<Tuple, KeyIndex<0, 1, 2>> r3(std::make_tuple(1, 2.4, "b"));
    assert(r1 == r2);
    assert(!(r1 == r3));
}

void testFileStrategy() {
    std::cout << "Test FileStrategy\n";
    RecordTest records[3]{
            RecordTest(std::make_tuple(1, 1, 1.2)),
            RecordTest(std::make_tuple(1, 2, 1.3)),
            RecordTest(std::make_tuple(2, 1, 2.3))
    };
    FileTest file("../test.txt");
    for (int i = 0; i < 3; ++i) {
        assert(*file.getRecord() == records[i]);
        if (i < 2) {
            assert(*file.getNextRecord() == records[i + 1]);
        }
        file.next();
    }
    assert(file.hasNext() == false);
}

void testFileStrategyClone() {
    std::cout << "Test FileStrategy.clone\n";
    RecordTest record1(std::make_tuple(1, 1, 1.2));
    RecordTest record2(std::make_tuple(1, 2, 1.3));
    FileTest file("../test.txt");
    assert(*file.getRecord() == record1);
    assert(*file.getNextRecord() == record2);
    std::shared_ptr<SourceStrategy<RecordTest>> file2 = file.clone();
    file2->next();
    assert(*file2->getRecord() == record2);
    assert(*file.getRecord() == record1);
    assert(*file.getNextRecord() == record2);
}

void testTreePreOrderIterator() {
    std::cout << "Test Tree PreOrderIterator\n";
    MockSourceStrategy<RecordTest> source(5, new RecordTest[5]{
            RecordTest(std::make_tuple(1, 1, 1.2)),
            RecordTest(std::make_tuple(1, 2, 1.3)),
            RecordTest(std::make_tuple(2, 2, 2.3)),
            RecordTest(std::make_tuple(3, 2, 3.1)),
            RecordTest(std::make_tuple(3, 4, 4.1))
    });
    auto answer = new std::tuple<int, int, int, double>[9]{
        std::make_tuple(0, 1, 1, 1.2),
        std::make_tuple(1, 1, 1, 1.2),
        std::make_tuple(2, 1, 1, 1.2),
        std::make_tuple(2, 1, 2, 1.3),
        std::make_tuple(1, 2, 2, 2.3),
        std::make_tuple(2, 2, 2, 2.3),
        std::make_tuple(1, 3, 2, 3.1),
        std::make_tuple(2, 3, 2, 3.1),
        std::make_tuple(2, 3, 4, 4.1)
    };
    int idx = 0;
    TreeTest tree(source);
    for (auto it = tree.beginPreOrderIterator(); it != tree.endPreOrderIterator(); ++it) {
        assert(answer[idx++] == std::tuple_cat(std::make_tuple(it.getDepth()), it->tuple));
    }
    assert(idx == 9);
}

void testTreePostOrderIterator() {
    std::cout << "Test Tree PostOrderIterator\n";
    MockSourceStrategy<RecordTest> source(5, new RecordTest[5]{
            RecordTest(std::make_tuple(1, 1, 1.2)),
            RecordTest(std::make_tuple(1, 2, 1.3)),
            RecordTest(std::make_tuple(2, 2, 2.3)),
            RecordTest(std::make_tuple(3, 2, 3.1)),
            RecordTest(std::make_tuple(3, 4, 4.1))
    });
    auto answer = new std::tuple<int, int, int, double>[9]{
            std::make_tuple(2, 1, 1, 1.2),
            std::make_tuple(2, 1, 2, 1.3),
            std::make_tuple(1, 1, 2, 1.3),
            std::make_tuple(2, 2, 2, 2.3),
            std::make_tuple(1, 2, 2, 2.3),
            std::make_tuple(2, 3, 2, 3.1),
            std::make_tuple(2, 3, 4, 4.1),
            std::make_tuple(1, 3, 4, 4.1),
            std::make_tuple(0, 3, 4, 4.1)
    };
    int idx = 0;
    TreeTest tree(source);
    for (auto it = tree.beginPostOrderIterator(); it != tree.endPostOrderIterator(); ++it) {
        assert(answer[idx++] == std::tuple_cat(std::make_tuple(it.getDepth()), it->tuple));
    }
    assert(idx == 9);
}

void testAll() {
    testTupleRead();
    testTuplePrint();
    testTupleFirst();
    testTupleCompare();
    testKeyCompare();
    testRecord();
    testRecordEqual();
    testFileStrategy();
    testFileStrategyClone();
    testTreePreOrderIterator();
    testTreePostOrderIterator();
}

#endif //DIPLOM_TEST_H
