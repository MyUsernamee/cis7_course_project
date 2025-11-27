#include "deck.hpp"

Deck::Deck() {
    for (unsigned int suit_index = 0; suit_index < 4; suit_index++)
        for (unsigned int rank_index = 0; rank_index < 13; rank_index++)
            _cards.insert(Card(Card::Suit(suit_index), Card::Rank(rank_index))); // Adds all
                                                                                 // 52 cards to deck.
}

Deck::Deck(std::set<Card> cards) : _cards(cards) {};

bool Deck::has_card(Card card) {
    return _cards.contains(card);
}

bool Deck::take_card(Card card) {
    if (!has_card(card))
        return false;

    _cards.erase(card);
    return true;
}

bool Deck::take_card(Card::Suit suit, Card::Rank rank) {
    return take_card(Card(suit, rank));
}

void Deck::add_card(Card card) {
    _cards.insert(card);
}

std::set<Card> Deck::get_cards() {
    return _cards;
}
