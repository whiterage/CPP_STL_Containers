#pragma once

#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace s21 {

template <typename T, std::size_t N>
class array {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = std::size_t;

  array() = default;

  array(std::initializer_list<value_type> const& items) {
    size_type count = std::min(items.size(), N);
    std::copy(items.begin(), items.begin() + count, data_);
  }

  array(const array& other) = default;

  array(array&& other) noexcept {
    std::move(std::begin(other.data_), std::end(other.data_), data_);
  }

  ~array() = default;

  array& operator=(const array& other) = default;

  array& operator=(array&& other) noexcept {
    if (this != &other) {
      std::move(std::begin(other.data_), std::end(other.data_), data_);
    }
    return *this;
  }

  reference at(size_type pos) {
    if (pos >= N) throw std::out_of_range("array index is out of bounds");
    return data_[pos];
  }
  const_reference at(size_type pos) const {
    if (pos >= N) throw std::out_of_range("array index is out of bounds");
    return data_[pos];
  }

  reference operator[](size_type pos) { return data_[pos]; }
  const_reference operator[](size_type pos) const { return data_[pos]; }

  reference front() { return data_[0]; }
  const_reference front() const { return data_[0]; }

  reference back() { return data_[N - 1]; }
  const_reference back() const { return data_[N - 1]; }

  value_type* data() noexcept { return data_; }
  const value_type* data() const noexcept { return data_; }

  iterator begin() noexcept { return data_; }
  const_iterator begin() const noexcept { return data_; }
  const_iterator cbegin() const noexcept { return data_; }

  iterator end() noexcept { return data_ + N; }
  const_iterator end() const noexcept { return data_ + N; }
  const_iterator cend() const noexcept { return end(); }

  bool empty() const noexcept { return N == 0; }
  size_type size() const noexcept { return N; }
  size_type max_size() const noexcept { return N; }

  void swap(array& other) noexcept {
    for (size_type i = 0; i < N; ++i) {
      using std::swap;
      swap(data_[i], other.data_[i]);
    }
  }

  void fill(const_reference value) {
    for (size_type i = 0; i < N; ++i) data_[i] = value;
  }

 private:
  value_type data_[N]{};
};

}  // namespace s21