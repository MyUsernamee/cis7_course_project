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
    EXPECT_EQ(Card::suit_as_char(Card::HIDDEN), '#');
    EXPECT_EQ(Card::suit_as_char(Card::DIAMONDS), 'D');
}

TEST(Card, RankFromChar) {
    EXPECT_EQ(Card::rank_from_char('A'), Card::ACE);
    EXPECT_EQ(Card::rank_from_char('5'), Card::FIVE);
}

TEST(Card, SuitFromChar) {
    EXPECT_EQ(Card::suit_from_char('#'), Card::HIDDEN);
    EXPECT_EQ(Card::suit_from_char('D'), Card::DIAMONDS);
}

TEST(Card, StringCTor) {
    EXPECT_NO_THROW(Card("2H"));
    EXPECT_NO_THROW(Card("#"));
    EXPECT_ANY_THROW(Card("x"));

    EXPECT_EQ(Card("4H")._rank, Card::FOUR);
}
