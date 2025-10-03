#include <gtest/gtest.h>

#include "../s21_containers.h"

TEST(ListExtra, SpliceWholeList) {
  s21::list<int> a{1, 2};
  s21::list<int> b{3, 4, 5};
  a.splice(a.cend(), b);
  ASSERT_TRUE(b.empty());
  ASSERT_EQ(a.size(), 5u);
  int expect[] = {1, 2, 3, 4, 5};
  size_t i = 0;
  for (auto x : a) EXPECT_EQ(x, expect[i++]);
}

TEST(ListExtra, SpliceSingleIterator) {
  s21::list<int> a{1, 3, 4};
  s21::list<int> b{2, 5};
  auto it_from_b = b.begin();
  a.splice(++a.cbegin(), b, it_from_b);
  int expect_a[] = {1, 2, 3, 4};
  size_t i = 0;
  for (auto x : a) EXPECT_EQ(x, expect_a[i++]);
  ASSERT_EQ(b.size(), 1u);
  EXPECT_EQ(b.front(), 5);
}

TEST(ListExtra, SpliceRange) {
  s21::list<int> a{1, 4};
  s21::list<int> b{2, 3, 5};
  auto last = b.begin();
  ++last;
  a.splice(++a.cbegin(), b, b.cbegin(), last);
  int expect_a[] = {1, 2, 4};
  size_t i = 0;
  for (auto x : a) EXPECT_EQ(x, expect_a[i++]);
  int expect_b[] = {3, 5};
  i = 0;
  for (auto x : b) EXPECT_EQ(x, expect_b[i++]);
}

TEST(ListExtra, MergeSorted) {
  s21::list<int> a{1, 3, 5};
  s21::list<int> b{2, 4, 6};
  a.merge(b);
  ASSERT_TRUE(b.empty());
  int expect[] = {1, 2, 3, 4, 5, 6};
  size_t i = 0;
  for (auto x : a) EXPECT_EQ(x, expect[i++]);
}

TEST(ListExtra, UniqueRemovesAdjacentDuplicates) {
  s21::list<int> a{1, 1, 2, 2, 2, 3, 1, 1};
  a.unique();
  int expect[] = {1, 2, 3, 1};
  size_t i = 0;
  for (auto x : a) EXPECT_EQ(x, expect[i++]);
}

TEST(ListExtra, Reverse) {
  s21::list<int> a{1, 2, 3, 4};
  a.reverse();
  int expect[] = {4, 3, 2, 1};
  size_t i = 0;
  for (auto x : a) EXPECT_EQ(x, expect[i++]);
}

TEST(ListExtra, SortMergeSortNodes) {
  s21::list<int> a{5, 3, 1, 4, 2};
  a.sort();
  int expect[] = {1, 2, 3, 4, 5};
  size_t i = 0;
  for (auto x : a) EXPECT_EQ(x, expect[i++]);
}

TEST(ListExtra, InsertManyFrontBack) {
  s21::list<int> a{3};
  a.insert_many_front(2, 1);
  a.insert_many_back(4, 5);

  int expect[] = {1, 2, 3, 4, 5};
  size_t i = 0;
  for (auto x : a) EXPECT_EQ(x, expect[i++]);
}

TEST(ListInsertMany, InsertFrontAndBack) {
  s21::list<int> l = {3, 4};

  l.insert_many_front(2, 1);

  l.insert_many_back(5, 6);

  EXPECT_EQ(l.size(), 6u);
  EXPECT_EQ(l.front(), 1);
  EXPECT_EQ(l.back(), 6);

  s21::list<int> expected = {1, 2, 3, 4, 5, 6};
  auto it_l = l.begin();
  auto it_e = expected.begin();
  for (; it_l != l.end(); ++it_l, ++it_e) {
    EXPECT_EQ(*it_l, *it_e);
  }
}

TEST(ListInsertMany, InsertMiddle_ReturnIterator) {
  s21::list<std::string> l = {"A", "D"};
  auto it_pos = l.end();
  --it_pos;

  l.insert_many(it_pos, "B", "C");

  s21::list<std::string> expected = {"A", "B", "C", "D"};

  auto it_l = l.begin();
  auto it_e = expected.begin();

  for (; it_l != l.end(); ++it_l, ++it_e) {
    EXPECT_EQ(*it_l, *it_e);
  }

  EXPECT_EQ(l.size(), 4u);
}
