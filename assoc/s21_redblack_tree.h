#pragma once
#include <cstddef>
#include <iterator>
#include <utility>

namespace s21 {

enum class Color { RED, BLACK };

template <typename Key, typename T>
struct Node {
  using value_type = std::pair<const Key, T>;

  value_type data{};
  Color color{Color::RED};
  Node* parent{nullptr};
  Node* left{nullptr};
  Node* right{nullptr};

  Node() = default;
  explicit Node(const value_type& v) : data(v) {}
  explicit Node(value_type&& v) noexcept : data(std::move(v)) {}
};

template <typename Key, typename T>
class RedBlackTree {
 private:
  using Node_t = Node<Key, T>;

  Node_t* header_ = nullptr;
  std::size_t size_ = 0;
  Node_t* min_node_ = nullptr;
  Node_t* max_node_ = nullptr;

 public:
  using value_type = typename Node_t::value_type;

  RedBlackTree() {
    header_ = new Node_t{};
    header_->color = Color::BLACK;
    header_->parent = nullptr;
    header_->left = header_;
    header_->right = header_;
  }

  RedBlackTree(const RedBlackTree& other) : RedBlackTree() {
    Node_t* new_root = nullptr;

    if (other.size_ > 0) {
      new_root = copy_helper(other.header_->parent, header_);
    }

    header_->parent = new_root;
    size_ = other.size_;

    if (size_ > 0) {
      min_node_ = find_min(new_root);
      max_node_ = find_max(new_root);

      header_->left = min_node_;
      header_->right = max_node_;
    }
  }

  RedBlackTree(RedBlackTree&& other) noexcept
      : header_(other.header_),
        size_(other.size_),
        min_node_(other.min_node_),
        max_node_(other.max_node_) {
    other.header_ = new Node_t{};
    other.header_->color = Color::BLACK;
    other.header_->parent = nullptr;
    other.header_->left = other.header_;
    other.header_->right = other.header_;

    other.size_ = 0;
    other.min_node_ = other.header_;
    other.max_node_ = other.header_;

    if (header_->parent != nullptr) {
      header_->parent->parent = header_;
    }
  }

  RedBlackTree& operator=(const RedBlackTree& other) {
    if (this == &other) return *this;
    RedBlackTree tmp(other);
    swap(tmp);
    return *this;
  }

  RedBlackTree& operator=(RedBlackTree&& other) noexcept {
    if (this != &other) {
      RedBlackTree tmp;
      this->swap(tmp);
      this->swap(other);
    }
    return *this;
  }

  ~RedBlackTree() {
    clear();
    delete header_;
  }

  std::pair<Node_t*, bool> insert_unique(
      const typename Node_t::value_type& val) {
    const Key& key = val.first;

    Node_t* cur = header_->parent;
    Node_t* par = header_;

    while (cur != nullptr) {
      par = cur;
      const Key& ck = cur->data.first;
      if (key < ck)
        cur = cur->left;
      else if (ck < key)
        cur = cur->right;
      else
        return {cur, false};
    }

    Node_t* new_node = new Node_t(val);

    Node_t* result = attach_node(new_node, par);

    ++size_;
    insert_rebalance(result);
    update_min_max_nodes(result);
    return {result, true};
  }

  Node_t* insert_equal(const value_type& value) {
    Node_t* parent = find_insertion_point_equal(value.first);

    Node_t* new_node = new Node_t(value);

    Node_t* result = attach_node(new_node, parent);

    insert_rebalance(result);

    update_min_max_nodes(result);

    size_++;

    return result;
  }

  void clear() noexcept {
    clear_helper(header_->parent);
    header_->parent = nullptr;
    min_node_ = nullptr;
    max_node_ = nullptr;
    size_ = 0;
    header_->left = header_;
    header_->right = header_;
  }

