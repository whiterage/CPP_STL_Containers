#include <gtest/gtest.h>

#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#include "../s21_containers.h"

// Vector Tests
TEST(VectorTest, DefaultConstructor) {
  s21::vector<int> v;
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0u);
}

TEST(VectorTest, SizeConstructor) {
  s21::vector<int> v(5);
  EXPECT_FALSE(v.empty());
  EXPECT_EQ(v.size(), 5u);
  for (size_t i = 0; i < v.size(); ++i) {
    EXPECT_EQ(v[i], 0);
  }
}

TEST(VectorTest, InitializerListConstructor) {
  s21::vector<int> v{1, 2, 3, 4, 5};
  EXPECT_EQ(v.size(), 5u);
  for (size_t i = 0; i < v.size(); ++i) {
    EXPECT_EQ(v[i], static_cast<int>(i + 1));
  }
}

TEST(VectorTest, PushBack) {
  s21::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);

  EXPECT_EQ(v.size(), 3u);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, AtMethod) {
  s21::vector<int> v{1, 2, 3};
  EXPECT_EQ(v.at(0), 1);
  EXPECT_EQ(v.at(1), 2);
  EXPECT_EQ(v.at(2), 3);
  EXPECT_THROW(v.at(3), std::out_of_range);
}

// List Tests
TEST(ListTest, DefaultConstructor) {
  s21::list<int> l;
  EXPECT_TRUE(l.empty());
  EXPECT_EQ(l.size(), 0u);
}

TEST(ListTest, InitializerListConstructor) {
  s21::list<int> l{1, 2, 3, 4, 5};
  EXPECT_EQ(l.size(), 5u);

  auto it = l.begin();
  for (int i = 1; i <= 5; ++i, ++it) {
    EXPECT_EQ(*it, i);
  }
}

TEST(ListTest, PushBack) {
  s21::list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  EXPECT_EQ(l.size(), 3u);
  EXPECT_EQ(l.front(), 1);
  EXPECT_EQ(l.back(), 3);
}

TEST(ListTest, PushFront) {
  s21::list<int> l;
  l.push_front(1);
  l.push_front(2);
  l.push_front(3);

  EXPECT_EQ(l.size(), 3u);
  EXPECT_EQ(l.front(), 3);
  EXPECT_EQ(l.back(), 1);
}

TEST(ListTest, Insert) {
  s21::list<int> l{1, 3, 5};
  auto it = l.begin();
  ++it;

  l.insert(it, 2);
  EXPECT_EQ(l.size(), 4u);

  it = l.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(*it, 5);
}

// Stack Tests
TEST(StackTest, DefaultConstructor) {
  s21::stack<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0u);
}

TEST(StackTest, Push) {
  s21::stack<int> s;
  s.push(1);
  s.push(2);
  s.push(3);

  EXPECT_EQ(s.size(), 3u);
  EXPECT_EQ(s.top(), 3);
}

TEST(StackTest, Pop) {
  s21::stack<int> s;
  s.push(1);
  s.push(2);
  s.push(3);

  s.pop();
  EXPECT_EQ(s.size(), 2u);
  EXPECT_EQ(s.top(), 2);
}

// Queue Tests
TEST(QueueTest, DefaultConstructor) {
  s21::queue<int> q;
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q.size(), 0u);
}

TEST(QueueTest, Push) {
  s21::queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);

  EXPECT_EQ(q.size(), 3u);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 3);
}

TEST(QueueTest, Pop) {
  s21::queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);

  q.pop();
  EXPECT_EQ(q.size(), 2u);
  EXPECT_EQ(q.front(), 2);
  EXPECT_EQ(q.back(), 3);
}

// Map Tests
TEST(MapTest, DefaultConstructor) {
  s21::map<int, std::string> m;
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0u);
}

TEST(MapTest, InitializerListConstructor) {
  s21::map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};
  EXPECT_EQ(m.size(), 3u);
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
  EXPECT_EQ(m[3], "three");
}

TEST(MapTest, Insert) {
  s21::map<int, std::string> m;
  auto result = m.insert({1, "one"});

  EXPECT_TRUE(result.second);
  EXPECT_EQ((*result.first).first, 1);
  EXPECT_EQ((*result.first).second, "one");

  // Insert duplicate
  auto result2 = m.insert({1, "ONE"});
  EXPECT_FALSE(result2.second);
  EXPECT_EQ(m.size(), 1u);
}

TEST(MapTest, AtMethod) {
  s21::map<int, std::string> m{{1, "one"}, {2, "two"}};
  EXPECT_EQ(m.at(1), "one");
  EXPECT_EQ(m.at(2), "two");
  EXPECT_THROW(m.at(3), std::out_of_range);
}

// Set Tests
TEST(SetTest, DefaultConstructor) {
  s21::set<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0u);
}

TEST(SetTest, InitializerListConstructor) {
  s21::set<int> s{3, 1, 4, 1, 5, 9, 2, 6};
  EXPECT_EQ(s.size(), 7u);

  auto it = s.begin();
  EXPECT_EQ((*it).first, 1);
  ++it;
  EXPECT_EQ((*it).first, 2);
  ++it;
  EXPECT_EQ((*it).first, 3);
  ++it;
  EXPECT_EQ((*it).first, 4);
  ++it;
  EXPECT_EQ((*it).first, 5);
  ++it;
  EXPECT_EQ((*it).first, 6);
  ++it;
  EXPECT_EQ((*it).first, 9);
}

TEST(SetTest, Insert) {
  s21::set<int> s;
  auto result = s.insert(5);

  EXPECT_TRUE(result.second);
  EXPECT_EQ((*result.first).first, 5);

  auto result2 = s.insert(5);
  EXPECT_FALSE(result2.second);
  EXPECT_EQ(s.size(), 1u);
}

TEST(SetTest, Contains) {
  s21::set<int> s{1, 2, 3, 4, 5};
  EXPECT_TRUE(s.contains(3));
  EXPECT_FALSE(s.contains(6));
}

TEST(SetTest, Find) {
  s21::set<int> s{1, 2, 3, 4, 5};
  auto it = s.find(3);
  EXPECT_NE(it, s.end());
  EXPECT_EQ((*it).first, 3);

  auto it2 = s.find(6);
  EXPECT_EQ(it2, s.end());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
