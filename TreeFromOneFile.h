#ifndef DIPLOM_TREEFROMONEFILE_H
#define DIPLOM_TREEFROMONEFILE_H

#include <fstream>
#include <tuple>

#include "tuple_io.h"

template<typename KeyTuple, typename ValueType>
class TreeFromManyFiles {
private:
    std::string dirPath;

    class BaseIterator {
    protected:
        KeyTuple nextKeys;
        ValueType nextValue;
        std::ifstream *in = nullptr;
        bool needRead = true;
        bool isEnd = false;

        BaseIterator() {
            isEnd = true;
            depth = -1;
        }

        virtual void next() {};

    public:
        KeyTuple keys;
        ValueType value;
        int depth;

        BaseIterator(const std::string &filePath, int startDepth) {
            in = new std::ifstream(filePath);
            if (*in >> keys) {
                *in >> value;
            } else {
                isEnd = true;
            }
            depth = startDepth;
        }

        BaseIterator(const BaseIterator &it) {
            keys = it.keys;
            value = it.value;
            nextKeys = it.nextKeys;
            nextValue = it.nextValue;
            in = std::move(it.in);
            needRead = it.needRead;
            isEnd = it.isEnd;
            depth = it.depth;
        }

        bool isLeaf() { return depth == std::tuple_size<KeyTuple>::value; }

        ValueType &operator*() { return value; }

        ValueType *operator->() { return *value; }

        BaseIterator operator++() {
            next();
            return *this;
        }

        BaseIterator operator++(int) {
            BaseIterator ret = *this;
            next();
            return ret;
        }

        bool operator==(BaseIterator oth) const {
            return (isEnd == oth.isEnd && depth == oth.depth && depth == -1) ||
                   (keys == oth.keys && value == oth.value);
        }

        bool operator!=(BaseIterator oth) const {
            return !((isEnd == oth.isEnd && depth == oth.depth && depth == -1) ||
                     (keys == oth.keys && value == oth.value));
        }
    };

public:
    TreeFromManyFiles(std::string filePath) {
        this->dirPath = filePath;
    }

    ~TreeFromManyFiles() {
    }

    class PreOrderIterator : public BaseIterator {
    protected:
        PreOrderIterator() : BaseIterator() {}

        virtual void next() {
            if (BaseIterator::needRead) {
                if (*BaseIterator::in >> BaseIterator::nextKeys) {
                    *BaseIterator::in >> BaseIterator::nextValue;
                } else {
                    BaseIterator::isEnd = true;
                }
                BaseIterator::needRead = false;
            }
            if (BaseIterator::depth != -1 && BaseIterator::depth < std::tuple_size<KeyTuple>::value) {
                ++BaseIterator::depth;
            } else {
                if (BaseIterator::isEnd) {
                    BaseIterator::depth = -1;
                } else {
                    int i = compare(BaseIterator::keys, BaseIterator::nextKeys);
                    BaseIterator::depth = i + 1;
                    BaseIterator::keys = BaseIterator::nextKeys;
                    BaseIterator::value = BaseIterator::nextValue;
                    BaseIterator::needRead = true;

                }
            }
        }

    public:
        PreOrderIterator(const std::string &filePath) : BaseIterator(filePath, 0) {}

        PreOrderIterator(const PreOrderIterator &it) : BaseIterator(it) {}

        static PreOrderIterator end() {
            return PreOrderIterator();
        }
    };

    PreOrderIterator preOrderBegin() {
        return PreOrderIterator(dirPath);
    }

    PreOrderIterator preOrderEnd() {
        return PreOrderIterator::end();
    }

    class PostOrderIterator : public BaseIterator {
    protected:
        PostOrderIterator() : BaseIterator() {}

        virtual void next() {
            if (BaseIterator::needRead) {
                if (*BaseIterator::in >> BaseIterator::nextKeys) {
                    *BaseIterator::in >> BaseIterator::nextValue;
                } else {
                    BaseIterator::isEnd = true;
                }
                BaseIterator::needRead = false;
            }
            if (BaseIterator::isEnd) {
                if (BaseIterator::depth > 0) {
                    --BaseIterator::depth;
                } else {
                    BaseIterator::depth = -1;
                }
            } else {
                int i = compare(BaseIterator::keys, BaseIterator::nextKeys);
                if (i + 1 == BaseIterator::depth) {
                    BaseIterator::depth = std::tuple_size<KeyTuple>::value;
                    BaseIterator::keys = BaseIterator::nextKeys;
                    BaseIterator::value = BaseIterator::nextValue;
                    BaseIterator::needRead = true;
                } else {
                    --BaseIterator::depth;
                }
            }
        }

    public:
        PostOrderIterator(const std::string &filePath) : BaseIterator(filePath, std::tuple_size<KeyTuple>::value) {}

        PostOrderIterator(const PostOrderIterator &it) : BaseIterator(it) {}

        static PostOrderIterator end() {
            return PostOrderIterator();
        }
    };

    PostOrderIterator postOrderBegin() {
        return PostOrderIterator(dirPath);
    }

    PostOrderIterator postOrderEnd() {
        return PostOrderIterator::end();
    }
};

#endif //DIPLOM_TREEFROMONEFILE_H
