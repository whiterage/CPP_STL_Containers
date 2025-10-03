#pragma once
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <utility>

namespace s21 {
template <class T>
class vector {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using iterator = T*;
  using const_iterator = const T*;
  using reference = value_type&;
  using const_reference = const value_type&;

  vector() noexcept = default;
  explicit vector(size_type n);
  vector(std::initializer_list<value_type> items);
  vector(const vector& other);
  vector(vector&& other) noexcept;
  ~vector() noexcept;

  reference operator[](size_type pos) noexcept;
  const_reference operator[](size_type pos) const noexcept;
  vector& operator=(const vector& other);
  vector& operator=(vector&& other) noexcept;

  reference at(size_type pos);
  const_reference at(size_type pos) const;

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;

  value_type* data() noexcept;
  const value_type* data() const noexcept;

  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept { return begin(); }
  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept { return end(); }

  [[nodiscard]] bool empty() const noexcept;
  [[nodiscard]] size_type size() const noexcept;
  [[nodiscard]] size_type capacity() const noexcept;
  [[nodiscard]] size_type max_size() const noexcept;
  void reserve(size_type new_cap);
  void shrink_to_fit();

  void clear() noexcept;
  iterator insert(iterator pos, const_reference value);
  iterator insert(const_iterator pos, const_reference value) {
    const size_type idx = static_cast<size_type>(pos - cbegin());
    return insert(begin() + idx, value);
  }
  void erase(iterator pos);
  void push_back(const_reference value);
  void pop_back();
  void swap(vector& other) noexcept;
  void reallocate(size_type new_cap);

  template <class... Args>
  iterator insert_many(const_iterator pos, Args&&... args);
  template <class... Args>
  void insert_many_back(Args&&... args);

 private:
  T* data_ = nullptr;
  size_type size_ = 0;
  size_type cap_ = 0;
};

template <class T>
typename vector<T>::size_type vector<T>::size() const noexcept {
  return size_;
}

template <class T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
  return cap_;
}

template <class T>
bool vector<T>::empty() const noexcept {
  return size_ == 0;
}

template <class T>
typename vector<T>::value_type* vector<T>::data() noexcept {
  return data_;
}

template <class T>
const typename vector<T>::value_type* vector<T>::data() const noexcept {
  return data_;
}

template <class T>
typename vector<T>::iterator vector<T>::begin() noexcept {
  return data_;
}

template <class T>
typename vector<T>::const_iterator vector<T>::begin() const noexcept {
  return data_;
}

template <class T>
typename vector<T>::iterator vector<T>::end() noexcept {
  return data_ + size_;
}

template <class T>
typename vector<T>::const_iterator vector<T>::end() const noexcept {
  return data_ + size_;
}

template <class T>
typename vector<T>::reference vector<T>::front() {
  return data_[0];
}

template <class T>
typename vector<T>::const_reference vector<T>::front() const {
  return data_[0];
}

template <class T>
typename vector<T>::reference vector<T>::back() {
  return data_[size_ - 1];
}

template <class T>
typename vector<T>::const_reference vector<T>::back() const {
  return data_[size_ - 1];
}

template <class T>
typename vector<T>::reference vector<T>::operator[](size_type pos) noexcept {
  return data_[pos];
}

template <class T>
typename vector<T>::const_reference vector<T>::operator[](
    size_type pos) const noexcept {
  return data_[pos];
}

template <class T>
typename vector<T>::reference vector<T>::at(size_type pos) {
  if (pos >= size_)
    throw std::out_of_range("s21::vector::at: index out of range");
  return data_[pos];
}

template <class T>
typename vector<T>::const_reference vector<T>::at(size_type pos) const {
  if (pos >= size_)
    throw std::out_of_range("s21::vector::at: index out of range");
  return data_[pos];
}

