#include <gtest/gtest.h>
#include "deck.hpp"

TEST(Deck, CTor) {
    Deck deck = Deck();

    EXPECT_EQ(deck.get_cards().size(), 52);
    EXPECT_TRUE(deck.has_card(Card(Card::CLUBS, Card::EIGHT)));
    EXPECT_TRUE(deck.has_card(Card("KS")));

    EXPECT_TRUE(deck.take_card(Card("KH")));
    EXPECT_FALSE(deck.has_card(Card("KH")));
    EXPECT_EQ(deck.get_cards().size(), 51);
}
