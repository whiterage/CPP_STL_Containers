#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <numeric>

#include "../s21_containersplus.h"

using s21::array;
using s21::multiset;

TEST(arrayTest, DefaultConstructor) {
  array<int, 5> a;
  EXPECT_EQ(a.size(), 5u);
  EXPECT_FALSE(a.empty());
}

TEST(arrayTest, InitListConstructor) {
  array<int, 3> a{1, 2, 3};
  EXPECT_EQ(a[0], 1);
  EXPECT_EQ(a[2], 3);
}

TEST(ArrayTest, InitListTooMany) {
  s21::array<int, 2> a{1, 2, 3};

  EXPECT_EQ(a.size(), 2u);

  EXPECT_EQ(a[0], 1);
  EXPECT_EQ(a[1], 2);
}

TEST(arrayTest, CopyConstructor) {
  array<int, 3> a{10, 20, 30};
  array<int, 3> b(a);
  EXPECT_EQ(b[1], 20);
}

TEST(arrayTest, MoveConstructor) {
  array<int, 3> a{1, 2, 3};
  array<int, 3> b(std::move(a));
  EXPECT_EQ(b[0], 1);
}

TEST(arrayTest, MoveAssignment) {
  array<int, 3> a{4, 5, 6};
  array<int, 3> b;
  b = std::move(a);
  EXPECT_EQ(b[2], 6);
}

TEST(arrayTest, AtMethod) {
  array<char, 3> a{'x', 'y', 'z'};
  EXPECT_EQ(a.at(2), 'z');
  EXPECT_THROW(a.at(5), std::out_of_range);
}

TEST(arrayTest, FrontBackAccess) {
  array<std::string, 2> a{"front", "back"};
  EXPECT_EQ(a.front(), "front");
  EXPECT_EQ(a.back(), "back");
}

TEST(arrayTest, Iterators) {
  array<int, 3> a{1, 2, 3};
  int sum = std::accumulate(a.begin(), a.end(), 0);
  EXPECT_EQ(sum, 6);
}

TEST(arrayTest, Swap) {
  array<int, 2> a{1, 2}, b{10, 20};
  a.swap(b);
  EXPECT_EQ(a[0], 10);
  EXPECT_EQ(b[1], 2);
}

TEST(arrayTest, Fill) {
  array<int, 4> a;
  a.fill(7);
  for (int v : a) EXPECT_EQ(v, 7);
}

TEST(arrayTest, DataPointer) {
  array<double, 2> a{3.14, 2.71};
  auto* ptr = a.data();
  EXPECT_DOUBLE_EQ(ptr[1], 2.71);
}

TEST(multisetTest, DefaultConstructor) {
  multiset<int> ms;
  EXPECT_TRUE(ms.empty());
  EXPECT_EQ(ms.size(), 0u);
}

TEST(multisetTest, InitListConstructor_Basic) {
  multiset<int> ms{30, 10, 20};
  EXPECT_EQ(ms.size(), 3u);

  auto it = ms.begin();
  EXPECT_EQ((*it++).first, 10);
  EXPECT_EQ((*it++).first, 20);
  EXPECT_EQ((*it++).first, 30);
}

TEST(multisetTest, InitListConstructor_Duplicates) {
  multiset<int> ms{30, 10, 20, 10, 30, 5};
  EXPECT_EQ(ms.size(), 6u);

  std::vector<int> expected = {5, 10, 10, 20, 30, 30};

  std::vector<int> actual;

  for (auto it = ms.begin(); it != ms.end(); ++it) {
    actual.push_back((*it).first);
  }

  EXPECT_EQ(actual, expected);
}

TEST(multisetTest, CopyConstructor) {
  multiset<int> original{1, 2, 2, 3};
  multiset<int> copy(original);
  EXPECT_EQ(copy.size(), 4u);
  EXPECT_EQ(original.size(), 4u);
  copy.insert(5);
  EXPECT_EQ(copy.size(), 5u);
  EXPECT_EQ(original.size(), 4u);
}

TEST(multisetTest, MoveConstructor) {
  multiset<int> original{5, 1, 5};
  multiset<int> moved(std::move(original));
  EXPECT_EQ(moved.size(), 3u);
  EXPECT_TRUE(original.empty());
  EXPECT_EQ(original.size(), 0u);
}

TEST(multisetTest, CopyAssignment) {
  multiset<int> ms1{10, 20, 30};
  multiset<int> ms2{5, 5};
  ms2 = ms1;
  EXPECT_EQ(ms2.size(), 3u);
  EXPECT_EQ((*ms2.begin()).first, 10);
  ms1 = ms1;
  EXPECT_EQ(ms1.size(), 3u);
}

TEST(multisetTest, MoveAssignment) {
  multiset<int> ms1{10, 20, 10};
  multiset<int> ms2{5, 5};
  ms2 = std::move(ms1);
  EXPECT_EQ(ms2.size(), 3u);
  EXPECT_EQ((*ms2.begin()).first, 10);
  EXPECT_TRUE(ms1.empty());
  EXPECT_EQ(ms1.size(), 0u);
}

