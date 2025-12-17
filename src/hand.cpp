
#include "hand.hpp"
#include "card.hpp"

int Hand::get_value(bool soft_hand) {
    int value = 0;

    for (auto card : _cards) {
        int hand_value = card.get_value(soft_hand);
        value += hand_value;
    }

    return value;
} 
int Hand::get_value() {
    int value = get_value(false);
    if (value > 21)
        value = get_value(true);

    return value;
} 

bool Hand::empty() {
    return _cards.empty();
}

void Hand::add_card(Card card) {
    _cards.insert(card);
} 

void Hand::add_cards(std::set<Card> cards) {
    _cards.insert(cards.begin(), cards.end());
} 

void Hand::deal_cards(Deck& deck, int count) {
    for (int i = 0; i < count; i++) 
        _cards.insert(deck.take_random_card());
} 

void Hand::deal_card(Deck& deck) {
    deal_cards(deck, 1);
}

std::set<Card> Hand::get_cards() {
    return _cards;
} 

void Hand::clear() {
    _cards.clear();
} 

double Hand::get_bust_probability(Deck deck) {
    return get_score_probability(deck, 21);
} 

double Hand::get_score_probability(Deck deck, int score) {
    int value = get_value();
    if (value > score)
        return 1.0; // We are already higher, 100% chance next is going to be higher.
    
    int higher_count = 0;

    for (auto card : deck.get_cards()) {
        int possible_value = value + card.get_value(true); // Assume soft;

        if (possible_value > score)
            higher_count += 1;
    }

    return (double)higher_count / (double)deck.get_cards().size();
} 

std::string Hand::as_string() {
    std::string ret = "";
    for (auto card: _cards) {
        ret += card.as_string();
    }

    return ret;
} 
