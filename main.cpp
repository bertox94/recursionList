#include <iostream>

class Node {
public:
    explicit Node(int _val) : val(_val) {}

    explicit Node(Node *node) : val(node->val) {}

    int val;
    Node *next = nullptr;

    ~Node() {
        delete next;
    }
};

class List {
private:
    int sz = 0;

    void _copylist(const List &l, int skip = 0) {
        empty();

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
            tmp->next = new Node(tmp_l->next->val);
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

    explicit List(int size) : sz(size) {
        if (size > 0) {
            int curr = 1;
            auto tmp = head = tail = new Node(curr);
            while (curr < size) {
                curr++;
                tmp->next = new Node(curr);
                tmp = tmp->next;
                tail = tmp;
            }
        }
    };

    explicit List(Node *node) : head(new Node(node->val)), tail(head), sz(1) {}

    List(const List &l) {
        _copylist(l);
        sz = l.size();
    }

    //param >0 shortens from the top
    //param <0 shortens from the bottom
    List(List &l, int param) {
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

    void empty() {
        delete head;
        head = tail = nullptr;
        sz = 0;
    }

    void append(Node *node) {
        auto tmp = new Node(node->val);
        tail->next = tmp;
        tail = tmp;
        sz++;
    }

    ~List() {
        delete head;
    }

    [[nodiscard]] int size() const {
        return sz;
    }

    bool just_one_left() const {
        return size() == 1;
    }
};

void print(List &list) {
    auto node = list.head;
    while (node != nullptr) {
        std::cout << node->val << " ";
        node = node->next;
    }
}

List reverseList(List list) {
    List reversed;
    if (list.just_one_left()) {
        reversed = list;
    } else {
        reversed = reverseList(List(list, 1));
        reversed.append(list.head);
    }
    return reversed;
}


int main() {
    List list(5);
    print(list);
    std::cout << std::endl;
    auto ret = reverseList(list);
    print(ret);

    return 0;
}
