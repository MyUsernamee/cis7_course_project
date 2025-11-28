#include "blackjack.hpp"
#include <iostream>

BlackJack::BlackJack() {
    _deck = Deck();
    _game_state = GameState::PLACE_BETS;
}

BlackJack::GameState BlackJack::step(bool hit) {
    switch(_game_state) {
        case GameState::PLACE_BETS:
            _game_state = GameState::PLAYER_TURN_DOUBLE_DOWN;
            deal_cards();
            break;
        case GameState::PLAYER_TURN_DOUBLE_DOWN:
            _game_state = GameState::PLAYER_TURN;
        case GameState::PLAYER_TURN:
            if (!hit) {
                _game_state = GameState::DEALER_TURN;
                break;
            }

            deal_card(_player_hand);

            if (did_player_bust())
                _game_state = GameState::PLAYER_BUST;

            break;
        case GameState::PLAYER_BUST:
            _game_state = GameState::DEALER_TURN;
            break;
        case GameState::DEALER_TURN: 
            {
                bool dealer_hit = do_dealer_move();

                if (did_dealer_bust()) {
                    _game_state = GameState::DEALER_BUST;
                    break;
                }

                if (get_hand_value(_dealer_hand) > get_hand_value(_player_hand) || !dealer_hit) {
                    _game_state = GameState::COUNT;
                    break;
                }

                break;
            }
        case GameState::DEALER_BUST:
            _game_state = GameState::COUNT;
            break;
        case GameState::COUNT:
            _player_money += _bet * get_bet_multiplier();
            _game_state = GameState::RESET;
            break;
        case GameState::RESET:
            _deck = Deck();
            _player_hand.clear();
            _dealer_hand.clear();
            
            _game_state = GameState::PLACE_BETS;
            break;
        // TODO: Doubling down and splits.
    }

    return _game_state;
};

BlackJack::GameState BlackJack::step() {
    return step(false);
}

BlackJack::GameState BlackJack::bet(int bet) {
    if (_game_state != GameState::PLACE_BETS)
        return _game_state;

    _bet = bet;
    _player_money -= bet;

    return step(false);
}

BlackJack::GameState BlackJack::bet() {
    if (_game_state != GameState::PLACE_BETS)
        return _game_state;

    _player_money -= _bet;
    return step(false);
}


void BlackJack::deal_cards() {

    // First we give the dealer their hand.
    _dealer_hand.insert(_deck.take_random_card());
    _dealer_hand.insert(_deck.take_random_card());

    _player_hand.insert(_deck.take_random_card());
    _player_hand.insert(_deck.take_random_card());

}

void BlackJack::deal_card(std::set<Card>& hand) {
    hand.insert(_deck.take_random_card());
}

bool BlackJack::did_player_bust() {
    return get_hand_value(_player_hand, true) > 21;
}
bool BlackJack::did_dealer_bust() {
    return get_hand_value(_dealer_hand) > 21;
}

bool BlackJack::do_dealer_move() {
    if (get_hand_value(_dealer_hand) <= 16) {
        deal_card(_dealer_hand);
        return true;
    }
    return false;
}

int BlackJack::get_hand_value(std::set<Card> hand, bool soft_hand) {
    int value = 0;

    for (auto card : hand) {
        if (card._rank == 0) {
            value += soft_hand ? 1 : 11;
            continue;
        }
        else if(card._rank >= 10) {
            value += 10;
            continue;
        }

        value += card._rank + 1;
    }

    return value;

}

int BlackJack::get_hand_value(std::set<Card> hand) {
    int hard_value = get_hand_value(hand, false);
    if (hard_value <= 21)
        return hard_value;

    return get_hand_value(hand, true); // TODO: Support multiple aces in 1 hand.
}

double BlackJack::get_bet_multiplier() {

    if (did_player_bust())
        return 0;
    if (did_dealer_bust() || get_hand_value(_dealer_hand) < get_hand_value(_player_hand))
        return get_hand_value(_player_hand) == 21 ? 2.5 : 1.5;
    if (get_hand_value(_dealer_hand) == get_hand_value(_player_hand))
        return 1;

    return 0; // Loose
}

void BlackJack::render() {
    std::cout << "Dealer hand: ";
    for (auto card : _dealer_hand)
        std::cout << card.as_string();

    std::cout << get_hand_value(_dealer_hand) << std::endl;

    std::cout << "Player hand: ";
    for (auto card : _player_hand)
        std::cout << card.as_string();

    std::cout << get_hand_value(_player_hand) << std::endl;

    std::cout << "GameState: " << _game_state << std::endl;

}

std::set<Card> BlackJack::get_hand() {
    return _player_hand;
}
std::set<Card> BlackJack::get_dealer_hand() {
    return _dealer_hand;
}

int BlackJack::get_money() {
    return _player_money;
}
void BlackJack::set_money(int money) {
    _player_money = money;
}
int BlackJack::get_bet() {
    return _bet;
}
void BlackJack::set_bet(int bet) {
    if (_game_state != GameState::PLACE_BETS)
        return;
    
    _bet = bet;
}

BlackJack::GameState BlackJack::get_state() {
    return _game_state;
}

double BlackJack::get_probabililty_bust(Deck deck, std::set<Card> hand) {
    int total_bad_hands = 0;

    for (auto card : deck.get_cards()) {
        std::set<Card> possible_hand = hand;
        possible_hand.insert(card);

        if (get_hand_value(possible_hand) > 21) 
            total_bad_hands += 1;
    }

    return double(total_bad_hands) / deck.get_cards().size();
}
double BlackJack::get_probabililty_bust(std::set<Card> hand) {
    return get_probabililty_bust(_deck, hand);
}

BlackJack::GameState BlackJack::double_down() {
    if (_game_state != GameState::PLAYER_TURN_DOUBLE_DOWN)
        return _game_state;

    _player_money -= _bet;
    _bet *= 2;

    deal_card(_player_hand);

    if (did_player_bust())
        _game_state = GameState::PLAYER_BUST;
    else
        _game_state = GameState::DEALER_TURN;

    return _game_state;
}

std::optional<BlackJack::GameResults> BlackJack::get_results() {
    if (_game_state != GameState::COUNT)
        return std::nullopt;

    auto player_hand_value = get_hand_value(_player_hand);
    auto dealer_hand_value = get_hand_value(_dealer_hand);

    if (player_hand_value == dealer_hand_value || (did_player_bust() && did_dealer_bust()))
        return GameResults::TIE;

    if (player_hand_value > dealer_hand_value && !did_player_bust() || did_dealer_bust())
        return GameResults::WIN;
    
    return GameResults::LOSS;
}
