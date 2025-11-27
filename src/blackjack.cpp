#include "blackjack.hpp"

BlackJack::BlackJack() {
    _deck = Deck();
    _game_state = GameState::PLACE_BETS;
}

BlackJack::GameState BlackJack::step(bool hit, double bet) {
    switch(_game_state) {
        case GameState::PLACE_BETS:
            _bet = bet;
            _game_state = GameState::PLAYER_TURN;
            deal_cards();
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
        case GameState::DEALER_TURN:
            do_dealer_move();

            if (did_dealer_bust())
                _game_state = GameState::DEALER_BUST;

            if (get_hand_value(_dealer_hand) > get_hand_value(_player_hand))
                _game_state = GameState::COUNT;

            break;
        case GameState::DEALER_BUST:
            _game_state = GameState::COUNT;
        case GameState::COUNT:
            _player_money += _bet * get_bet_multiplier();
            _game_state = GameState::RESET;
            break;
        case GameState::RESET:
            _deck = Deck();
            auto _ = _player_hand.empty();
            auto _ = _dealer_hand.empty();
            
            _game_state = GameState::PLACE_BETS;
            break;
        // TODO: Doubling down and splits.
    }

    return _game_state;
};

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

void BlackJack::do_dealer_move() {
    if (get_hand_value(_dealer_hand) <= 16)
        deal_card(_dealer_hand);
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
