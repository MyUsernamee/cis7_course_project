#include <gtest/gtest.h>

#include "card.hpp"

TEST(Card, AsString) {
    EXPECT_EQ(Card(Card::DIAMONDS, Card::KING).as_string(), "\U0001F0CE");
}

TEST(Card, RankAsString) {
    EXPECT_EQ(Card(Card::DIAMONDS, Card::KING).as_string(), "\U0001F0CE");
}
