#include "deck.hpp"
#include <random>
#include <algorithm>

Deck::Deck() {
    for (unsigned int suit_index = 0; suit_index < 4; suit_index++)
        for (unsigned int rank_index = 0; rank_index < 13; rank_index++)
            _cards.insert(Card(Card::Suit(suit_index), Card::Rank(rank_index))); // Adds all
                                                                                 // 52 cards to deck.
}

Deck::Deck(Deck& other) {
    _cards = std::set(other._cards);
    _missing_cards = std::set(other._missing_cards);
}

Deck::Deck(std::set<Card> cards) : _cards(cards) {};

bool Deck::has_card(Card card) {
    return _cards.contains(card);
}

bool Deck::take_card(Card card) {
    if (!has_card(card))
        return false;

    _cards.erase(card);
    _missing_cards.insert(card);
    return true;
}

bool Deck::take_card(Card::Suit suit, Card::Rank rank) {
    return take_card(Card(suit, rank));
}

Card Deck::take_random_card() {
    static std::random_device dev;
    static std::mt19937 rng(dev()); // Bruh, average C++ standard library

    std::uniform_int_distribution<std::mt19937::result_type> dist(0, _cards.size() - 1);

    auto it = _cards.begin();
    std::advance(it, dist(rng));

    auto card = *it;
    _cards.erase(it); // Like what is this, who thought this was better than the 900 million other more simpler ways to do this.
    _missing_cards.insert(card);

    return card;
}

void Deck::add_card(Card card) {
    _cards.insert(card);
    _missing_cards.erase(card);
}

std::set<Card> Deck::get_cards() {
    return _cards;
}

int Deck::num_cards_less_than(int value) {
    int num = 0;

    if (value < 10)
        num += value * 4; // Num ranks less than * num suits.
    else 
        num += 52; // All cards have a min value less than or equal to value

    for (auto missing_card: _missing_cards) {
        if (missing_card.get_value(true) < value)
            num -= 1;
    }

    return num;
}

