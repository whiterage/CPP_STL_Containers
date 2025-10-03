#pragma once
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <utility>

namespace s21 {

template <class T>
class list {
 public:
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;

 private:
  struct Node {
    value_type value;
    Node* prev;
    Node* next;
  };

  Node* sentinel_ = nullptr;
  size_type size_ = 0;

 public:
  list() noexcept;
  explicit list(size_type n);
  list(std::initializer_list<value_type> items);
  list(const list& other);
  list(list&& other) noexcept;
  ~list() noexcept;
  list& operator=(const list& other);
  list& operator=(list&& other) noexcept;

  class iterator;
  class const_iterator;

  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept { return begin(); }
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept { return end(); }

  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  void clear() noexcept;
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void push_back(const_reference value);
  void pop_back();
  void push_front(const_reference value);
  void pop_front();
  void swap(list& other) noexcept;
  void merge(list& other);
  void splice(const_iterator pos, list& other);
  void splice(const_iterator pos, list& other, const_iterator it);
  void splice(const_iterator pos, list& other, const_iterator first,
              const_iterator last);
  void reverse() noexcept;
  void unique();
  void sort();

  template <class... Args>
  iterator insert_many(const_iterator pos, Args&&... args);
  template <class... Args>
  void insert_many_back(Args&&... args);
  template <class... Args>
  void insert_many_front(Args&&... args);

  class iterator {
    friend class list;
    friend class const_iterator;
    Node* cur_ = nullptr;
    explicit iterator(Node* p) : cur_(p) {}

   public:
    iterator() = default;
    reference operator*() const;
    iterator& operator++();
    iterator& operator--();
    bool operator==(const iterator& other) const noexcept;
    bool operator!=(const iterator& other) const noexcept;
  };

  class const_iterator {
    friend class list;
    const Node* cur_ = nullptr;
    explicit const_iterator(const Node* p) : cur_(p) {}

   public:
    const_iterator() = default;
    const_iterator(const iterator& it) : cur_(it.cur_) {}
    const_reference operator*() const;
    const_iterator& operator++();
    const_iterator& operator--();
    bool operator==(const const_iterator& other) const noexcept;
    bool operator!=(const const_iterator& other) const noexcept;
  };