template <class T>
typename vector<T>::size_type vector<T>::max_size() const noexcept {
  return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

template <class T>
void vector<T>::reallocate(size_type new_cap) {
  if (new_cap == cap_) return;

  value_type* new_data = (new_cap ? new value_type[new_cap] : nullptr);

  for (size_type i = 0; i < size_; ++i) {
    new_data[i] = std::move(data_[i]);
  }
  delete[] data_;
  data_ = new_data;
  cap_ = new_cap;
}

template <class T>
void vector<T>::reserve(size_type new_cap) {
  if (!(new_cap <= cap_)) {
    reallocate(new_cap);
  }
}

template <class T>
void vector<T>::shrink_to_fit() {
  if (size_ < cap_) reallocate(size_);
}

template <class T>
void vector<T>::push_back(const_reference value) {
  if (size_ == cap_) {
    const size_type new_cap = (cap_ == 0 ? 1 : cap_ * 2);
    reallocate(new_cap);
  }
  data_[size_] = value;
  ++size_;
}

template <class T>
void vector<T>::pop_back() {
  if (size_ != 0) --size_;
}

template <class T>
vector<T>::~vector() noexcept {
  delete[] data_;
  data_ = nullptr;
  size_ = cap_ = 0;
}

template <class T>
vector<T>::vector(size_type n) {
  if (n == 0) {
    return;
  }
  data_ = new value_type[n]();
  size_ = n;
  cap_ = n;
}

template <class T>
vector<T>::vector(std::initializer_list<value_type> items) {
  const size_type n = items.size();
  if (n == 0) {
    data_ = nullptr;
    size_ = cap_ = 0;
    return;
  }
  data_ = new value_type[n];
  size_ = cap_ = n;

  size_type i = 0;
  for (const auto& x : items) {
    data_[i++] = x;
  }
}

template <class T>
vector<T>::vector(const vector& other) {
  if (other.size_ == 0) {
    data_ = nullptr;
    size_ = cap_ = 0;
    return;
  }
  data_ = new value_type[other.size_];
  size_ = cap_ = other.size_;
  for (size_type i = 0; i < size_; ++i) {
    data_[i] = other.data_[i];
  }
}

template <class T>
vector<T>::vector(vector&& other) noexcept {
  data_ = other.data_;
  size_ = other.size_;
  cap_ = other.cap_;

  other.data_ = nullptr;
  other.size_ = other.cap_ = 0;
}

template <class T>
vector<T>& vector<T>::operator=(vector&& other) noexcept {
  if (this == &other) return *this;
  delete[] data_;
  data_ = other.data_;
  size_ = other.size_;
  cap_ = other.cap_;
  other.data_ = nullptr;
  other.size_ = other.cap_ = 0;
  return *this;
}

template <class T>
vector<T>& vector<T>::operator=(const vector& other) {
  if (this == &other) return *this;
  if (other.size_ == 0) {
    delete[] data_;
    data_ = nullptr;
    size_ = cap_ = 0;
    return *this;
  }

  value_type* new_data = new value_type[other.size_];
  for (size_type i = 0; i < other.size_; ++i) new_data[i] = other.data_[i];
  delete[] data_;
  data_ = new_data;
  size_ = cap_ = other.size_;
  return *this;
}

template <class T>
typename vector<T>::iterator vector<T>::insert(iterator pos,
                                               const_reference value) {
  size_type idx = static_cast<size_type>(pos - begin());
  if (size_ == cap_) {
    const size_type new_cap = (cap_ == 0 ? 1 : cap_ * 2);
    reallocate(new_cap);
  }

  for (size_type i = size_; i > idx; --i) {
    data_[i] = std::move(data_[i - 1]);
  }
  data_[idx] = value;
  ++size_;
  return begin() + idx;
}

template <class T>
void vector<T>::erase(iterator pos) {
  size_type idx = static_cast<size_type>(pos - begin());
  for (size_type i = idx; i + 1 < size_; ++i)
    data_[i] = std::move(data_[i + 1]);
  if (size_ > 0) --size_;
}

template <class T>
template <class... Args>
typename vector<T>::iterator vector<T>::insert_many(const_iterator pos,
                                                    Args&&... args) {
  const size_type idx =
      static_cast<size_type>(pos - static_cast<const_iterator>(data_));

  const size_type count = sizeof...(args);
  if (count == 0) return begin() + idx;

  size_type new_cap = (cap_ == 0 ? 1 : cap_);
  while (new_cap < size_ + count) new_cap *= 2;
  if (new_cap != cap_) reallocate(new_cap);

  for (size_type i = size_; i > idx; --i) {
    data_[i + count - 1] = std::move(data_[i - 1]);
  }

  size_type j = 0;
  ((data_[idx + j++] = std::forward<Args>(args)), ...);

  size_ += count;
  return begin() + idx;
}

template <class T>
template <class... Args>
void vector<T>::insert_many_back(Args&&... args) {
  const size_type count = sizeof...(args);
  if (count == 0) return;

  size_type new_cap = (cap_ == 0 ? 1 : cap_);
  while (new_cap < size_ + count) new_cap *= 2;
  if (new_cap != cap_) reallocate(new_cap);

  size_type j = 0;
  ((data_[size_ + j++] = std::forward<Args>(args)), ...);
  size_ += count;
}
}  // namespace s21