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

    std::set<Card> get_hand();
    std::set<Card> get_dealer_hand();

    int get_money();
    int get_bet();

    static double get_probability(Deck deck, std::set<Card> hand, unsigned int value); ///< Gets the probabililty that, given the current deck, and hand, that this hand will receive a card that causes the hand value to exceed the given value.
    static double get_probability_bust(Deck deck, std::set<Card> hand); ///< Gets the probabililty that, given the current deck, and hand, that this hand will receive a card that causes a bust.
    double get_probability_bust(std::set<Card> hand); ///< Gets the probabililty that, given the current deck, and hand, that this hand will receive a card that causes a bust.

    double get_probability_wining(); ///< Get the probability of the player wining given the current state of the game, deck, and dealer hand.

    static int get_hand_value(std::set<Card> hand);
    static int get_hand_value(std::set<Card> hand, bool soft_hand);

    std::optional<GameResults> get_results(); ///< If run during the COUNT state, returns if the player
                                              /// Won, Lost, or tied the dealer. Otherwise std::nullopt

    void render(); ///< \brief Renders the game to the terminal.
    
private:
    Deck _deck;
    GameState _game_state;
    std::set<Card> _player_hand;
    std::set<Card> _dealer_hand;

    int _player_money;
    int _bet;

    void deal_cards();
    void deal_card(std::set<Card>& hand);
    
    bool should_dealer_hit();
    bool do_dealer_move();

    bool did_player_bust();
    bool did_dealer_bust();

    double get_bet_multiplier();
};

