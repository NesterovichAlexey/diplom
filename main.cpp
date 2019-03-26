#include <iostream>
#include <fstream>
#include <tuple>

#include "tuple_io.h"
#include "TreeFromOneFile.h"

typedef std::tuple<int, int, int> KeyType;

void testPreOrderWithOneFile() {
    std::ofstream out("out-pre-one.txt");

    int n = std::tuple_size<KeyType>::value;
    TreeFromManyFiles<KeyType, int> tree("data.txt");
    for (TreeFromManyFiles<KeyType, int>::PreOrderIterator it = tree.preOrderBegin(); it != tree.preOrderEnd(); ++it) {
        out << n - it.depth << " " << it.keys << " " << *it << std::endl;
    }
    out.close();
}

void testPostOrderWithOneFile() {
    std::ofstream out("out-post-one.txt");

    int n = std::tuple_size<KeyType>::value;
    int* sum = new int[n + 1];
    for (int i = 0; i <= n; ++i) {
        sum[i] = 0;
    }
    TreeFromManyFiles<KeyType, int> tree("data.txt");
    for (TreeFromManyFiles<KeyType, int>::PostOrderIterator it = tree.postOrderBegin(); it != tree.postOrderEnd(); ++it) {
        if (it.isLeaf()) {
            sum[it.depth] = *it;
        }
        if (it.depth != 0) {
            sum[it.depth - 1] += sum[it.depth];
        }
        out << it.depth << " " << it.keys << " " << sum[it.depth] << std::endl;
        sum[it.depth] = 0;
    }
    out.close();
}

int main() {
    std::ofstream out("../out.txt");

    int n = std::tuple_size<KeyType>::value;
    int* sum = new int[n + 1];
    for (int i = 0; i <= n; ++i) {
        sum[i] = 0;
    }
    TreeFromManyFiles<KeyType, int> tree("../data.txt");
    for (TreeFromManyFiles<KeyType, int>::PostOrderIterator it = tree.postOrderBegin(); it != tree.postOrderEnd(); ++it) {
        if (it.isLeaf()) {
            sum[it.depth] = *it;
        }
        if (it.depth != 0) {
            sum[it.depth - 1] += sum[it.depth];
        }
        out << it.depth << " " << it.keys << " " << sum[it.depth] << std::endl;
        sum[it.depth] = 0;
    }
    out.close();
    return 0;
}