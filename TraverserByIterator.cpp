
#include <iostream>
#include <vector>

struct Node {
    int data;
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;
    std::vector<Node> childrens;

    Node(int data) : data(data) {}

    void addLeft(Node *left) {
        this->left = left;
        left->parent = this;
    }

    void addRight(Node *right) {
        this->right = right;
        right->parent = this;
    }
};

class Tree {
private:
    Node *root;
public:
    Tree(Node *root) : root(root) {}

    class BaseIterator {
    protected:
        Node *cur;

        BaseIterator(Node *root) : cur(root) {}

        virtual void next() {};

    public:
        Node &operator*() { return *cur; }

        Node *operator->() { return cur; }

        BaseIterator operator++() {
            next();
            return *this;
        }

        BaseIterator operator++(int) {
            BaseIterator ret = *this;
            next();
            return ret;
        }

        bool operator==(const BaseIterator &oth) const {
            return cur == oth.cur;
        }

        bool operator!=(const BaseIterator &oth) const {
            return !(cur == oth.cur);
        }
    };

    class PreOrderIterator : public BaseIterator {
    protected:
        void next() override {
            Node *prev = nullptr;
            while (cur != nullptr) {
                if (cur->left != nullptr && prev == nullptr) {
                    cur = cur->left;
                    break;
                } else if (cur->right != nullptr && cur->right != prev) {
                    cur = cur->right;
                    break;
                } else {
                    prev = cur;
                    cur = cur->parent;
                }
            }
        }

    public:
        explicit PreOrderIterator(Node *root) : BaseIterator(root) {}
    };

    class PostOrderIterator : public BaseIterator {
    private:
        static Node *moveLeftRight(Node *node) {
            while (node != nullptr) {
                if (node->left != nullptr) {
                    node = node->left;
                } else if (node->right != nullptr) {
                    node = node->right;
                } else {
                    break;
                }
            }
            return node;
        }

    protected:
        void next() override {
            if (cur != nullptr) {
                Node *prev = cur;
                cur = cur->parent;
                if (cur != nullptr && cur->left == prev &&
                    cur->right != nullptr) {
                    cur = moveLeftRight(cur->right);
                }
            }
        }

    public:
        explicit PostOrderIterator(Node *root) : BaseIterator(
                moveLeftRight(root)) {}
    };

    PreOrderIterator beginPreOrderIterator() {
        return PreOrderIterator(root);
    }

    PreOrderIterator endPreOrderIterator() {
        return PreOrderIterator(nullptr);
    }

    PostOrderIterator beginPostOrderIterator() {
        return PostOrderIterator(root);
    }

    PostOrderIterator endPostOrderIterator() {
        return PostOrderIterator(nullptr);
    }
};

//int main() {
//    Node *root = new Node(0);
//    root->addLeft(new Node(1));
//    root->left->addLeft(new Node(2));
//    root->left->left->addRight(new Node(3));
//    root->left->addRight(new Node(4));
//    root->left->right->addLeft(new Node(5));
//    root->addRight(new Node(6));
//    root->right->addRight(new Node(7));
//    root->right->right->addLeft(new Node(8));
//    root->right->right->addRight(new Node(9));
//    Tree tree(root);
//    for (auto preOrderIt = tree.beginPreOrderIterator(); preOrderIt != tree.endPreOrderIterator(); ++preOrderIt) {
//        std::cout << preOrderIt->data << " ";
//    }
//    std::cout << std::endl;
//    for (auto postOrderIt = tree.beginPostOrderIterator(); postOrderIt != tree.endPostOrderIterator(); ++postOrderIt) {
//        std::cout << postOrderIt->data << " ";
//    }
//    std::cout << std::endl;
//    auto it1 = tree.beginPreOrderIterator();
//    auto it2 = ++it1;
//    auto it3 = it1++;
//    std::cout << it1->data << " " << it2->data << " " << it3->data << std::endl;
//    return 0;
//}