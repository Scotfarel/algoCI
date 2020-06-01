//
//  Created by ivan on 14/05/20.
//
//  Задача 4. Использование АВЛ-дерева
//
//  Порядковые статистики. Дано число N и N строк. Каждая строка содержит команду добавления или удаления
//  натуральных чисел, а также запрос на получение k-ой порядковой статистики.
//  Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным
//  числом “-A”. Запрос на получение k-ой порядковой статистики задается числом k.
//  Требования: скорость выполнения запроса - O(log n).
//

#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>

template<class T>
struct DefaultComparator {
    int operator()(const T &l, const T &r) const {
        if (l < r) return -1;
        if (l > r) return 1;
        return 0;
    }
};

template<class Value, class Comparator=DefaultComparator<Value>>
class AVLTree {
    struct Node {
        Node* left;
        Node* right;

        Value value;
        int height;
        int count;

        explicit Node(const Value &value) : left(nullptr), right(nullptr), value(value), height(1), count(1) {};
    };

public:
    explicit AVLTree(Comparator comp = Comparator()) : root(nullptr), comp(comp) {}

    ~AVLTree() {
        deleteTree(root);
    }

    void deleteTree(Node *node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    void insert(const Value &value) {
        root = insertAux(root, value);
    }

    void erase(const Value &value) {
        root = eraseAux(root, value);
    }

    int findKthStatistic(int kth) {
        return findKthStatisticAux(root, kth + 1);
    }

private:
    Node* root;
    Comparator comp;

    Node* balance(Node *node) {
        fixHeight(node);
        fixCount(node);

        int bf = balanceFactor(node);

        if (bf == 2) {
            if (balanceFactor(node->right) < 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        } else if (bf == -2) {
            if (balanceFactor(node->left) > 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }
        return node;
    }

    int balanceFactor(Node* node) {
        return height(node->right) - height(node->left);
    }

    void fixHeight(Node* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    void fixCount(Node* node) {
        node->count = count(node->left) + count(node->right) + 1;
    }

    int height(Node* node) {
        if (!node) {
            return 0;
        }
        return node->height;
    }

    int count(Node* node) {
        if (!node) {
            return 0;
        }
        return node->count;
    }

    Node* insertAux(Node* node, const Value &value) {
        if (!node) {
            return new Node(value);
        }
        int comp_res = comp(value, node->value);
        if (comp_res == -1) {
            node->left = insertAux(node->left, value);
        } else {
            node->right = insertAux(node->right, value);
        }
        return balance(node);
    }

    Node* eraseAux(Node* node, const Value &value) {
        if (!node)
            return nullptr;

        int comp_res = comp(value, node->value);
        if (comp_res == -1) {
            node->left = eraseAux(node->left, value);
        } else if (comp_res == 1) {
            node->right = eraseAux(node->right, value);
        } else {
            Node* min_node = node;
            Node* left = node->left;
            Node* right = node->right;

            delete node;

            if (!right) {
                return left;
            }

            min_node->right = findAndDelete(right, min_node);
            min_node->left = left;

            return balance(min_node);
        }

        return balance(node);
    }

    Node* findAndDelete(Node* node, Node* result) {
        if (!node->left) {
            result = new Node(node->value);
            return node->right;
        }

        node->left = findAndDelete(node->left, result);
        return balance(node);

    }

    Node* rotateRight(Node* node) {
        Node* newNode = node->left;

        node->left = newNode->right;
        newNode->right = node;

        fixCount(node);
        fixCount(newNode);
        fixHeight(node);
        fixHeight(newNode);


        return newNode;
    }

    Node* rotateLeft(Node* node) {
        Node* newNode = node->right;

        node->right = newNode->left;
        newNode->left = node;

        fixCount(node);
        fixCount(newNode);
        fixHeight(node);
        fixHeight(newNode);

        return newNode;
    }

    int findKthStatisticAux(Node* node, const int kth) {
        int child = (!node->right) ? count(node) : (count(node) - count(node->right));

        if (kth == child) {                     //  The answer we're looking for is the value of node
            return node->value;
        } else if (kth > child) {               //  Ignore left sub-tree, smaller than the kth smallest
            return findKthStatisticAux(node->right, kth - child);
        } else {                                //  Kth in the left sub-tree
            return findKthStatisticAux(node->left, kth);
        }
    }
};


int run(std::istream &input, std::ostream &output) {
    int numsTotal = 0;
    input >> numsTotal;

    std::vector<int> nums;
    std::vector<int> kths;


    for (int i = 0; i < numsTotal; i++) {
        int num = 0;
        int kth = 0;
        input >> num >> kth;

        nums.push_back(num);
        kths.push_back(kth);
    }

    AVLTree<int> avlTree;
    for (int i = 0; i < numsTotal; i++) {
        if (nums.at(i) < 0)
            avlTree.erase(-nums.at(i));
        else
            avlTree.insert(nums.at(i));
        output << avlTree.findKthStatistic(kths.at(i)) << " ";
    }

    return 0;
}

void testAVL() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 40 0 10 1 4 1 -10 0 50 2";
        run(input, output);
        assert(output.str() == "40 40 10 4 50 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 12 0 13 0 -12 0 12 0 7 0";
        run(input, output);
        assert(output.str() == "12 12 13 12 7 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 10 0 11 1 12 2 -11 1 1 0";
        run(input, output);
        assert(output.str() == "10 11 12 12 1 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 50 0 30 0 40 1 35 1 -40 2 -50 1";
        run(input, output);
        assert(output.str() == "50 30 40 35 50 35 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 10 0 11 0 9 1 12 1 8 2 13 2 7 3 13 3 6 4 14 4";
        run(input, output);
        assert(output.str() == "10 10 10 10 10 10 10 10 10 10 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 100 0 70 0 55 1 -70 1 60 1 -100 0";
        run(input, output);
        assert(output.str() == "100 70 70 100 60 55 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "7 10 0 8 0 12 2 -8 0 -10 0 15 1 -12 0";
        run(input, output);
        assert(output.str() == "10 8 12 10 12 15 15 ");
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "10 1 0 2 1 -2 0 2 1 -2 0 2 1 -2 0 2 1 -2 0 2 0";
        run(input, output);
        assert(output.str() == "1 2 1 2 1 2 1 2 1 1 ");
    }
};

int main() {
    //run(std::cin, std::cout);
    testAVL();
}
