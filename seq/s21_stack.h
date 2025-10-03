#pragma once
#include <initializer_list>

#include "../seq/s21_list.h"

namespace s21 {

template <class T>
class stack {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

 private:
  list<T> base_;

 public:
  stack() = default;

  stack(std::initializer_list<value_type> items);

  stack(const stack& other) = default;
  stack& operator=(const stack& other) = default;

  stack(stack&& other) noexcept = default;

  ~stack() = default;

  stack& operator=(stack&& other) noexcept = default;

  bool empty() const noexcept;
  size_type size() const noexcept;

  const_reference top() const;

  void push(const_reference value);
  void pop();
  void swap(stack& other) noexcept;

  template <class... Args>
  void insert_many_back(Args&&... args);
};

template <class T>
stack<T>::stack(std::initializer_list<value_type> items) {
  for (const auto& x : items) base_.push_back(x);
}

template <class T>
bool stack<T>::empty() const noexcept {
  return base_.empty();
}

template <class T>
typename stack<T>::size_type stack<T>::size() const noexcept {
  return base_.size();
}

template <class T>
typename stack<T>::const_reference stack<T>::top() const {
  return base_.back();
}

template <class T>
void stack<T>::push(const_reference value) {
  base_.push_back(value);
}

template <class T>
void stack<T>::pop() {
  base_.pop_back();
}

template <class T>
void stack<T>::swap(stack& other) noexcept {
  base_.swap(other.base_);
}

template <class T>
template <class... Args>
void stack<T>::insert_many_back(Args&&... args) {
  base_.insert_many_back(std::forward<Args>(args)...);
}

}  // namespace s21