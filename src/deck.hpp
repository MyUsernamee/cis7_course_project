#pragma once

#include <set>
#include "card.hpp"

///
/// \brief A class representing a deck of cards.
/// This internally uses a `std::set` to represent cards, because we assume have no duplicates of cards.
///
class Deck {
    
public:
    Deck();
    Deck(std::set<Card> cards);

    bool take_card(Card card); ///< \brief Attempts to remove a card from the deck. 
                               ///< Returns true if the deck contains the card and removes if it does.
    bool take_card(Card::Suit suit, Card::Rank rank); ///< Same as `Deck::take_card(Card card)` but
                                                      ///< with rank and suit instead.

    void add_card(Card card); ///< Adds a card to the deck, does nothing if card is already in deck.
    bool add_card(Card::Rank rank, Card::Suit suit); ///< Same as `Deck::add_card(Card card)` but
                                                     ///< with rank and suit instead.

    std::set<Card> get_cards(); ///< Returns a `std::set` containing all cards in the deck
    bool has_card(Card card); ///< Returns true if the deck contains this card.

private:
    std::set<Card> _cards;

};
