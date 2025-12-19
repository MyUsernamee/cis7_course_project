
#include "hand.hpp"
#include "card.hpp"

Hand::Hand() {

}

int Hand::get_value() {
    int value = 0;
    int num_aces = 0;

    for (auto card : _cards) {
        if (card._rank == Card::ACE) {
            num_aces += 1;
            continue;
        }
        int hand_value = card.get_value(false);
        value += hand_value;
    }
    
    int num_11_aces = std::min((21 - value) / 11, num_aces);
    value += num_11_aces * 11 + (num_aces - num_11_aces);

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

std::set<Card>& Hand::get_cards() {
    return _cards;
} 

void Hand::clear() {
    _cards.clear();
} 

double Hand::get_bust_probability(Deck deck) {
    return get_score_probability(deck, 21);
} 

double Hand::get_score_probability(Deck deck, int score) {
    if (get_value() > score)
        return 1.0;
    int card_max_value = (score - get_value());

    return 1.0 - ((double)deck.num_cards_less_than(card_max_value) / deck.get_cards().size());
} 

std::string Hand::as_string() {
    std::string ret = "";
    for (auto card: _cards) {
        ret += card.as_string();
    }

    return ret;
} 
