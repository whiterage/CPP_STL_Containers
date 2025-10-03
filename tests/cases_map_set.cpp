#include <gtest/gtest.h>

#include "../s21_containers.h"

// MAP

TEST(MapExtra, InsertOrAssign) {
  s21::map<int, std::string> m{{1, "one"}};
  auto [it1, inserted1] = m.insert_or_assign(1, "ONE");
  EXPECT_FALSE(inserted1);
  EXPECT_EQ((*it1).second, std::string("ONE"));

  auto [it2, inserted2] = m.insert_or_assign(2, "two");
  EXPECT_TRUE(inserted2);
  EXPECT_EQ((*it2).second, std::string("two"));
  EXPECT_EQ(m.size(), 2u);
}

TEST(MapExtra, OperatorBracketInsertsDefault) {
  s21::map<int, int> m;
  EXPECT_EQ(m.size(), 0u);
  m[42] = 7;
  EXPECT_EQ(m.size(), 1u);
  EXPECT_EQ(m.at(42), 7);
}

TEST(MapExtra, EraseIterator) {
  s21::map<int, int> m{{1, 1}, {2, 2}, {3, 3}};
  auto it = m.begin();
  ++it;
  m.erase(it);
  EXPECT_EQ(m.size(), 2u);
  EXPECT_TRUE(m.contains(1));
  EXPECT_FALSE(m.contains(2));
  EXPECT_TRUE(m.contains(3));
}

TEST(MapExtra, InsertManyAndEmplace) {
  s21::map<int, int> m;
  auto res = m.insert_many(std::make_pair(1, 10), std::make_pair(2, 20));
  ASSERT_EQ(res.size(), 2u);
  EXPECT_TRUE(res[0].second);
  EXPECT_TRUE(res[1].second);

  auto [it, ok] = m.emplace(3, 30);
  EXPECT_TRUE(ok);
  EXPECT_EQ((*it).second, 30);
}

TEST(MapErase, EraseRebalance_Case4_MirrorFixesViolation) {
  s21::map<int, int> m;

  m.insert({20, 0});
  m.insert({10, 0});
  m.insert({30, 0});
  m.insert({35, 0});
  m.insert({5, 0});

  m.clear();
  m.insert({10, 0});
  m.insert({5, 0});
  m.insert({20, 0});
  m.insert({30, 0});
  m.insert({15, 0});

  auto it_to_erase = m.find(5);
  ASSERT_NE(it_to_erase, m.end());
  m.erase(it_to_erase);

  EXPECT_EQ(m.size(), 4u);
  EXPECT_FALSE(m.contains(5));
}

TEST(MapErase, EraseRebalance_Case2_ViolationPropagatesUp) {
  s21::map<int, int> m;

  m.insert({20, 0});
  m.insert({10, 0});
  m.insert({30, 0});
  m.insert({25, 0});
  m.insert({35, 0});
  m.insert({5, 0});

  m.clear();
  m.insert({30, 0});
  m.insert({20, 0});
  m.insert({40, 0});
  m.insert({10, 0});
  m.insert({25, 0});

  auto it_to_erase = m.find(40);
  ASSERT_NE(it_to_erase, m.end());
  m.erase(it_to_erase);

  EXPECT_EQ(m.size(), 4u);
  EXPECT_FALSE(m.contains(40));
}

TEST(RedBlackTreeRotation, RotateRight_YIsRightChild) {
  s21::map<int, int> m;

  m.insert({40, 0});
  m.insert({30, 0});
  m.insert({50, 0});

  m.insert({60, 0});
  m.insert({70, 0});

  m.clear();
  m.insert({50, 0});
  m.insert({60, 0});
  m.insert({55, 0});

  m.clear();
  m.insert({20, 0});
  m.insert({30, 0});
  m.insert({40, 0});
  m.insert({10, 0});
  m.insert({25, 0});

  EXPECT_EQ(m.size(), 5u);
  EXPECT_TRUE(m.contains(25));

  EXPECT_TRUE(m.contains(30));
}

TEST(RedBlackTreeRotation, RotateRight_YIsLeftChild) {
  s21::map<int, int> m;

  m.insert({40, 0});
  m.insert({20, 0});
  m.insert({60, 0});
  m.insert({10, 0});
  m.insert({30, 0});

  m.insert({5, 0});

  EXPECT_EQ(m.size(), 6u);
  EXPECT_TRUE(m.contains(5));

  EXPECT_TRUE(m.contains(10));
}

TEST(MapEraseRebalance, Case3_RightNearNephewIsRed_1) {
  s21::map<int, int> m;

  m.insert({30, 0});
  m.insert({20, 0});
  m.insert({40, 0});
  m.insert({10, 0});
  m.insert({25, 0});
  m.insert({35, 0});

  auto it_to_erase = m.find(30);
  ASSERT_NE(it_to_erase, m.end());
  m.erase(it_to_erase);

  EXPECT_EQ(m.size(), 5u);
  EXPECT_FALSE(m.contains(30));
  EXPECT_TRUE(m.contains(25));
  EXPECT_TRUE(m.contains(40));
}

