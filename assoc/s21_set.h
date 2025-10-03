#pragma once
#include <initializer_list>
#include <limits>
#include <utility>
#include <vector>

#include "s21_redblack_tree.h"

namespace s21 {

template <class Key>
class set {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = std::size_t;

  using tree_type = RedBlackTree<Key, Key>;
  using iterator = typename tree_type::iterator;
  using const_iterator = typename tree_type::const_iterator;

 private:
  tree_type tree_;

 public:
  set() = default;

  set(std::initializer_list<value_type> items) {
    for (const auto& key : items) insert(key);
  }

  set(const set&) = default;
  set(set&&) noexcept = default;
  ~set() = default;
  set& operator=(set&&) noexcept = default;

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
    auto [node, inserted] = tree_.insert_unique({value, value});
    return {tree_.make_iterator(node), inserted};
  }

  void erase(iterator pos) {
    if (pos == end()) return;
    tree_.erase(pos);
  }

  void swap(set& other) noexcept {
    using std::swap;
    swap(tree_, other.tree_);
  }

  void merge(set& other) {
    if (this == &other) return;
    for (auto it = other.begin(); it != other.end();) {
      auto next = it;
      ++next;
      auto [pos, inserted] = insert((*it).first);
      if (inserted) other.erase(it);
      it = next;
    }
  }

  iterator find(const key_type& key) {
    auto it = tree_.find(key);
    return it;
  }

  const_iterator find(const key_type& key) const {
    auto it = tree_.find(key);
    return it;
  }

  bool contains(const key_type& key) const {
    return tree_.find(key) != tree_.end();
  }

  template <class... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    std::vector<std::pair<iterator, bool>> res;
    res.reserve(sizeof...(args));
    (res.emplace_back(insert(std::forward<Args>(args))), ...);
    return res;
  }
};

}  // namespace s21