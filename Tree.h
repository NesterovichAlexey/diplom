#ifndef DIPLOM_TREE_H
#define DIPLOM_TREE_H

#include "SourceStrategy.h"
#include "Record.h"
#include "Key.h"
#include <tuple>
#include <iterator>

template<typename Record>
class Tree;

template<typename... Types, int... Idx>
class Tree<Record<std::tuple<Types...>, KeyIndex<Idx...>>> {
private:
    typedef std::tuple<Types...> Tuple;
    typedef Key<tuple_elements_t<Tuple, Idx...>> Key;
    typedef Record<Tuple, KeyIndex<Idx...>> Record;
    typedef SourceStrategy<Record> SourceStrategy;
    SourceStrategy &source;
public:
    explicit Tree(SourceStrategy &source) : source(source) {}

    class PostOrderIterator {
    private:
        std::shared_ptr<SourceStrategy> source;
        int keyCount = sizeof...(Idx);
        int depth = keyCount;

        void next() {
            if (depth > -1) {
                --depth;
                if (source != nullptr && source->hasNext()) {
                    Key key = source->getRecord()->key;
                    Key nextKey = source->getNextRecord()->key;
                    int diff = key.compare(nextKey);
                    if (diff == depth) {
                        readRecord();
                        depth = keyCount;
                    }
                }
            }
        }

        void readRecord() {
            if (source != nullptr) {
                source->next();
            }
        }

    public:
        explicit PostOrderIterator(std::shared_ptr<SourceStrategy> source)
                : source(source) {}

        int getDepth() {
            return depth;
        }

        bool isLeaf() {
            return depth == keyCount;
        }

        Record &operator*() { return *(source->getRecord()); }

        Record *operator->() { return source->getRecord(); }

        PostOrderIterator operator++() {
            next();
            return *this;
        }

        PostOrderIterator operator++(int) {
            PostOrderIterator ret = *this;
            next();
            return ret;
        }

        bool operator==(const PostOrderIterator &oth) {
            if (source == nullptr && oth.depth == -1 &&
                (oth.source == nullptr ||
                 oth.source->hasNext() == false)) {
                return true;
            }
            if (oth.source == nullptr && depth == -1 &&
                (source == nullptr || source->hasNext() == false)) {
                return true;
            }
            if (source == nullptr || oth.source == nullptr) {
                return source == oth.source;
            }
            return source->getRecord() == oth.source->getRecord() &&
                   depth == oth.depth;
        }

        bool operator!=(const PostOrderIterator &oth) {
            if (source == nullptr && oth.depth == -1 &&
                (oth.source == nullptr ||
                 oth.source->hasNext() == false)) {
                return false;
            }
            if (oth.source == nullptr && depth == -1 &&
                (source == nullptr || source->hasNext() == false)) {
                return false;
            }
            if (source == nullptr || oth.source == nullptr) {
                return source != oth.source;
            }
            return !(source->getRecord() == oth.source->getRecord() &&
                     depth == oth.depth);
        }
    };

    PostOrderIterator beginPostOrderIterator() {
        return PostOrderIterator(source.clone());
    }

    PostOrderIterator endPostOrderIterator() {
        return PostOrderIterator(nullptr);
    }

    class PreOrderIterator {
    private:
        std::shared_ptr<SourceStrategy> source;
        int keyCount = sizeof...(Idx);
        int depth = 0;

        void next() {
            if (depth < keyCount + 1) {
                ++depth;
                if (depth == keyCount + 1 && source != nullptr &&
                    source->hasNext()) {
                    Key key = source->getRecord()->key;
                    Key nextKey = source->getNextRecord()->key;
                    depth = key.compare(nextKey) + 1;
                    readRecord();
                }
            }
        }

        void readRecord() {
            if (source != nullptr) {
                source->next();
            }
        }

    public:
        explicit PreOrderIterator(std::shared_ptr<SourceStrategy> source)
                : source(source) {}

        int getDepth() {
            return depth;
        }

        bool isLeaf() {
            return depth == keyCount;
        }

        Record &operator*() { return *(source->getRecord()); }

        Record *operator->() { return source->getRecord(); }

        PreOrderIterator operator++() {
            next();
            return *this;
        }

        PreOrderIterator operator++(int) {
            PreOrderIterator ret = *this;
            next();
            return ret;
        }

        bool operator==(const PreOrderIterator &oth) {
            if (source == nullptr && oth.depth == keyCount + 1 &&
                (oth.source == nullptr ||
                 oth.source->hasNext() == false)) {
                return true;
            }
            if (oth.source == nullptr && depth == keyCount + 1 &&
                (source == nullptr || source->hasNext() == false)) {
                return true;
            }
            if (source == nullptr || oth.source == nullptr) {
                return source == oth.source;
            }
            return source->getRecord() == oth.source->getRecord() &&
                   depth == oth.depth;
        }

        bool operator!=(const PreOrderIterator &oth) {
            if (source == nullptr && oth.depth == keyCount + 1 &&
                (oth.source == nullptr ||
                 oth.source->hasNext() == false)) {
                return false;
            }
            if (oth.source == nullptr && depth == keyCount + 1 &&
                (source == nullptr || source->hasNext() == false)) {
                return false;
            }
            if (source == nullptr || oth.source == nullptr) {
                return source != oth.source;
            }
            return !(source->getRecord() == oth.source->getRecord() &&
                     depth == oth.depth);
        }
    };

    PreOrderIterator beginPreOrderIterator() {
        return PreOrderIterator(source.clone());
    }

    PreOrderIterator endPreOrderIterator() {
        return PreOrderIterator(nullptr);
    }

};

#endif //DIPLOM_TREE_H