TEST(MapEraseRebalance, Case4_LeftFarNephewIsRed_2) {
  s21::map<int, int> m;

  m.insert({20, 0});
  m.insert({10, 0});
  m.insert({30, 0});
  m.insert({40, 0});

  m.insert({50, 0});
  m.insert({45, 0});
  m.erase(m.find(10));

  EXPECT_EQ(m.size(), 5u);
  EXPECT_FALSE(m.contains(10));
  EXPECT_TRUE(m.contains(40));
}

TEST(MapEraseOneLeftChild, EraseNodeWithOneLeftChild) {
  s21::map<int, int> m;

  m.insert({30, 0});
  m.insert({20, 0});
  m.insert({40, 0});

  auto it_to_erase_right = m.find(40);
  ASSERT_NE(it_to_erase_right, m.end());
  m.erase(it_to_erase_right);

  auto it_to_erase_target = m.find(30);
  ASSERT_NE(it_to_erase_target, m.end());
  m.erase(it_to_erase_target);

  EXPECT_EQ(m.size(), 1u);
  EXPECT_TRUE(m.contains(20));
  EXPECT_FALSE(m.contains(30));
}

TEST(MapUpdateNodesTest, UpdateMinMaxAfterLastErase) {
  s21::map<int, int> m;

  m.insert({42, 100});
  EXPECT_EQ(m.size(), 1u);

  auto it_to_erase = m.begin();
  m.erase(it_to_erase);

  EXPECT_EQ(m.size(), 0u);
  EXPECT_TRUE(m.empty());
}

// SET

TEST(SetExtra, InsertMany) {
  s21::set<int> s;
  auto res = s.insert_many(3, 1, 2, 3);
  ASSERT_EQ(res.size(), 4u);
  EXPECT_TRUE(res[0].second);
  EXPECT_TRUE(res[1].second);
  EXPECT_TRUE(res[2].second);
  EXPECT_FALSE(res[3].second);
}

TEST(SetExtra, EraseIterator) {
  s21::set<int> s{1, 2, 3};
  auto it = s.begin();
  ++it;
  s.erase(it);
  EXPECT_EQ(s.size(), 2u);
  EXPECT_TRUE(s.contains(1));
  EXPECT_FALSE(s.contains(2));
  EXPECT_TRUE(s.contains(3));
}

// INASERT_MANY

TEST(MapInsertMany, MixedNewAndDuplicates) {
  s21::map<int, std::string> m = {{10, "ten"}, {30, "thirty"}};

  auto results = m.insert_many(
      std::make_pair(5, "five"), std::make_pair(10, "duplicate"),
      std::make_pair(20, "twenty"), std::make_pair(30, "thirty_new"),
      std::make_pair(40, "forty"));

  EXPECT_EQ(m.size(), 5u);
  EXPECT_EQ(results.size(), 5u);

  EXPECT_TRUE(results[0].second);

  EXPECT_FALSE(results[1].second);
  EXPECT_EQ((*results[1].first).second, "ten");

  EXPECT_TRUE(results[2].second);

  EXPECT_FALSE(results[3].second);

  EXPECT_TRUE(results[4].second);
}

TEST(MapInsertMany, ZeroArgs) {
  s21::map<int, int> m = {{1, 1}};
  auto results = m.insert_many();

  EXPECT_EQ(m.size(), 1u);
  EXPECT_TRUE(results.empty());
}

TEST(SetInsertMany, DuplicatesAndNew) {
  s21::set<int> s = {1, 5};

  auto results = s.insert_many(0, 1, 3, 5, 10);

  EXPECT_EQ(s.size(), 5u);
  EXPECT_EQ(results.size(), 5u);

  EXPECT_TRUE(results[0].second);

  EXPECT_FALSE(results[1].second);

  EXPECT_TRUE(results[2].second);

  EXPECT_FALSE(results[3].second);

  EXPECT_TRUE(results[4].second);
}

TEST(QueueInsertMany, InsertMultipleElements) {
  s21::queue<int> q;

  q.insert_many_back(1, 2, 3);

  EXPECT_EQ(q.size(), 3u);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 3);

  q.pop();
  EXPECT_EQ(q.front(), 2);
}

TEST(StackInsertMany, InsertMultipleElements) {
  s21::stack<int> s;

  s.insert_many_back(1, 2, 3);

  EXPECT_EQ(s.size(), 3u);
  EXPECT_EQ(s.top(), 3);

  s.pop();
  EXPECT_EQ(s.top(), 2);
}