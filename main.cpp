#include <iostream>

class Node {
public:
    explicit Node(int _val) : val(_val) {}

    explicit Node(const Node *node) : val(node->val) {}

    void appendNode(int vval) {
        auto tmp = this;
        while (tmp->next != nullptr)
            tmp = tmp->next;
        tmp->next = new Node(vval);
        tmp->next->next = nullptr;
    }

    int val;
    Node *next = nullptr;

    bool has_child() const {
        return next != nullptr;
    }

    ~Node() {
        //std::cout << "Deleting: " << val << std::endl;
        delete next;
        next = nullptr;
    }
};

class List {
private:
    int sz = 0;

    void _copylist(const List &l, int skip = 0) {
        flush();

        if (abs(skip) > l.size())
            throw std::out_of_range("abs(skip) > l.size()");

        auto tmp_l = l.head;
        int pos = 0;
        while (skip > 0) {
            tmp_l = tmp_l->next;
            skip--;
            pos++;
        }

        Node *tmp;
        if (pos < l.size() + skip) {
            head = tail = tmp = new Node(tmp_l);
            pos++;
        }

        while (pos < l.size() + skip) {
            tmp->next = new Node(tmp_l->next);
            tmp_l = tmp_l->next;
            tmp = tmp->next;
            tail = tmp;
            pos++;
        }
    }

public:

    Node *head = nullptr;
    Node *tail = nullptr;

    List() = default;

    explicit List(const int size) : sz(size) {
        std::srand(time(nullptr));
        if (size > 0) {
            int curr = 1;
            auto tmp = head = tail = new Node(std::rand() % 10);
            while (curr < size) {
                curr++;
                tmp->next = new Node(std::rand() % 10);
                tmp = tmp->next;
                tail = tmp;
            }
        }
    };

    explicit List(const Node *node) : head(new Node(node)), tail(head), sz(1) {}

    List(const List &l) {
        _copylist(l);
        sz = l.size();
    }

    //param >0 shortens from the top
    //param <0 shortens from the bottom
    List(const List &l, int param) {
        _copylist(l, param);
        sz = l.size() - abs(param);
    }

    List &operator=(const List &l) {
        if (&l != this) {
            _copylist(l);
            sz = l.size();
        }
        return *this;
    }

    void flush() {
        Node *prev, *curr = head;
        while (curr != nullptr) {
            prev = curr;
            curr = curr->next;
            delete prev;
        }
        head = tail = nullptr;
        sz = 0;
    }

    void append(Node *node) {
        auto tmp = new Node(node);
        tail->next = tmp;
        tail = tmp;
        sz++;
    }

    ~List() {
        flush();
    }

    [[nodiscard]] int size() const {
        return sz;
    }

    [[nodiscard]] bool just_one_left() const {
        return size() == 1;
    }
};

void print(Node *node) {
    if (node != nullptr) {
        std::cout << node->val << " ";
        print(node->next);
    }
}

//L: [head of reversed]

class L {
public:
    Node *head;
    Node *tail;
};

// reversed instead of reverse, it makes more clear that what you have from children it is an already reversed list
L reversedList(Node *node) {
    L L;
    if (node->has_child()) {
        auto Lchild = reversedList(node->next);
        auto old_tail = Lchild.tail;
        auto new_tail = new Node(node->val);
        old_tail->next = new_tail;
        L.tail = new_tail;
        L.head = Lchild.head;
    } else {
        L.head = L.tail = new Node(node->val);
    }
    return L;
}

L sortedList(Node *node) {
    L L;
    if (node->has_child()) {
        auto Lchild = sortedList(node->next);
        if (Lchild.head->val > node->val) {
            L.head = new Node(node->val);
            L.head->next = Lchild.head;
        } else {
            auto tmp = Lchild.head;
            while (tmp->has_child() && tmp->next->val < node->val)
                tmp = tmp->next;

            auto post = tmp->next;
            tmp->next = new Node(node->val);
            tmp->next->next = post;

            L.head = Lchild.head;
        }
    } else {
        L.head = new Node(node->val);
    }
    return L;
}

int main() {
    std::srand(time(nullptr));
    Node *root = new Node(1);
    Node *node = root = root;

    std::cout << "Creating list ..." << std::endl;
    for (int i = 2; i <= 5; i++) {
        node->appendNode(std::rand() % 30);
        node = node->next;
    }

    print(root);
    std::cout << std::endl;
    std::cout << "Reversing list ..." << std::endl;
    auto ret = reversedList(root);
    std::cout << "Done" << std::endl;
    print(ret.head);
    std::cout << std::endl;

    std::cout << "Sorting list ..." << std::endl;
    auto ret2 = sortedList(ret.head);
    std::cout << "Done" << std::endl;
    print(ret2.head);
    std::cout << std::endl;

    delete root;
    delete ret.head;
    delete ret2.head;

    return 0;
}
