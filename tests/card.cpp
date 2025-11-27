#include <gtest/gtest.h>

#include "card.hpp"

TEST(Card, AsString) {
    EXPECT_EQ(Card(Card::DIAMONDS, Card::KING).as_string(), "\U0001F0CE");
}

TEST(Card, RankAsChar) {
    EXPECT_EQ(Card::rank_as_char(Card::ACE), 'A');
    EXPECT_EQ(Card::rank_as_char(Card::FIVE), '5');
}

TEST(Card, SuitAsChar) {
    EXPECT_EQ(Card::suit_as_char(Card::HIDDEN), 'H');
    EXPECT_EQ(Card::suit_as_char(Card::DIAMONDS), 'D');
}
