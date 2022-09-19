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

    void _copylist(const List &l, int skip = 0) {
        auto tmp_l = l.head;
        int pos = 0;
        while (skip > 0) {
            tmp_l = tmp_l->next;
            skip--;
            pos++;
        }

        Node *tmp;
        if (pos <= l.size() + skip) {
            head = tail = tmp = new Node(tmp_l);
            pos++;
        }

        while (pos <= l.size() + skip) {
            tmp->next = new Node(tmp_l->next->val);
            tmp_l = tmp_l->next;
            tmp = tmp->next;
            tail = tmp;
            pos++;
        }
    }

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

    void append(Node *node) {
        auto tmp = head;
        while (tmp->next != nullptr)
            tmp = tmp->next;
        tmp->next = new Node(node->val);
        sz++;
    }

    ~List() {
        delete head;
    }

    [[nodiscard]] int size() const {
        return sz;
    }

};

bool last(Node *node) {
    return node->next == nullptr;
}

void printList(Node *node) {
    if (last(node)) {
        std::cout << node->val << " ";
    } else {
        std::cout << node->val << " ";
        printList(node->next);
    }
}

int sumElements(Node *node) {
    int sum;
    if (last(node)) {
        sum = node->val;
    } else {
        auto ret = sumElements(node->next);
        sum = ret + node->val;
    }
    return sum;
}

List reverseList(List list) {
    List reversed;
    if (list.size() == 1) {
        reversed = list;
    } else {
        reversed = reverseList(List(list, 1));
        reversed.append(list.head);
    }
    return reversed;
}


int main() {
    List list(4);
    list = List(list, 1);
    auto l2 = List(list, 1);
    printList(list.head);
    std::cout << std::endl;
    auto pair = reverseList(list);
    printList(pair.head);

    return 0;
}