 private:
  Node* create_node_(const_reference v);
  void link_between_(Node* left, Node* node, Node* right) noexcept;
  void unlink_node_(Node* node) noexcept;
  static Node* merge_two_sorted_(Node* a, Node* b);
  static Node* merge_sort_nodes_(Node* head);
};

template <class T>
list<T>::list() noexcept {
  sentinel_ = new Node{};
  sentinel_->next = sentinel_;
  sentinel_->prev = sentinel_;
  size_ = 0;
}

template <class T>
list<T>::~list() noexcept {
  clear();
  delete sentinel_;
  sentinel_ = nullptr;
  size_ = 0;
}

template <class T>
typename list<T>::iterator list<T>::begin() noexcept {
  return iterator(sentinel_->next);
}

template <class T>
typename list<T>::const_iterator list<T>::begin() const noexcept {
  return const_iterator(sentinel_->next);
}

template <class T>
typename list<T>::iterator list<T>::end() noexcept {
  return iterator(sentinel_);
}

template <class T>
typename list<T>::const_iterator list<T>::end() const noexcept {
  return const_iterator(sentinel_);
}

template <class T>
bool list<T>::empty() const noexcept {
  return size_ == 0;
}

template <class T>
typename list<T>::size_type list<T>::size() const noexcept {
  return size_;
}

// iterator
template <class T>
typename list<T>::reference list<T>::iterator::operator*() const {
  return cur_->value;
}

template <class T>
typename list<T>::iterator& list<T>::iterator::operator++() {
  cur_ = cur_->next;
  return *this;
}

template <class T>
typename list<T>::iterator& list<T>::iterator::operator--() {
  cur_ = cur_->prev;
  return *this;
}

template <class T>
bool list<T>::iterator::operator==(const iterator& other) const noexcept {
  return cur_ == other.cur_;
}

template <class T>
bool list<T>::iterator::operator!=(const iterator& other) const noexcept {
  return cur_ != other.cur_;
}

// const_iterator
template <class T>
typename list<T>::const_reference list<T>::const_iterator::operator*() const {
  return cur_->value;
}

template <class T>
typename list<T>::const_iterator& list<T>::const_iterator::operator++() {
  cur_ = cur_->next;
  return *this;
}

template <class T>
typename list<T>::const_iterator& list<T>::const_iterator::operator--() {
  cur_ = cur_->prev;
  return *this;
}

template <class T>
bool list<T>::const_iterator::operator==(
    const const_iterator& other) const noexcept {
  return cur_ == other.cur_;
}

template <class T>
bool list<T>::const_iterator::operator!=(
    const const_iterator& other) const noexcept {
  return cur_ != other.cur_;
}

template <class T>
typename list<T>::Node* list<T>::create_node_(const_reference v) {
  Node* n = new Node{};
  n->value = v;
  n->prev = nullptr;
  n->next = nullptr;
  return n;
}

template <class T>
void list<T>::link_between_(Node* left, Node* node, Node* right) noexcept {
  node->prev = left;
  node->next = right;
  left->next = node;
  right->prev = node;
}

template <class T>
void list<T>::unlink_node_(Node* node) noexcept {
  node->prev->next = node->next;
  node->next->prev = node->prev;
}

template <class T>
void list<T>::push_back(const_reference value) {
  Node* n = create_node_(value);
  link_between_(sentinel_->prev, n, sentinel_);
  ++size_;
}

template <class T>
void list<T>::push_front(const_reference value) {
  Node* n = create_node_(value);
  link_between_(sentinel_, n, sentinel_->next);
  ++size_;
}

template <class T>
typename list<T>::reference list<T>::front() {
  return sentinel_->next->value;
}

template <class T>
typename list<T>::const_reference list<T>::front() const {
  return sentinel_->next->value;
}

template <class T>
typename list<T>::reference list<T>::back() {
  return sentinel_->prev->value;
}

template <class T>
typename list<T>::const_reference list<T>::back() const {
  return sentinel_->prev->value;
}

template <class T>
void list<T>::pop_back() {
  if (empty()) return;

  Node* n = sentinel_->prev;
  unlink_node_(n);
  delete n;
  --size_;
}

template <class T>
void list<T>::pop_front() {
  if (empty()) return;

  Node* n = sentinel_->next;
  unlink_node_(n);
  delete n;
  --size_;
}

template <class T>
void list<T>::clear() noexcept {
  while (!empty()) pop_back();

  sentinel_->next = sentinel_->prev = sentinel_;
  size_ = 0;
}

template <class T>
typename list<T>::iterator list<T>::insert(iterator pos,
                                           const_reference value) {
  Node* p = pos.cur_;
  Node* n = create_node_(value);

  link_between_(p->prev, n, p);
  ++size_;

  return iterator(n);
}

template <class T>
void list<T>::erase(iterator pos) {
  Node* p = pos.cur_;
  if (p == sentinel_) return;
  unlink_node_(p);
  delete p;
  --size_;
}

template <class T>
list<T>::list(size_type n) {
  sentinel_ = new Node{};
  sentinel_->next = sentinel_;
  sentinel_->prev = sentinel_;
  size_ = 0;

  for (size_type i = 0; i < n; ++i) {
    push_back(value_type{});
  }
}

template <class T>
list<T>::list(std::initializer_list<value_type> items) {
  sentinel_ = new Node{};
  sentinel_->next = sentinel_;
  sentinel_->prev = sentinel_;
  size_ = 0;

  for (const auto& x : items) {
    push_back(x);
  }
}

template <class T>
list<T>::list(const list& other) {
  sentinel_ = new Node{};
  sentinel_->next = sentinel_;
  sentinel_->prev = sentinel_;
  size_ = 0;

  for (const auto& x : other) {
    push_back(x);
  }
}

template <class T>
list<T>& list<T>::operator=(const list& other) {
  if (this == &other) return *this;
  list tmp(other);
  swap(tmp);
  return *this;
}

template <class T>
list<T>::list(list&& other) noexcept {
  sentinel_ = other.sentinel_;
  size_ = other.size_;

  other.sentinel_ = new Node{};
  other.sentinel_->next = other.sentinel_;
  other.sentinel_->prev = other.sentinel_;
  other.size_ = 0;
}

template <class T>
list<T>& list<T>::operator=(list&& other) noexcept {
  if (this == &other) return *this;
  clear();
  delete sentinel_;

  sentinel_ = other.sentinel_;
  size_ = other.size_;

  other.sentinel_ = new Node{};
  other.sentinel_->next = other.sentinel_;
  other.sentinel_->prev = other.sentinel_;
  other.size_ = 0;

  return *this;
}

template <class T>
void list<T>::swap(list& other) noexcept {
  using std::swap;
  swap(sentinel_, other.sentinel_);
  swap(size_, other.size_);
}

template <class T>
template <class... Args>
typename list<T>::iterator list<T>::insert_many(const_iterator pos,
                                                Args&&... args) {
  Node* anchor = const_cast<Node*>(pos.cur_);

  iterator first;
  bool have_first = false;

  (void)std::initializer_list<int>{(
      [&] {
        iterator it = insert(iterator(anchor), std::forward<Args>(args));
        if (!have_first) {
          first = it;
          have_first = true;
        }
      }(),
      0)...};

  return have_first ? first : iterator(anchor);
}

template <class T>
typename list<T>::size_type list<T>::max_size() const noexcept {
  return std::numeric_limits<size_type>::max() / sizeof(Node);
}

template <class T>
void list<T>::merge(list& other) {
  if (this == &other) return;

  iterator it1 = begin();
  iterator it2 = other.begin();

  while (it1 != end() && it2 != other.end()) {
    if (*it2 < *it1) {
      iterator next = it2;
      ++next;
      splice(const_iterator(it1.cur_), other, const_iterator(it2.cur_));
      it2 = next;
    } else {
      ++it1;
    }
  }

  if (it2 != other.end()) {
    splice(const_iterator(sentinel_), other, const_iterator(it2.cur_),
           other.end());
  }
}

template <class T>
void list<T>::splice(const_iterator pos, list& other) {
  if (other.empty()) return;

  Node* p = const_cast<Node*>(pos.cur_);
  Node* other_first = other.sentinel_->next;
  Node* other_last = other.sentinel_->prev;

  other.sentinel_->next = other.sentinel_;
  other.sentinel_->prev = other.sentinel_;

  Node* prev = p->prev;
  prev->next = other_first;
  other_first->prev = prev;
  p->prev = other_last;
  other_last->next = p;

  size_ += other.size_;
  other.size_ = 0;
}

template <class T>
void list<T>::splice(const_iterator pos, list& other, const_iterator it) {
  Node* p = const_cast<Node*>(pos.cur_);
  Node* node = const_cast<Node*>(it.cur_);

  if (node == other.sentinel_) return;

  unlink_node_(node);
  --other.size_;

  link_between_(p->prev, node, p);
  ++size_;
}

template <class T>
void list<T>::splice(const_iterator pos, list& other, const_iterator first,
                     const_iterator last) {
  if (first == last) return;

  Node* p = const_cast<Node*>(pos.cur_);
  Node* f = const_cast<Node*>(first.cur_);
  Node* l = const_cast<Node*>(last.cur_);

  size_type count = 0;
  Node* temp = f;
  while (temp != l) {
    ++count;
    temp = temp->next;
  }

  Node* tail = l->prev;
  f->prev->next = l;
  l->prev = f->prev;
  other.size_ -= count;

  Node* prev = p->prev;
  prev->next = f;
  f->prev = prev;
  tail->next = p;
  p->prev = tail;
  size_ += count;
}

template <class T>
void list<T>::unique() {
  if (size_ <= 1) return;

  iterator it = begin();
  iterator next = it;
  ++next;

  while (next != end()) {
    if (*it == *next) {
      erase(next);
      next = it;
      ++next;
    } else {
      ++it;
      ++next;
    }
  }
}

template <class T>
void list<T>::reverse() noexcept {
  if (size_ <= 1) return;

  Node* current = sentinel_->next;
  while (current != sentinel_) {
    Node* next = current->next;
    std::swap(current->prev, current->next);
    current = next;
  }
  std::swap(sentinel_->next, sentinel_->prev);
}

template <class T>
void list<T>::sort() {
  if (size_ <= 1) return;

  Node* head = sentinel_->next;
  Node* tail = sentinel_->prev;
  tail->next = nullptr;
  head->prev = nullptr;

  head = merge_sort_nodes_(head);

  Node* new_tail = head;
  while (new_tail && new_tail->next) new_tail = new_tail->next;

  sentinel_->next = head ? head : sentinel_;
  sentinel_->prev = new_tail ? new_tail : sentinel_;
  if (head) head->prev = sentinel_;
  if (new_tail) new_tail->next = sentinel_;
}

template <class T>
typename list<T>::Node* list<T>::merge_two_sorted_(Node* a, Node* b) {
  Node dummy{};
  Node* tail = &dummy;
  dummy.next = nullptr;

  while (a && b) {
    if (!(b->value < a->value)) {
      tail->next = a;
      a->prev = tail;
      a = a->next;
    } else {
      tail->next = b;
      b->prev = tail;
      b = b->next;
    }
    tail = tail->next;
  }

  Node* rest = a ? a : b;
  if (rest) {
    tail->next = rest;
    rest->prev = tail;
  }

  Node* new_head = dummy.next;
  if (new_head) new_head->prev = nullptr;
  return new_head;
}

template <class T>
typename list<T>::Node* list<T>::merge_sort_nodes_(Node* head) {
  if (!head || !head->next) return head;

  Node* slow = head;
  Node* fast = head->next;
  while (fast && fast->next) {
    slow = slow->next;
    fast = fast->next->next;
  }
  Node* mid = slow->next;
  slow->next = nullptr;
  if (mid) mid->prev = nullptr;

  Node* left = merge_sort_nodes_(head);
  Node* right = merge_sort_nodes_(mid);
  return merge_two_sorted_(left, right);
}

template <class T>
template <class... Args>
void list<T>::insert_many_back(Args&&... args) {
  (void)std::initializer_list<int>{(push_back(std::forward<Args>(args)), 0)...};
}

template <class T>
template <class... Args>
void list<T>::insert_many_front(Args&&... args) {
  (void)std::initializer_list<int>{
      (insert(begin(), std::forward<Args>(args)), 0)...};
}

}  // namespace s21