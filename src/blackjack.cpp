#include "blackjack.hpp"
#include <iostream>
#include <cassert>

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

            _player_hand.deal_card(_deck);

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

                if (_dealer_hand.get_value() > _player_hand.get_value() || !dealer_hit) {
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
    _dealer_hand.deal_cards(_deck, 2);
    _player_hand.deal_cards(_deck, 2);

}

bool BlackJack::did_player_bust() {
    return _player_hand.get_value() > 21;
}
bool BlackJack::did_dealer_bust() {
    return _dealer_hand.get_value() > 21;
}

bool BlackJack::should_dealer_hit(Hand dealer_hand) {
    return dealer_hand.get_value() <= 16;
}

bool BlackJack::do_dealer_move() {
    if (_dealer_hand.get_value() <= 16) {
        _dealer_hand.deal_card(_deck);
        return true;
    }
    return false;
}

double BlackJack::get_bet_multiplier() {

    if (did_player_bust())
        return 0;
    if (did_dealer_bust() || _dealer_hand.get_value() < _player_hand.get_value())
        return _player_hand.get_value() == 21 ? 2.5 : 1.5;
    if (_dealer_hand.get_value() == _player_hand.get_value())
        return 1;

    return 0; // Loose
}

Hand BlackJack::get_player_hand() {
    return _player_hand;
}
Hand BlackJack::get_dealer_hand() {
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

BlackJack::GameState BlackJack::double_down() {
    if (_game_state != GameState::PLAYER_TURN_DOUBLE_DOWN)
        return _game_state;

    _player_money -= _bet;
    _bet *= 2;

    _player_hand.deal_card(_deck);

    if (did_player_bust())
        _game_state = GameState::PLAYER_BUST;
    else
        _game_state = GameState::DEALER_TURN;

    return _game_state;
}

std::optional<BlackJack::GameResults> BlackJack::get_results() {
    if (_game_state != GameState::COUNT)
        return std::nullopt;

    auto player_hand_value = _player_hand.get_value();
    auto dealer_hand_value = _dealer_hand.get_value();

    if (player_hand_value == dealer_hand_value || (did_player_bust() && did_dealer_bust()))
        return GameResults::TIE;

    if (player_hand_value > dealer_hand_value && !did_player_bust() || did_dealer_bust())
        return GameResults::WIN;
    
    return GameResults::LOSS;
}

Deck BlackJack::get_deck() {
    return _deck;
}

double BlackJack::get_player_wining_probability(bool hit, Hand player_hand, Hand dealer_hand, Deck deck) {


}

double BlackJack::get_player_wining_probability(bool hit) {
    if (_game_state == PLACE_BETS || _game_state == RESET)
        return 1.0; // Game hasn't started
    double probability = 1.0;

    if (_player_hand.get_value() > 21)
        return 0.0; // Tie or loss.
    if (_dealer_hand.get_value() > 21)
        return probability; // We didn't bust but the dealer did, we won.

    if (hit) {
        // Probability of getting higher then dealer, but not busting.
        double probability_dealt_higher = _player_hand.get_score_probability(_deck, _dealer_hand.get_value());
        double bust_probability = _player_hand.get_bust_probability(_deck);
        probability *= (probability_dealt_higher - bust_probability);

    }
    else {
        probability *= _dealer_hand.get_value() >= _player_hand.get_value() ? 0.0 : 1.0;
    }
   
    if (should_dealer_hit(_dealer_hand)){
        probability *= 1.0 - (_dealer_hand.get_score_probability(_deck, _player_hand.get_value()) - _dealer_hand.get_bust_probability(_deck)) ;
    }

    return probability;                   
}