TEST(multisetTest, CapacityEmptySize) {
  multiset<int> ms;
  EXPECT_TRUE(ms.empty());
  EXPECT_EQ(ms.size(), 0u);

  ms.insert(10);
  ms.insert(10);
  EXPECT_FALSE(ms.empty());
  EXPECT_EQ(ms.size(), 2u);

  ms.clear();
  EXPECT_TRUE(ms.empty());
  EXPECT_EQ(ms.size(), 0u);
}

TEST(multisetTest, CapacityMaxSize) {
  multiset<int> ms;
  EXPECT_GT(ms.max_size(), 1000000u);
}

TEST(multisetTest, IteratorsBeginEnd) {
  multiset<int> ms{3, 1, 2};
  EXPECT_EQ((*ms.begin()).first, 1);
  EXPECT_NE(ms.begin(), ms.end());

  multiset<int> empty_ms;
  EXPECT_EQ(empty_ms.begin(), empty_ms.end());
}

TEST(multisetTest, IteratorsTraversal) {
  multiset<int> ms{3, 1, 2, 2, 4};
  std::vector<int> expected = {1, 2, 2, 3, 4};
  std::vector<int> actual;

  for (auto it = ms.begin(); it != ms.end(); ++it) {
    actual.push_back((*it).first);
  }
  EXPECT_EQ(actual, expected);

  actual.clear();
  for (auto it = ms.end(); it != ms.begin();) {
    --it;
    actual.push_back((*it).first);
  }
  std::reverse(actual.begin(), actual.end());
  EXPECT_EQ(actual, expected);
}

TEST(multisetTest, ConstIterators) {
  const multiset<int> ms{10, 20, 10};
  auto it = ms.begin();
  EXPECT_EQ((*it).first, 10);
  ++it;
  EXPECT_EQ((*it).first, 10);
  ++it;
  EXPECT_EQ((*it).first, 20);
}

TEST(multisetTest, ModifiersClear) {
  multiset<int> ms{1, 2, 2, 3};
  ms.clear();
  EXPECT_TRUE(ms.empty());
  EXPECT_EQ(ms.size(), 0u);
  EXPECT_EQ(ms.begin(), ms.end());
}

TEST(multisetTest, ModifiersInsertBasic) {
  multiset<int> ms;
  ms.insert(20);
  ms.insert(10);
  auto it = ms.insert(30);

  EXPECT_EQ(ms.size(), 3u);
  EXPECT_EQ((*it).first, 30);
  EXPECT_EQ((*ms.begin()).first, 10);
}

TEST(multisetTest, ModifiersInsertDuplicates) {
  multiset<int> ms;
  ms.insert(10);
  auto it1 = ms.insert(20);
  auto it2 = ms.insert(10);
  auto it3 = ms.insert(10);

  EXPECT_EQ(ms.size(), 4u);
  EXPECT_EQ((*it1).first, 20);
  EXPECT_EQ((*it2).first, 10);
  EXPECT_EQ((*it3).first, 10);

  std::vector<int> expected = {10, 10, 10, 20};
  std::vector<int> actual;
  for (auto it = ms.begin(); it != ms.end(); ++it) {
    actual.push_back((*it).first);
  }
  EXPECT_EQ(actual, expected);
}

TEST(multisetTest, ModifiersEraseSingle) {
  multiset<int> ms{10, 20, 30};
  auto it = ms.find(20);
  ms.erase(it);

  EXPECT_EQ(ms.size(), 2u);
  EXPECT_EQ((*ms.begin()).first, 10);
  EXPECT_EQ((*std::next(ms.begin())).first, 30);
  EXPECT_TRUE(ms.find(20) == ms.end());
}

TEST(multisetTest, ModifiersEraseDuplicates) {
  multiset<int> ms{10, 20, 10, 30, 10};
  EXPECT_EQ(ms.size(), 5u);

  auto it_first_10 = ms.find(10);
  ms.erase(it_first_10);

  EXPECT_EQ(ms.size(), 4u);
  EXPECT_EQ((*ms.begin()).first, 10);

  auto it_next_10 = ms.find(10);
  if (it_next_10 != ms.end() && (*it_next_10).first == 10) {
    ms.erase(it_next_10);
  }

  EXPECT_EQ(ms.size(), 3u);
  EXPECT_EQ((*ms.begin()).first, 10);

  ms.erase(ms.find(10));
  EXPECT_EQ(ms.size(), 2u);
  EXPECT_EQ((*ms.begin()).first, 20);

  ms.erase(ms.end());
  EXPECT_EQ(ms.size(), 2u);
}

TEST(multisetTest, ModifiersSwap) {
  multiset<int> ms1{1, 2, 2, 3};
  multiset<int> ms2{10, 20, 30, 40, 50};

  ms1.swap(ms2);

  EXPECT_EQ(ms1.size(), 5u);
  EXPECT_EQ((*ms1.begin()).first, 10);
  EXPECT_EQ(ms2.size(), 4u);
  EXPECT_EQ((*ms2.begin()).first, 1);
}

