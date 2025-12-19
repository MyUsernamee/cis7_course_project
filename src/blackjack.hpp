#pragma once

#include "deck.hpp"
#include "hand.hpp"

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
        PLAYER_TURN_DOUBLE_DOWN, ///< Player's turn, able to double down.
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
    
    ///
    /// Represents the results of a round. Either Tie, Win, or Loss.
    ///
    enum GameResults {
        TIE,
        WIN,
        LOSS
    };

    GameState step(bool hit); ///< Like the "main loop", steps the game
                                          ///< and returns the game state the new game state. For example if you were in the place bets state, and you placed a bet of $100 the next state returned would be `PLAYER_TURN` for the players turn.

    GameState step(); ///< Overload for the `step` function. Assumes default options in `step`. Best used when the player has no input, or input would have no effect.
    GameState bet(int bet); ///< Place bet and step game. Does nothing if game state is not `PLACE_BETS`
    GameState bet(); ///< Overload for `bet`, bets the stored bet amount.
    
    GameState double_down(); ///< Double the bet and take a card. 

    void set_money(int money);
    void set_bet(int bet); ///< Simmilar to `bet(double bet)`, but doesn't step the game.

    GameState get_state();

    Hand get_player_hand();
    Hand get_dealer_hand();

    Deck get_deck();

    int get_money();
    int get_bet();

    std::optional<BlackJack::GameResults> get_results();

    double get_player_wining_probability(bool hit);

private:
    Deck _deck;
    GameState _game_state;
    Hand _player_hand;
    Hand _dealer_hand;

    int _player_money;
    int _bet;

    void deal_cards();
    
    static bool should_dealer_hit(Hand dealer_hand);
    bool do_dealer_move();

    bool did_player_bust();
    bool did_dealer_bust();

    double get_bet_multiplier();
};

