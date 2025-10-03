#include <gtest/gtest.h>

#include "../s21_containers.h"

TEST(VectorExtra, ReserveAndCapacityGrowth) {
  s21::vector<int> v;
  EXPECT_EQ(v.capacity(), 0u);
  v.reserve(10);
  EXPECT_GE(v.capacity(), 10u);
  EXPECT_EQ(v.size(), 0u);
  v.push_back(1);
  v.push_back(2);
  EXPECT_EQ(v.size(), 2u);
}

TEST(VectorExtra, ShrinkToFit) {
  s21::vector<int> v{1, 2, 3, 4, 5};
  const auto cap_before = v.capacity();
  v.pop_back();
  v.pop_back();
  EXPECT_EQ(v.size(), 3u);
  v.shrink_to_fit();
  EXPECT_EQ(v.capacity(), v.size());
  EXPECT_LE(v.capacity(), cap_before);
}

TEST(VectorExtra, InsertEraseMiddle) {
  s21::vector<int> v{1, 2, 5, 6};
  auto it = v.begin() + 2;
  v.insert(it, 3);
  v.insert(v.begin() + 3, 4);
  ASSERT_EQ(v.size(), 6u);
  for (int i = 0; i < 6; ++i) EXPECT_EQ(v[i], i + 1);

  v.erase(v.begin() + 2);
  v.erase(v.begin() + 2);
  ASSERT_EQ(v.size(), 4u);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 5);
  EXPECT_EQ(v[3], 6);
}

TEST(VectorExtra, InsertManyAndInsertManyBack) {
  s21::vector<int> v{1, 2};
  v.insert_many(v.begin() + 1, 7, 8, 9);
  ASSERT_EQ(v.size(), 5u);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 7);
  EXPECT_EQ(v[2], 8);
  EXPECT_EQ(v[3], 9);
  EXPECT_EQ(v[4], 2);

  v.insert_many_back(10, 11);
  ASSERT_EQ(v.size(), 7u);
  EXPECT_EQ(v[5], 10);
  EXPECT_EQ(v[6], 11);
}

TEST(VectorInsertMany, Middle_Normal) {
  s21::vector<int> v = {10, 50, 60};
  auto it = v.begin() + 1;

  auto first_inserted_it = v.insert_many(it, 20, 30, 40);

  EXPECT_EQ(v.size(), 6u);
  EXPECT_EQ(*first_inserted_it, 20);
  EXPECT_EQ(v[0], 10);
  EXPECT_EQ(v[1], 20);
  EXPECT_EQ(v[2], 30);
  EXPECT_EQ(v[3], 40);
  EXPECT_EQ(v[4], 50);
  EXPECT_EQ(v[5], 60);
}

TEST(VectorInsertMany, Begin_ZeroArgs) {
  s21::vector<int> v = {1, 2, 3};

  auto result_it = v.insert_many(v.begin(), 100);
  v.erase(result_it);

  auto final_it = v.insert_many(v.begin());

  EXPECT_EQ(v.size(), 3u);
  EXPECT_EQ(*final_it, *v.begin());
  EXPECT_EQ(*v.begin(), 1);
}

TEST(VectorInsertMany, End_CapacityIncrease) {
  s21::vector<int> v;
  v.reserve(2);

  v.push_back(1);
  v.push_back(2);
  const auto old_capacity = v.capacity();

  v.insert_many(v.end(), 3, 4, 5, 6, 7);

  EXPECT_EQ(v.size(), 7u);
  EXPECT_GT(v.capacity(), old_capacity);
  EXPECT_EQ(v.back(), 7);
}