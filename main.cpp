#include <iostream>
#include <fstream>
#include <tuple>

#include "File.h"
#include "Tuple.h"
#include "Tree.h"
#include "ManySourceStrategy.h"

int main() {
    typedef std::tuple<int, int, int, int> Tuple;
    typedef KeyIndex<0, 2> Key;
    typedef Record<Tuple, Key> Record;
    typedef File<Record> File;
    Tree<Record> file(new File("../data.txt"));
    int sum[3] = {0, 0, 0};
    for (auto it = file.beginPostOrderIterator(); it != file.endPostOrderIterator(); ++it) {
        if (it.isLeaf()) {
            sum[it.getDepth()] = std::get<1>(it->tuple) * std::get<3>(it->tuple);
        }
        if (it.getDepth() != 0) {
            sum[it.getDepth() - 1] += sum[it.getDepth()];
        }
        std::cout << it.getDepth() << " "
                  << it->key.value << " "
                  << sum[it.getDepth()] << std::endl;
        sum[it.getDepth()] = 0;
    }
    std::cout << std::endl;
    for (auto it = file.beginPreOrderIterator();
         it != file.endPreOrderIterator(); ++it) {
        std::cout << it.getDepth() << " " << it->tuple << std::endl;
    }
    std::cout << std::endl;
//    typedef File<Record<std::tuple<int, int>, KeyIndex<0>>> File0;
//    typedef File<Record<std::tuple<int, int, int>, KeyIndex<0, 1>>> File1;
//    typedef File<Record<std::tuple<int, int, int, int>, KeyIndex<0, 1, 2>>> File2;
//    ManyFilesStrategy<File0, File1, File2> files(std::make_tuple(File0("../data.txt0"), File1("../data.txt1"), File2("../data.txt2")));
//    for (auto it = files.beginPreOrderIterator(); it != files.endPreOrderIterator(); ++it) {
//        std::cout << it.getDepth() << " " << it->tuple << std::endl;
//    }
    return 0;
}