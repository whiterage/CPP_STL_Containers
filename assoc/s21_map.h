#pragma once
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

#include "s21_redblack_tree.h"

namespace s21 {

template <class Key, class T>
class map {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;

  using tree_type = RedBlackTree<Key, T>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;

 private:
  tree_type tree_;

 public:
  map() = default;

  map(std::initializer_list<value_type> items) {
    for (const auto& kv : items) insert(kv);
  }

  map(const map&) = default;
  map(map&&) noexcept = default;
  ~map() = default;
  map& operator=(map&&) noexcept = default;

  iterator begin() noexcept { return tree_.begin(); }
  const_iterator begin() const noexcept { return tree_.begin(); }

  iterator end() noexcept { return tree_.end(); }
  const_iterator end() const noexcept { return tree_.end(); }

  const_iterator cbegin() const noexcept { return begin(); }
  const_iterator cend() const noexcept { return end(); }

  bool empty() const noexcept { return tree_.empty(); }
  size_type size() const noexcept { return tree_.size(); }
  size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }

  void clear() noexcept { tree_.clear(); }

  std::pair<iterator, bool> insert(const value_type& value) {
    auto [node, inserted] = tree_.insert_unique(value);
    return {tree_.make_iterator(node), inserted};
  }

  std::pair<iterator, bool> insert(const key_type& key, const mapped_type& obj);

  std::pair<iterator, bool> insert_or_assign(const key_type& key,
                                             const mapped_type& obj);

  void erase(iterator pos);

  void swap(map& other) noexcept {
    using std::swap;
    swap(tree_, other.tree_);
  }

  void merge(map& other);

  mapped_type& at(const key_type& key);
  const mapped_type& at(const key_type& key) const;

  mapped_type& operator[](const key_type& key);

  iterator find(const key_type& key) { return tree_.find(key); }

  const_iterator find(const key_type& key) const { return tree_.find(key); }

  bool contains(const key_type& key) const;

  template <class... Args>
  std::pair<iterator, bool> emplace(Args&&... args);

  template <class... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args);
};

template <class Key, class T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const key_type& key, const mapped_type& obj) {
  value_type p{key, obj};
  auto [node, inserted] = tree_.insert_unique(p);
  return {tree_.make_iterator(node), inserted};
}

template <class Key, class T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert_or_assign(
    const key_type& key, const mapped_type& obj) {
  auto it = tree_.find(key);
  if (it != tree_.end()) {
    (*it).second = obj;
    return {it, false};
  }
  return insert(value_type{key, obj});
}

template <class Key, class T>
typename map<Key, T>::mapped_type& map<Key, T>::at(const key_type& key) {
  auto it = tree_.find(key);
  if (it == tree_.end()) throw std::out_of_range("map::at: key not found");
  return (*it).second;
}

template <class Key, class T>
const typename map<Key, T>::mapped_type& map<Key, T>::at(
    const key_type& key) const {
  auto it = tree_.find(key);
  if (it == tree_.end()) throw std::out_of_range("map::at: key not found");
  return (*it).second;
}

template <class Key, class T>
typename map<Key, T>::mapped_type& map<Key, T>::operator[](
    const key_type& key) {
  auto it = tree_.find(key);
  if (it != tree_.end()) return (*it).second;

  auto [it2, inserted] = insert(value_type{key, mapped_type{}});
  (void)inserted;
  return (*it2).second;
}

template <class Key, class T>
void map<Key, T>::erase(iterator pos) {
  if (pos == end()) return;
  tree_.erase(pos);
}

template <class Key, class T>
bool map<Key, T>::contains(const key_type& key) const {
  return tree_.find(key) != tree_.end();
}

template <class Key, class T>
void map<Key, T>::merge(map& other) {
  if (this == &other) return;
  std::vector<key_type> keys;
  keys.reserve(other.size());
  for (auto it = other.begin(); it != other.end(); ++it) {
    keys.push_back((*it).first);
  }
  for (const auto& k : keys) {
    auto it_other = other.find(k);
    if (it_other == other.end()) continue;
    auto [pos, inserted] = insert(*it_other);
    if (inserted) other.erase(it_other);
  }
}

template <class Key, class T>
template <class... Args>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::emplace(
    Args&&... args) {
  value_type v(std::forward<Args>(args)...);
  return insert(v);
}

template <class Key, class T>
template <class... Args>
std::vector<std::pair<typename map<Key, T>::iterator, bool>>
map<Key, T>::insert_many(Args&&... args) {
  std::vector<std::pair<iterator, bool>> res;
  res.reserve(sizeof...(args));
  (res.emplace_back(insert(std::forward<Args>(args))), ...);
  return res;
}

}  // namespace s21