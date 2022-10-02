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
Node *reverseList(Node *node) {
    Node *head;
    if (node->has_child()) {
        auto head_child = reverseList(node->next);
        head_child->appendNode(node->val);
        head = head_child;
    } else {
        head = new Node(node);
    }
    return head;
}

//L: [head of reversed, last element of reversed]
std::pair<Node *, Node *> reverseListOpt(Node *node) {
    Node *head, *tail;
    if (node->has_child()) {
        auto [new_head_child, tail_child] = reverseListOpt(node->next);
        tail_child->next = node;
        tail = node;
        tail->next = nullptr;
    } else {
        head = tail = new Node(node);
    }
    return {head, tail};
}


int main() {
    Node *root = new Node(1);
    Node *node = root = root;

    std::cout << "Creating list ..." << std::endl;
    for (int i = 2; i <= 4000; i++) {
        node->appendNode(i);
        node = node->next;
    }

    //print(root);
    //std::cout << std::endl;
    std::cout << "Reversing list ..." << std::endl;
    auto ret = reverseList(root);
    //print(ret);
    delete root;
    delete ret;

    return 0;
}
