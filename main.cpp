#include <iostream>
#include <fstream>
#include <tuple>
#include <sstream>

#include "FileStrategy.h"
#include "Tuple.h"
#include "Tree.h"
#include "test.h"
#include "ManySourceStrategy.h"

void sampleSumOneLevel() {
    typedef std::tuple<int, int, double> Tuple;
    typedef KeyIndex<0, 1> Key;
    typedef Record<Tuple, Key> Record;
    typedef FileStrategy<Record> File;
    File file("../data.txt");
    Tree<Record> tree(file);
    double sum = 0.;
    for (auto it = tree.beginPostOrderIterator(); it != tree.endPostOrderIterator(); ++it) {
        if (it.isLeaf()) {
            sum += std::get<2>(it->tuple);
        }
        if (it.getDepth() == 1) {
            std::cout << std::get<0>(it->key.value) << " " << sum << std::endl;
            sum = 0.;
        }
    }
    std::cout << std::endl;
}

void sampleSumAllLevel() {
    typedef std::tuple<int, int, double> Tuple;
    typedef KeyIndex<0, 1> Key;
    typedef Record<Tuple, Key> Record;
    typedef FileStrategy<Record> File;
    File file("../data.txt");
    Tree<Record> tree(file);
    double sum[3]{0, 0, 0};
    for (auto it = tree.beginPostOrderIterator(); it != tree.endPostOrderIterator(); ++it) {
        switch (it.getDepth()) {
            case 0:
                std::cout << "0 " << sum[0] << std::endl;
                break;
            case 1:
                std::cout << "1 " << std::get<0>(it->key.value) << " " << sum[1] << std::endl;
                sum[0] += sum[1];
                sum[1] = 0;
                break;
            case 2:
                sum[2] = std::get<2>(it->tuple);
                std::cout << "2 " << it->key.value << " " << sum[2] << std::endl;
                sum[1] += sum[2];
                break;
        }
    }
    std::cout << std::endl;
}

void sample() {
    typedef std::tuple<std::string, int, std::string, int> Tuple;
    typedef KeyIndex<1, 0, 2> Key;
    typedef Record<Tuple, Key> Record;
    typedef FileStrategy<Record> File;
    File file("../data2.txt");
    Tree<Record> tree(file);
    int sum = 0, n = 0;
    for (auto it = tree.beginPostOrderIterator(); it != tree.endPostOrderIterator(); ++it) {
        if (it.isLeaf()) {
            sum += std::get<3>(it->tuple);
            ++n;
        }
        if (it.getDepth() == 2) {
            std::cout << std::get<1>(it->key.value) << " " << 1. * sum / n << std::endl;
            sum = n = 0;
        }
    }
}

int main() {
    testAll();
    sampleSumOneLevel();
    sampleSumAllLevel();
    sample();
    return 0;
}