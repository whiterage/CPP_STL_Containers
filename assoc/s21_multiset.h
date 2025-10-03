#pragma once

#include <algorithm>
#include <initializer_list>
#include <limits>
#include <utility>
#include <vector>

#include "s21_redblack_tree.h"

namespace s21 {

template <class Key>
class multiset {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;

 private:
  using tree_type = RedBlackTree<Key, Key>;
  tree_type tree_;

 public:
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;

  multiset() = default;
  multiset(std::initializer_list<value_type> items) {
    for (const auto& key : items) insert(key);
  }
  multiset(const multiset&) = default;
  multiset(multiset&&) noexcept = default;
  ~multiset() = default;

  multiset& operator=(const multiset& other) = default;
  multiset& operator=(multiset&& other) noexcept = default;

  iterator begin() noexcept { return tree_.begin(); }
  const_iterator begin() const noexcept { return tree_.begin(); }
  iterator end() noexcept { return tree_.end(); }
  const_iterator end() const noexcept { return tree_.end(); }

  bool empty() const noexcept { return tree_.empty(); }
  size_type size() const noexcept { return tree_.size(); }
  size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(void*);
  }

  void clear() noexcept { tree_.clear(); }

  iterator insert(const value_type& value) {
    auto node = tree_.insert_equal({value, value});
    return tree_.make_iterator(node);
  }

  iterator erase(iterator pos) {
    if (pos != end()) {
      return tree_.erase(pos);
    }
    return end();
  }

  void swap(multiset& other) noexcept { tree_.swap(other.tree_); }

  void merge(multiset& other) {
    if (this == &other) return;

    std::vector<key_type> keys_to_move;
    keys_to_move.reserve(other.size());
    for (auto it = other.begin(); it != other.end(); ++it) {
      keys_to_move.push_back((*it).first);
    }

    for (const auto& key : keys_to_move) {
      this->insert(key);
    }

    other.clear();
  }

  iterator find(const key_type& key) {
    auto it = lower_bound(key);

    if (it != end() && !((*it).first < key) && !(key < (*it).first)) return it;
    return end();
  }
  const_iterator find(const key_type& key) const {
    auto it = lower_bound(key);
    if (it != end() && !((*it).first < key) && !(key < (*it).first)) return it;
    return end();
  }

  size_type count(const key_type& key) const {
    auto range = equal_range(key);
    return std::distance(range.first, range.second);
  }

  bool contains(const key_type& key) const { return find(key) != end(); }

  iterator lower_bound(const key_type& key) { return tree_.lower_bound(key); }
  const_iterator lower_bound(const key_type& key) const {
    return tree_.lower_bound(key);
  }

  iterator upper_bound(const key_type& key) { return tree_.upper_bound(key); }
  const_iterator upper_bound(const key_type& key) const {
    return tree_.upper_bound(key);
  }

  std::pair<iterator, iterator> equal_range(const key_type& key) {
    return {lower_bound(key), upper_bound(key)};
  }
  std::pair<const_iterator, const_iterator> equal_range(
      const key_type& key) const {
    return {lower_bound(key), upper_bound(key)};
  }

  template <class... Args>
  std::vector<iterator> insert_many(Args&&... args) {
    std::vector<iterator> result;
    result.reserve(sizeof...(args));

    (result.push_back(insert(std::forward<Args>(args))), ...);
    return result;
  }
};

}  // namespace s21