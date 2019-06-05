//#ifndef DIPLOM_MANYSOURCESTRATEGY_H
//#define DIPLOM_MANYSOURCESTRATEGY_H
//
//#include "FileStrategy.h"
//#include "Record.h"
//#include "Key.h"
//#include <tuple>
//
//template<typename... Files>
//class ManySourceStrategy;
//
//template<typename... Record>
//class ManySourceStrategy<FileStrategy<Record>...> {
//private:
//    typedef std::tuple<Record...> RecordTuple;
//    typedef std::tuple<FileStrategy<Record>...> FileTuple;
//    FileTuple files;
//public:
//    explicit ManySourceStrategy(FileTuple files) : files(files) {}
//
////    template<int Depth>
////    class PostOrderIterator {
////    private:
////        FileTuple *files;
////        int fileCount = sizeof...(Record);
////
////        void next() {
////            if (Depth > -2) {
////
////                if (endFile == false) {
////                    Key key = record.key();
////                    Key nextKey = nextRecord.key();
////                    int diff = key.compare(nextKey);
////                    if (diff == depth) {
////                        readRecord();
////                        depth = fileCount;
////                    }
////                }
////            }
////        }
////
////        template<int Idx>
////        void readRecord() {
////            if (files != nullptr) {
////                std::tuple_element_t<Idx> file = std::get<Idx>(files);
////                file->next();
////            }
////        }
////
////    public:
////        explicit PostOrderIterator(FileTuple *files) : files(files) {
////            readRecord();
////            readRecord();
////        }
////
////        int getDepth() {
////            return depth;
////        }
////
////        bool isLeaf() {
////            return Depth == fileCount - 1;
////        }
////
////        auto &operator*() { return *(std::get<Depth>(files).record); }
////
////        auto *operator->() { return std::get<Depth>(files).record; }
////
////        PostOrderIterator operator++() {
////            next();
////            return *this;
////        }
////
////        PostOrderIterator operator++(int) {
////            PostOrderIterator ret = *this;
////            next();
////            return ret;
////        }
////
////        bool operator==(const PostOrderIterator &oth) const { //TODO
////            return record == oth.record && depth == oth.depth;
////        }
////
////        bool operator!=(const PostOrderIterator &oth) const {
////            return !(record == oth.record && depth == oth.depth);
////        }
////    };
////
////    auto beginPostOrderIterator() {
////        return PostOrderIterator<sizeof...(Record) - 1>(files);
////    }
////
////    auto endPostOrderIterator() {
////        return PostOrderIterator(nullptr);
////    }
//
//    template<int Depth>
//    class PreOrderIterator {
//    private:
//        int fileCount = sizeof...(Record);
//
//        decltype(auto) next() {
//            if (files == nullptr) {
//                return this;
//            }
//            if (Depth == 0) {
//                if (std::get<0>(*files).record != nullptr)
//                    return new PreOrderIterator<Depth + 1>(files);
//                else
//                    return new PreOrderIterator<-1>(nullptr);
//            } else if (Depth > 0) {
//                auto file = std::get<Depth>(*files);
//                if (Depth == fileCount || std::get<Depth + 1>(*files).record == nullptr) {
//                    file.next();
//                    return (new PreOrderIterator<Depth - 1>(files))->next();
//                }
//                auto nextFile = std::get<Depth + 1>(*files);
//                auto key = file.record->key;
//                auto nextKey = nextFile.record->key;
//                if (key.compare(nextKey) == key.size) {
//                    return new PreOrderIterator<Depth + 1>(files);
//                } else {
//                    file.next();
//                    return (new PreOrderIterator<Depth - 1>(files))->next();
//                }
//            }
//
//            return this;
//        }
//
//        template<int Idx>
//        void readRecord() {
//            if (files != nullptr && Idx >= 0) {
//                auto file = std::get<Idx < 0 ? 0 : Idx>(*files);
//                file.next();
//            }
//        }
//
//        template<int Idx>
//        void readAll() {
//            readRecord<Idx>();
//            readAll<Idx - 1>();
//        }
//
//        template<>
//        void readAll<0>() {
//            readRecord<0>();
//        }
//
//        void readAll() {
//            readAll<sizeof...(Record) - 1>();
//        }
//
//    public:
//        FileTuple *files;
//
//        explicit PreOrderIterator(FileTuple *files) : files(files) {
//            if (files != nullptr) {
//                readAll();
//                readAll();
//            }
//        }
//
//        int getDepth() {
//            return Depth;
//        }
//
//        bool isLeaf() {
//            return Depth == fileCount;
//        }
//
//        auto &operator*() { return Depth == 0 ? nullptr : *(std::get<Depth == 0 ? 0 : Depth - 1>(*files).record); }
//
//        auto *operator->() { return Depth == 0 ? nullptr : std::get<Depth == 0 ? 0 : Depth - 1>(*files).record; }
//
//        PreOrderIterator operator++() {
//            return next();
//        }
//
////        auto operator++(int) {
////            PreOrderIterator ret = *this;
////            next();
////            return ret;
////        }
//
//        template<int OtherDepth>
//        bool operator==(const PreOrderIterator<OtherDepth> &oth) const {
//            return files == oth.files;
//        }
//
//        template<int OtherDepth>
//        bool operator!=(const PreOrderIterator<OtherDepth> &oth) const {
//            return !(files != oth.files);
//        }
//    };
//
//    PreOrderIterator<0> beginPreOrderIterator() {
//        return PreOrderIterator<0>(&files);
//    }
//
//    PreOrderIterator<-1> endPreOrderIterator() {
//        return PreOrderIterator<-1>(nullptr);
//    }
//
//    PreOrderIterator<> it = PreOrderIterator<2>(files);
//
//};
//
//#endif //DIPLOM_MANYSOURCESTRATEGY_H