  void erase(Node_t* z) {
    if (!z || z == header_) return;

    Node_t* x = nullptr;
    Node_t* x_parent = nullptr;
    Node_t* y = z;
    Color y_color = y->color;

    if (z->left == nullptr) {
      x = z->right;
      x_parent = z->parent;
      transplant(z, z->right);
      if (x) x_parent = x->parent;
    } else if (z->right == nullptr) {
      x = z->left;
      x_parent = z->parent;
      transplant(z, z->left);
      if (x) x_parent = x->parent;
    } else {
      y = z->right;
      while (y->left) y = y->left;
      y_color = y->color;
      x = y->right;

      if (y->parent != z) {
        x_parent = y->parent;
        transplant(y, y->right);
        if (y->right) y->right->parent = y;
        y->right = z->right;
        y->right->parent = y;
      } else {
        x_parent = y;
        if (x) x->parent = y;
      }

      transplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }

    delete z;
    --size_;

    if (y_color == Color::BLACK) erase_rebalance(x, x_parent);

    update_min_max_nodes_after_erase();
  }

  class MapIterator {
   private:
    Node_t* current_ = nullptr;
    Node_t* header_ptr_ = nullptr;
    friend class RedBlackTree<Key, T>;

   public:
    using difference_type = std::ptrdiff_t;
    using value_type = typename Node_t::value_type;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::bidirectional_iterator_tag;

    MapIterator() = default;
    MapIterator(Node_t* n, Node_t* h) : current_(n), header_ptr_(h) {}

    reference operator*() const { return current_->data; }
    pointer operator->() const { return &current_->data; }

    bool operator==(const MapIterator& o) const {
      return current_ == o.current_;
    }
    bool operator!=(const MapIterator& o) const {
      return current_ != o.current_;
    }

    MapIterator& operator++() {
      if (!current_) return *this;
      if (current_->right) {
        current_ = current_->right;
        while (current_->left) current_ = current_->left;
      } else {
        Node_t* p = current_->parent;
        while (p && current_ == p->right) {
          current_ = p;
          p = p->parent;
        }
        current_ = p;
      }
      return *this;
    }
    MapIterator operator++(int) {
      MapIterator t(*this);
      ++(*this);
      return t;
    }

    MapIterator& operator--() {
      if (current_ == header_ptr_) {
        current_ = header_ptr_->right;
        return *this;
      }
      if (current_->left) {
        current_ = current_->left;
        while (current_->right) current_ = current_->right;
      } else {
        Node_t* p = current_->parent;
        while (p && current_ == p->left) {
          current_ = p;
          p = p->parent;
        }
        current_ = p;
      }
      return *this;
    }

    MapIterator operator--(int) {
      MapIterator t(*this);
      --(*this);
      return t;
    }
  };

  class ConstMapIterator {
   private:
    const Node_t* current_ = nullptr;
    const Node_t* header_ptr_ = nullptr;
    friend class RedBlackTree<Key, T>;

   public:
    using difference_type = std::ptrdiff_t;
    using value_type = typename Node_t::value_type;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::bidirectional_iterator_tag;

    ConstMapIterator() = default;
    ConstMapIterator(const Node_t* n, const Node_t* h)
        : current_(n), header_ptr_(h) {}

    reference operator*() const { return current_->data; }
    pointer operator->() const { return &current_->data; }

    bool operator==(const ConstMapIterator& o) const {
      return current_ == o.current_;
    }
    bool operator!=(const ConstMapIterator& o) const {
      return current_ != o.current_;
    }

    ConstMapIterator& operator++() {
      if (!current_) return *this;
      if (current_->right) {
        current_ = current_->right;
        while (current_->left) current_ = current_->left;
      } else {
        const Node_t* p = current_->parent;
        while (p && current_ == p->right) {
          current_ = p;
          p = p->parent;
        }
        current_ = p;
      }
      return *this;
    }
    ConstMapIterator operator++(int) {
      ConstMapIterator t(*this);
      ++(*this);
      return t;
    }

    ConstMapIterator& operator--() {
      if (current_ == header_ptr_) {
        current_ = header_ptr_->right;
        return *this;
      }
      if (current_->left) {
        current_ = current_->left;
        while (current_->right) current_ = current_->right;
      } else {
        const Node_t* p = current_->parent;
        while (p && current_ == p->left) {
          current_ = p;
          p = p->parent;
        }
        current_ = p;
      }
      return *this;
    }
    ConstMapIterator operator--(int) {
      ConstMapIterator t(*this);
      --(*this);
      return t;
    }
  };

  using iterator = MapIterator;
  using const_iterator = ConstMapIterator;

  iterator begin() {
    return (size_ == 0) ? iterator(header_, header_)
                        : iterator(min_node_, header_);
  }
  iterator end() { return iterator(header_, header_); }

