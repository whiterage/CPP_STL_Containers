#pragma once
#include <initializer_list>

#include "../seq/s21_list.h"

namespace s21 {

template <class T>
class queue {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

 private:
  list<T> base_;

 public:
  queue() = default;

  queue(std::initializer_list<value_type> items) {
    for (const auto& x : items) base_.push_back(x);
  }

  queue(const queue& other) = default;
  queue& operator=(const queue& other) = default;
  queue(queue&& other) noexcept = default;
  ~queue() = default;
  queue& operator=(queue&& other) noexcept = default;

  bool empty() const noexcept { return base_.empty(); }
  size_type size() const noexcept { return base_.size(); }

  const_reference front() const { return base_.front(); }
  const_reference back() const { return base_.back(); }

  void push(const_reference value) { base_.push_back(value); }
  void pop() { base_.pop_front(); }
  void swap(queue& other) noexcept { base_.swap(other.base_); }

  template <class... Args>
  void insert_many_back(Args&&... args) {
    base_.insert_many_back(std::forward<Args>(args)...);
  }
};

}  // namespace s21