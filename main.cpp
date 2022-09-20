#include <iostream>

class Node {
public:
    explicit Node(int _val) : val(_val) {}

    explicit Node(const Node *node) : val(node->val) {}

    int val;
    Node *next = nullptr;
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

void print(List &list) {
    auto node = list.head;
    while (node != nullptr) {
        std::cout << node->val << " ";
        node = node->next;
    }
}

List reverseList(const List &list) {
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