  const_iterator begin() const {
    return (size_ == 0) ? const_iterator(header_, header_)
                        : const_iterator(min_node_, header_);
  }
  const_iterator end() const { return const_iterator(header_, header_); }

  iterator find(const Key& key) {
    Node_t* n = find_node(key);
    return n ? iterator(n, header_) : end();
  }
  const_iterator find(const Key& key) const {
    Node_t* n = find_node(key);
    return n ? const_iterator(n, header_) : end();
  }

  iterator make_iterator(Node_t* n) noexcept { return iterator(n, header_); }
  const_iterator make_const_iterator(const Node_t* n) const noexcept {
    return const_iterator(n, header_);
  }

  iterator lower_bound(const Key& key) {
    Node_t* current = header_->parent;
    Node_t* result = header_;

    while (current) {
      if (!(current->data.first < key)) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return iterator(result, header_);
  }

  iterator upper_bound(const Key& key) {
    Node_t* current = header_->parent;
    Node_t* result = header_;

    while (current) {
      if (key < current->data.first) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return iterator(result, header_);
  }

  const_iterator lower_bound(const Key& key) const {
    Node_t* current = header_->parent;
    Node_t* result = header_;

    while (current) {
      if (!(current->data.first < key)) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return const_iterator(result, header_);
  }

  const_iterator upper_bound(const Key& key) const {
    Node_t* current = header_->parent;
    Node_t* result = header_;

    while (current) {
      if (key < current->data.first) {
        result = current;
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return const_iterator(result, header_);
  }

  iterator erase(iterator pos) noexcept {
    if (pos == end()) {
      return end();
    }

    iterator next_it = pos;
    ++next_it;

    Node_t* node_to_delete = pos.current_;

    erase(node_to_delete);

    return next_it;
  }

  void swap(RedBlackTree& other) noexcept {
    std::swap(header_, other.header_);
    std::swap(size_, other.size_);
    std::swap(min_node_, other.min_node_);
    std::swap(max_node_, other.max_node_);

    if (header_->parent) {
      header_->parent->parent = header_;
    }

    if (other.header_->parent) {
      other.header_->parent->parent = other.header_;
    }
  }

  std::size_t size() const noexcept { return size_; }
  bool empty() const noexcept { return size_ == 0; }

 private:
  void rotate_left(Node_t* x) {
    Node_t* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == header_)
      header_->parent = y;
    else if (x == x->parent->left)
      x->parent->left = y;
    else
      x->parent->right = y;

    y->left = x;
    x->parent = y;
  }

  void rotate_right(Node_t* y) {
    Node_t* x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == header_)
      header_->parent = x;
    else if (y == y->parent->right)
      y->parent->right = x;
    else
      y->parent->left = x;

    x->right = y;
    y->parent = x;
  }

  void insert_rebalance(Node_t* z) {
    while (z != header_->parent && z->parent->color == Color::RED) {
      Node_t* p = z->parent;
      Node_t* g = p->parent;
      if (p == g->left) {
        Node_t* u = g->right;
        if (u && u->color == Color::RED) {
          p->color = Color::BLACK;
          u->color = Color::BLACK;
          if (g != header_->parent) g->color = Color::RED;
          z = g;
        } else {
          if (z == p->right) {
            rotate_left(p);
            z = z->left;
            p = z->parent;
          }
          p->color = Color::BLACK;
          g->color = Color::RED;
          rotate_right(g);
          break;
        }
      } else {
        Node_t* u = g->left;
        if (u && u->color == Color::RED) {
          p->color = Color::BLACK;
          u->color = Color::BLACK;
          if (g != header_->parent) g->color = Color::RED;
          z = g;
        } else {
          if (z == p->left) {
            rotate_right(p);
            z = z->right;
            p = z->parent;
          }
          p->color = Color::BLACK;
          g->color = Color::RED;
          rotate_left(g);
          break;
        }
      }
    }
    if (header_->parent) header_->parent->color = Color::BLACK;
  }

  void transplant(Node_t* u, Node_t* v) {
    if (u->parent == header_)
      header_->parent = v;
    else if (u == u->parent->left)
      u->parent->left = v;
    else
      u->parent->right = v;
    if (v) v->parent = u->parent;
  }

  void erase_rebalance(Node_t* x, Node_t* parent) {
    while ((x != header_->parent) &&
           (x == nullptr || x->color == Color::BLACK)) {
      Node_t* p = x ? x->parent : parent;
      if (!p) break;

      bool x_is_left = (x == p->left);
      if (x == nullptr) x_is_left = (p->left == nullptr);

      Node_t* w = x_is_left ? p->right : p->left;

      if (w && w->color == Color::RED) {
        w->color = Color::BLACK;
        p->color = Color::RED;
        if (x_is_left)
          rotate_left(p);
        else
          rotate_right(p);
        w = x_is_left ? p->right : p->left;
      }

      Node_t* wl = w ? w->left : nullptr;
      Node_t* wr = w ? w->right : nullptr;
      bool wl_black = (!wl || wl->color == Color::BLACK);
      bool wr_black = (!wr || wr->color == Color::BLACK);

      if (wl_black && wr_black) {
        if (w) w->color = Color::RED;
        x = p;
        parent = x->parent;
      } else {
        if (x_is_left) {
          if (wr_black) {
            if (wl) wl->color = Color::BLACK;
            if (w) w->color = Color::RED;
            rotate_right(w);
            w = p->right;
            wr = w ? w->right : nullptr;
          }
          if (w) w->color = p->color;
          p->color = Color::BLACK;
          if (wr) wr->color = Color::BLACK;
          rotate_left(p);
        } else {
          if (wl_black) {
            if (wr) wr->color = Color::BLACK;
            if (w) w->color = Color::RED;
            rotate_left(w);
            w = p->left;
            wl = w ? w->left : nullptr;
          }
          if (w) w->color = p->color;
          p->color = Color::BLACK;
          if (wl) wl->color = Color::BLACK;
          rotate_right(p);
        }
        x = header_->parent;
      }
    }
    if (x) x->color = Color::BLACK;
  }

  void update_min_max_nodes(Node_t* n) {
    if (!min_node_ || n->data.first < min_node_->data.first) {
      min_node_ = n;
      header_->left = min_node_;
    }
    if (!max_node_ || max_node_->data.first < n->data.first) {
      max_node_ = n;
      header_->right = max_node_;
    }
  }

  void update_min_max_nodes_after_erase() {
    Node_t* root = header_->parent;
    if (!root) {
      min_node_ = nullptr;
      max_node_ = nullptr;
      header_->left = header_;
      header_->right = header_;
    } else {
      min_node_ = find_min(root);
      max_node_ = find_max(root);
      header_->left = min_node_;
      header_->right = max_node_;
    }
  }

  static void clear_helper(Node_t* node) {
    if (!node) return;
    clear_helper(node->left);
    clear_helper(node->right);
    delete node;
  }

  static Node_t* find_min(Node_t* n) {
    while (n && n->left) n = n->left;
    return n;
  }
  static Node_t* find_max(Node_t* n) {
    while (n && n->right) n = n->right;
    return n;
  }

  Node_t* find_node(const Key& key) const {
    Node_t* cur = header_->parent;
    while (cur) {
      const Key& ck = cur->data.first;
      if (key < ck)
        cur = cur->left;
      else if (ck < key)
        cur = cur->right;
      else
        return cur;
    }
    return nullptr;
  }

  static Node_t* copy_helper(const Node_t* other, Node_t* parent) {
    if (!other) return nullptr;
    Node_t* n = new Node_t(other->data);
    n->color = other->color;
    n->parent = parent;
    n->left = copy_helper(other->left, n);
    n->right = copy_helper(other->right, n);
    return n;
  }

  Node_t* find_insertion_point_equal(const Key& key) const {
    Node_t* current = header_->parent;
    Node_t* parent = header_;

    while (current) {
      parent = current;
      const Key& current_key = current->data.first;

      if (key < current_key) {
        current = current->left;
      } else {
        current = current->right;
      }
    }
    return parent;
  }

  Node_t* attach_node(Node_t* new_node, Node_t* parent) {
    const Key& key = new_node->data.first;
    new_node->parent = parent;

    if (parent == header_) {
      header_->parent = new_node;
    } else if (key < parent->data.first) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }

    return new_node;
  }
};

}  // namespace s21