#pragma once

#include <set>
#include <string>
#include "card.hpp"
#include "deck.hpp"

/// Represents a single hand.
class Hand {


public:
    int get_value(bool soft_hand); ///> Get's the value of the hand according to blackjack rules.
    int get_value(); ///> Get's the value of the hand according to blackjack rules. Chooses higheset non winning hand;

    bool empty(); ///< Returns true if this hand has no cards.

    void add_card(Card card); ///> Adds a card to this hand.
    void add_cards(std::set<Card> cards); ///> Add a collection of cards to this hand.

    void deal_cards(Deck& deck, int count); ///> Takes `count` card from the deck.
    void deal_card(Deck& deck); ///> Deals one card from the deck.

    std::set<Card> get_cards(); ///> Get cards in this hand.

    void clear(); ///> Clears the hand.

    double get_bust_probability(Deck deck); ///> Gets the probability this hand, given deck, will be dealt a card higher than 21.
    double get_score_probability(Deck deck, int score); ///> Gets the probability if dealt cards from this deck that this hand will get a score higher than that.
    std::string as_string(); ///> Get a string representation of this hand.

private:
    std::set<Card> _cards; ///> The cards in this hand.

};