TEST(multisetTest, ModifiersMerge) {
  multiset<int> ms1{10, 30, 10};
  multiset<int> ms2{5, 20, 30, 5};

  ms1.merge(ms2);

  EXPECT_EQ(ms1.size(), 7u);
  EXPECT_TRUE(ms2.empty());

  std::vector<int> expected = {5, 5, 10, 10, 20, 30, 30};
  std::vector<int> actual;
  for (auto it = ms1.begin(); it != ms1.end(); ++it) {
    actual.push_back((*it).first);
  }

  EXPECT_EQ(actual, expected);

  multiset<int> ms3{1, 2};
  ms3.merge(ms3);
  EXPECT_EQ(ms3.size(), 2u);
}

TEST(multisetTest, ModifiersInsertMany) {
  multiset<int> ms{10};

  int a = 5;
  std::string s1 = "a";
  std::string s2 = "b";
  auto results = ms.insert_many(a, 20, 10, 5);

  EXPECT_EQ(ms.size(), 5u);

  EXPECT_EQ((*results[1]).first, 20);
  EXPECT_EQ((*results[0]).first, 5);
  EXPECT_EQ((*results[2]).first, 10);
  EXPECT_EQ((*results[3]).first, 5);

  multiset<std::string> ms_str;
  auto str_results = ms_str.insert_many("c", std::move(s2), "a", "c");
  EXPECT_EQ(ms_str.size(), 4u);
  EXPECT_EQ(ms_str.count("c"), 2u);

  std::vector<std::string> str_expected = {"a", "b", "c", "c"};
  std::vector<std::string> str_actual;
  for (auto it = ms_str.begin(); it != ms_str.end(); ++it) {
    str_actual.push_back((*it).first);
  }
  EXPECT_EQ(str_actual, str_expected);
}

TEST(multisetTest, LookupCount) {
  multiset<int> ms{10, 20, 10, 30, 10};

  EXPECT_EQ(ms.count(10), 3u);
  EXPECT_EQ(ms.count(20), 1u);
  EXPECT_EQ(ms.count(30), 1u);
  EXPECT_EQ(ms.count(99), 0u);

  multiset<int> empty_ms;
  EXPECT_EQ(empty_ms.count(1), 0u);
}

TEST(multisetTest, LookupFind) {
  multiset<int> ms{3, 1, 3, 2, 3};

  auto it_found = ms.find(2);
  EXPECT_NE(it_found, ms.end());
  EXPECT_EQ((*it_found).first, 2);

  auto it_first_3 = ms.find(3);
  EXPECT_NE(it_first_3, ms.end());
  EXPECT_EQ((*it_first_3).first, 3);

  auto it_lb_3 = ms.lower_bound(3);
  EXPECT_EQ(it_first_3, it_lb_3);
}

TEST(multisetTest, LookupContains) {
  multiset<std::string> ms{"a", "b", "a", "c"};

  EXPECT_TRUE(ms.contains("a"));
  EXPECT_TRUE(ms.contains("b"));
  EXPECT_FALSE(ms.contains("d"));
}

TEST(multisetTest, LookupLowerBound) {
  multiset<int> ms{10, 20, 30, 20, 20};

  auto it20 = ms.lower_bound(20);
  EXPECT_EQ((*it20).first, 20);

  auto it15 = ms.lower_bound(15);
  EXPECT_EQ((*it15).first, 20);

  auto it40 = ms.lower_bound(40);
  EXPECT_EQ(it40, ms.end());

  EXPECT_EQ((*ms.lower_bound(10)).first, 10);
}

TEST(multisetTest, LookupUpperBound) {
  multiset<int> ms{10, 20, 30, 20, 20};

  auto it20 = ms.upper_bound(20);
  EXPECT_EQ((*it20).first, 30);

  auto it15 = ms.upper_bound(15);
  EXPECT_EQ((*it15).first, 20);

  auto it40 = ms.upper_bound(40);
  EXPECT_EQ(it40, ms.end());
}

TEST(multisetTest, LookupEqualRange) {
  multiset<int> ms{10, 20, 30, 20, 20};

  auto range20 = ms.equal_range(20);
  EXPECT_EQ((*range20.first).first, 20);
  EXPECT_EQ((*range20.second).first, 30);
  EXPECT_EQ(std::distance(range20.first, range20.second), 3);

  auto range30 = ms.equal_range(30);
  EXPECT_EQ((*range30.first).first, 30);
  EXPECT_EQ(range30.second, ms.end());
  EXPECT_EQ(std::distance(range30.first, range30.second), 1);

  auto range15 = ms.equal_range(15);
  EXPECT_EQ((*range15.first).first, 20);
  EXPECT_EQ((*range15.second).first, 20);
  EXPECT_EQ(range15.first, range15.second);
  EXPECT_EQ(std::distance(range15.first, range15.second), 0);
}