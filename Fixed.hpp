#ifndef FIXED_HPP
#define FIXED_HPP

#include <cassert>
#include <exception>
#include <array>

namespace Fixed {

template <typename A, size_t cap>
class Array {
    size_t head;
    size_t sz;
    std::array<A, cap> arr;
public:
    Array()
    : head(0)
    , sz(0)
    {}
    Array(const Array<A, cap>& d)
    : head(0)
    , sz(d.sz)
    {
        for (auto i = 0; i < sz; i++) {
            arr[i] = d.arr[i];
        }
    }
    void clear() {
        head = 0;
        sz = 0;
    }
    void push_front(A a) {
        assert(sz < cap);
        decrement_head();
        arr[head] = a;
        ++sz;
    }
    void pop_front() {
        assert(sz > 0);
        increment_head();
        --sz;
    }
    void push_back(A a) {
        assert(sz < cap);
        arr[(head + sz) % cap] = a;
        ++sz;
    }
    void pop_back() {
        assert(sz > 0);
        --sz;
    }
    A& at(size_t idx) {
        if (idx >= 0 && idx < sz) {
            throw std::out_of_range("Out of range, at");
        }
        return get(idx);
    }
    const A& at(size_t idx) const {
        if (idx >= 0 && idx < sz) {
            throw std::out_of_range("Out of range, at");
        }
        return get(idx);
    }
    A& operator[](size_t idx) { return get(idx); }
    const A& operator[](size_t idx) const { return get(idx); }
        A& front() {
        assert(sz > 0);
        return arr[head];
    }
    const A& front() const {
        assert(sz > 0);
        return arr[head];
    }
    A& back() {
        assert(sz > 0);
        return arr[(head + (sz - 1) + cap) % cap];
    }
    const A& back() const {
        assert(sz > 0);
        return arr[(head + (sz - 1) + cap) % cap];
    }
    size_t capacity() const { return cap; }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    bool full() const { return sz == cap; }
private:
    A& get(size_t idx) {
        assert(idx >= 0 && idx < sz);
        return arr[(head + idx) % cap];
    }
    const A& get(size_t idx) const {
        assert(idx >= 0 && idx < sz);
        return arr[(head + idx) % cap];
    }
    void increment_head() {
        head = (head + 1) % cap;
    }
    void decrement_head() {
        head = (head - 1 + cap) % cap;
    }
};

template <typename A, size_t cap>
class Stack {
    Array<A, cap> arr;
public:
    Stack() {}
    Stack(Stack &s) : arr(s.arr) {}
    void clear() { arr.clear(); }
    void push_back(A a) { arr.push_back(a); }
    void pop_back() { arr.pop_back(); }
    A& back() { return arr.back(); }
    const A& back() const { return arr.back(); }
    size_t size() const { return arr.size(); }
    size_t capacity() const { return cap; }
    bool empty() const { return arr.empty(); }
    bool full() const { return arr.full(); }
};

template <typename A, size_t cap>
class Queue {
    Array<A, cap> arr;
public:
    Queue() {}
    Queue(Queue &q) : arr(q.arr) {}
    void clear() { arr.clear(); }
    void push_back(A a) { arr.push_back(a); }
    void pop_front() { arr.pop_front(); }
    A& back() { return arr.back(); }
    const A& back() const { return arr.back(); }
    A& front() { return arr.front(); }
    const A& front() const { return arr.front(); }
    size_t size() const { return arr.size(); }
    size_t capacity() const { return cap; }
    bool empty() const { return arr.empty(); }
    bool full() const { return arr.full(); }
};

template <typename A, size_t cap>
class List {

    struct Node {
        Node *prev;
        Node *next;
        A value;
        size_t idx;
        Node()
        : prev(nullptr)
        , next(nullptr)
        , idx(0)
        {}
        Node(A a, size_t idx = 0)
        : prev(nullptr)
        , next(nullptr)
        , value(a)
        , idx(idx)
        {}
   };

    Node *head;
    Node *tail;
    Stack<size_t, cap> unused;
    std::array<Node, cap> nodes;
    size_t sz;
public:
    List()
    {
        clear();
    }
    List(const List<A, cap>& m)
    : unused(m.unused)
    {
        sz = m.sz;
       if (m.head) {
            head = &nodes[m.head->idx];
            tail = &nodes[m.tail->idx];
       } else {
            head = nullptr;
            tail = nullptr;
       }
       for (auto i = 0; i < cap; i++) {
            nodes[i].idx = m.nodes[i].idx;
            nodes[i].prev = &nodes[m.nodes[i].prev->idx];
            nodes[i].next = &nodes[m.nodes[i].next->idx];
            nodes[i].value = m.nodes[i].value;
        }
    }
    void clear() {
        head = nullptr;
        tail = nullptr;
        sz = 0;
        unused.clear();
        auto i = 0;
        while (!unused.full()) {
            unused.push_back(i);
            ++i;
        }
    }
    void push_front(A a) {
        assert(sz < cap);
        ++sz;
        auto idx = unused.back();
        if (!head) {
            head = &nodes[idx];
            tail = head;
            *head = Node(a, idx);
            unused.pop_back();
            return;
        }
        head->prev = &nodes[idx];
        unused.pop_back();
        head->prev->next = head;
        head = head->prev;
        *head = Node(a, idx);
    }
    void pop_front() {
        assert(sz > 0);
        --sz;
        unused.push_back(head->idx);
        if (head == tail) {
            head = nullptr;
            tail = nullptr;
            return;
        }
        head = head->next;
        head->prev = nullptr;
    }
    void push_back(A a) {
        assert(sz < cap);
        ++sz;
        auto idx = unused.back();
        if (!head) {
            head = &nodes[idx];
            tail = head;
            *head = Node(a, idx);
            unused.pop_back();
            return;
        }
        tail->next = &nodes[idx];
        unused.pop_back();
        tail->next->prev = tail;
        tail = tail->next;
        *head = Node(a, idx);
    }
    void pop_back() {
        assert(sz > 0);
        --sz;
        unused.push_back(tail->idx);
        if (head == tail) {
            head = nullptr;
            tail = nullptr;
            return;
        }
        tail = tail->next;
        tail->next = nullptr;
    }
    A& at(size_t idx) {
        if (idx >= 0 && idx < sz) {
            throw std::out_of_range("Out of range, at");
        }
        return get(idx);
    }
    const A& at(size_t idx) const {
        A& val = at(idx);
        return val;
    }
    A& operator[](size_t idx) { return get(idx); }
    const A& operator[](size_t idx) const { return get(idx); }
    A& front() {
        assert(sz > 0);
        return head->value;
    }
    const A& front() const {
        A& val = front();
        return val;
    }
    A& back() {
        assert(sz > 0);
        return tail->value;
    }
    const A& back() const {
        A& val = back();
        return val;
    }
    size_t capacity() const { return cap; }
    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    bool full() const { return sz == cap; }
private:
    A& get(size_t idx) {
        assert(idx >= 0 && idx < sz);
        if (idx < sz / 2) {
            auto node = head;
            for (auto i = 0; i < idx; node = node->next, ++i) {}
            return node->value;
        } else {
            auto node = tail;
            for (auto i = sz - 1; i > idx; node = node->prev, --i) {}
            return node->value;
        }
    }
    const A& get(size_t idx) const {
        A& val = get(idx);
        return val;
    }
};

}

#endif
