#ifndef FIXED_HPP
#define FIXED_HPP

#include <cassert>
#include <exception>
#include <array>

template <typename A, size_t cap>
class FixedDeque {
  size_t head;
  size_t sz;
  std::array<A, cap> arr;
public:
  FixedDeque()
  : head(0)
  , sz(0)
  {}
  FixedDeque(const FixedDeque<A, cap>& d)
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
class FixedStack {
  FixedDeque<A, cap> d;
public:
  void clear() { d.clear(); }
  void push_back(A a) { d.push_back(a); }
  void pop_back() { d.pop_back(); }
  A& back() { return d.back(); }
  const A& back() const { return d.back(); }
  size_t size() const { return d.size(); }
  size_t capacity() const { return cap; }
  bool empty() const { return d.empty(); }
  bool full() const { return d.full(); }
};

template <typename A, size_t cap>
class FixedQueue {
  FixedDeque<A, cap> d;
public:
  void clear() { d.clear(); }
  void push_back(A a) { d.push_back(a); }
  void pop_front() { d.pop_front(); }
  A& back() { return d.back(); }
  const A& back() const { return d.back(); }
  A& front() { return d.front(); }
  const A& front() const { return d.front(); }
  size_t size() const { return d.size(); }
  size_t capacity() const { return cap; }
  bool empty() const { return d.empty(); }
  bool full() const { return d.full(); }
};

#endif
