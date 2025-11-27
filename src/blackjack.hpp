#pragma once

#include "deck.hpp"

///
/// \brief This represents the actual Black Jack game.
/// This assumes a fair game, with a singular deck.
/// There is only one player and it stores internal state.
/// Internally uses a Finite State Machine to keep track of the game state.
/// All actions, and game states are managed by the `turn` function.
///
class BlackJack {

public:
    BlackJack();

    /// 
    /// Represents the current game state.
    ///
    enum GameState {
        PLACE_BETS, 
        PLAYER_TURN, ///< Player's turn. Only time the 
                     ///< `hit` parameter has effect in
                     ///< the `step` function.
        PLAYER_BUST, ///< Player busts. Essentially is
                     ///< just a NOOP, but can be used
                     ///< for presenting information to
                     ///< the player.
        DEALER_TURN, ///< Dealer takes turns. 
        DEALER_BUST, ///< Same as `PLAYER_BUST` but for dealer.
        COUNT, ///< We check the actual state of the game and 
               ///< Update player's bet.
        RESET, ///< Reset the game state. Is also a NOOP,
               ///< but can be used again for presenting
               ///< information to the player.
    };

    GameState step(bool hit, double bet); ///< Like the "main loop", steps the game
                                          ///< and returns the game state the new game state. For example if you were in the place bets state, and you placed a bet of $100 the next state returned would be `PLAYER_TURN` for the players turn.

    GameState step(); ///< Overload for the `step` function. Assumes default options in `step`. Best used when the player has no input, or input would have no effect.
    GameState bet(double bet); ///< Alias for `step` function. Best used when we are in the `PLACE_BETS` state, and the `hit` parameter has no effect.
    GameState hit(); ///< Helper alias for step.
    GameState stand(); ///< Helper alias for step.
    
    void render(); ///< \brief Renders the game to the terminal.
    
private:
    Deck _deck;
    GameState _game_state;
    double _bet;
    std::set<Card> _player_hand;
    std::set<Card> _dealer_hand;
    double _player_money;


    void deal_cards();
    void deal_card(std::set<Card>& hand);

    void do_dealer_move();

    bool did_player_bust();
    bool did_dealer_bust();

    int get_hand_value(std::set<Card> hand);
    int get_hand_value(std::set<Card> hand, bool soft_hand);

    double get_bet_multiplier();
};

