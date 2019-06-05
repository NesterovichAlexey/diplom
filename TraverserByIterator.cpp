
#include <iostream>
#include <vector>

struct BinaryNode {
    int data;
    BinaryNode *parent = nullptr;
    BinaryNode *left = nullptr;
    BinaryNode *right = nullptr;

    BinaryNode(int data) : data(data) {}

    void addLeft(BinaryNode *left) {
        this->left = left;
        left->parent = this;
    }

    void addRight(BinaryNode *right) {
        this->right = right;
        right->parent = this;
    }
};

class BinaryTree {
private:
    BinaryNode *root;
public:
    BinaryTree(BinaryNode *root) : root(root) {}

    class BaseIterator {
    protected:
        BinaryNode *node;

        BaseIterator(BinaryNode *root) : node(root) {}

        virtual void next() {};

    public:
        BinaryNode &operator*() { return *node; }

        BinaryNode *operator->() { return node; }

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
            return node == oth.node;
        }

        bool operator!=(const BaseIterator &oth) const {
            return node != oth.node;
        }
    };

    class PreOrderIterator : public BaseIterator {
    protected:
        void next() override {
            BinaryNode *prev = nullptr;
            while (node != nullptr) {
                if (node->left != nullptr && prev == nullptr) {
                    node = node->left;
                    break;
                } else if (node->right != nullptr && node->right != prev) {
                    node = node->right;
                    break;
                } else {
                    prev = node;
                    node = node->parent;
                }
            }
        }

    public:
        explicit PreOrderIterator(BinaryNode *root) : BaseIterator(root) {}
    };

    class PostOrderIterator : public BaseIterator {
    private:
        static BinaryNode *moveLeftRight(BinaryNode *node) {
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
            if (node != nullptr) {
                BinaryNode *prev = node;
                node = node->parent;
                if (node != nullptr && node->left == prev &&
                    node->right != nullptr) {
                    node = moveLeftRight(node->right);
                }
            }
        }

    public:
        explicit PostOrderIterator(BinaryNode *root) :
                BaseIterator(moveLeftRight(root)) {}
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

void printBinaryTreeUseIterator() {
    std::vector<BinaryNode*> nodes;
    for (int i = 0; i < 10; ++i) {
        nodes.push_back(new BinaryNode(i));
    }
    auto *root = nodes[0];
    root->addLeft(nodes[1]);
    root->left->addLeft(nodes[2]);
    root->left->left->addRight(nodes[3]);
    root->left->addRight(nodes[4]);
    root->left->right->addLeft(nodes[5]);
    root->addRight(nodes[6]);
    root->right->addRight(nodes[7]);
    root->right->right->addLeft(nodes[8]);
    root->right->right->addRight(nodes[9]);
    BinaryTree tree(root);
    std::cout << "Binary tree pre order\n";
    for (auto it = tree.beginPreOrderIterator(); it != tree.endPreOrderIterator(); ++it) {
        std::cout << it->data << " ";
    }
    std::cout << std::endl;
    std::cout << "Binary tree post order\n";
    for (auto it = tree.beginPostOrderIterator(); it != tree.endPostOrderIterator(); ++it) {
        std::cout << it->data << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 10; ++i) {
        delete nodes[i];
    }
}

struct Node {
    int data;
    Node *parent = nullptr;
    std::vector<Node*> children;

    Node(int data) : data(data) {}

    void addNode(Node *node) {
        children.push_back(node);
        node->parent = this;
    }
};

class Tree {
private:
    Node *root;
public:
    Tree(Node *root) : root(root) {}

    class BaseIterator {
    protected:
        Node *node;

        BaseIterator(Node *root) : node(root) {}

        virtual void next() {};

    public:
        Node &operator*() { return *node; }

        Node *operator->() { return node; }

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
            return node == oth.node;
        }

        bool operator!=(const BaseIterator &oth) const {
            return node != oth.node;
        }
    };

    class PreOrderIterator : public BaseIterator {
    protected:
        void next() override {
            if (node != nullptr) {
                Node *prev = nullptr;
                if (node->children.empty() == false) {
                    node = node->children[0];
                    return;
                } else {
                    prev = node;
                    node = node->parent;
                }
                while (node != nullptr) {
                    int idx = 0;
                    while (idx < node->children.size() &&
                           node->children[idx] != prev) {
                        ++idx;
                    }
                    if (idx == node->children.size() - 1) {
                        prev = node;
                        node = node->parent;
                    } else {
                        node = node->children[idx + 1];
                        break;
                    }
                }
            }
        }

    public:
        explicit PreOrderIterator(Node *root) : BaseIterator(root) {}
    };

    class PostOrderIterator : public BaseIterator {
    private:
        void moveToLeaf() {
            while (node != nullptr && node->children.empty() == false)
                node = node->children[0];
        }
    protected:
        void next() override {
            if (node != nullptr) {
                Node *prev = node;
                node = node->parent;
                if (node != nullptr) {
                    int idx = 0;
                    while (idx < node->children.size() &&
                           node->children[idx] != prev)
                        ++idx;
                    if (idx != node->children.size() - 1) {
                        node = node->children[idx + 1];
                        moveToLeaf();
                    }
                }
            }

        }

    public:
        explicit PostOrderIterator(Node *root) : BaseIterator(root) {
            moveToLeaf();
        }
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

void printTreeUseIterator() {
    std::vector<Node*> nodes;
    for (int i = 0; i < 10; ++i) {
        nodes.push_back(new Node(i));
    }
    nodes[0]->addNode(nodes[1]);
    nodes[0]->addNode(nodes[2]);
    nodes[0]->addNode(nodes[3]);
    nodes[1]->addNode(nodes[4]);
    nodes[1]->addNode(nodes[5]);
    nodes[2]->addNode(nodes[6]);
    nodes[3]->addNode(nodes[7]);
    nodes[3]->addNode(nodes[8]);
    nodes[7]->addNode(nodes[9]);
    Tree tree(nodes[0]);
    std::cout << "Tree pre order\n";
    for (auto it = tree.beginPreOrderIterator(); it != tree.endPreOrderIterator(); ++it) {
        std::cout << it->data << " ";
    }
    std::cout << std::endl;
    std::cout << "Tree post order\n";
    for (auto it = tree.beginPostOrderIterator(); it != tree.endPostOrderIterator(); ++it) {
        std::cout << it->data << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < 10; ++i) {
        delete nodes[i];
    }
}

//int main() {
//    printBinaryTreeUseIterator();
//    printTreeUseIterator();
//    return 0;
//}