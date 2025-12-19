#include <gtest/gtest.h>
#include "hand.hpp"

double slow_prob(Hand hand, Deck deck, int value) {
    int count = 0;
    for (auto card: deck.get_cards()){
        if (hand.get_value() + card.get_value(true) > value)
            count += 1;
    }
    return (double)count / deck.get_cards().size();
}

double slow_prob_bust(Hand hand, Deck deck) {
    return slow_prob(hand, deck, 21);
}

TEST(Hand, Probability) {
    Deck deck = Deck();
    Hand hand = Hand();

    EXPECT_EQ(deck.get_cards().size(), 52);
    EXPECT_EQ(hand.get_cards().size(), 0);

    EXPECT_FLOAT_EQ(hand.get_bust_probability(deck), 0.0);
    for (int i = 0; i < 11; i++) {
        EXPECT_FLOAT_EQ(hand.get_score_probability(deck, i), slow_prob(hand, deck, i));
    }
    
    auto card = Card(Card::DIAMONDS, Card::JACK);
    deck.take_card(card);
    hand.add_card(card);

    EXPECT_FLOAT_EQ(hand.get_bust_probability(deck), slow_prob_bust(hand, deck));

    for (int j = 0; j < 10; j++) {

        hand.deal_card(deck);
        EXPECT_FLOAT_EQ(hand.get_bust_probability(deck), slow_prob_bust(hand, deck));
        for (int i = 0; i < 11; i++) {
            EXPECT_FLOAT_EQ(hand.get_score_probability(deck, i), slow_prob(hand, deck, i));
        }
    }

}
