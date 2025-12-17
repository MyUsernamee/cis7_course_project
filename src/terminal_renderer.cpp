#include <iostream>
#include "terminal_renderer.hpp"

#define GAME_SEPERATOR_WIDTH 32 /// Width of the line seperating game states.

void render_terminal(BlackJack& game) {

    std::cout << "Current GameState:" <<
        game.get_state() << std::endl;

    std::cout << "Dealer hand: " << 
        game.get_dealer_hand().as_string() << std::endl;
    std::cout << "Value:" << 
        game.get_dealer_hand().get_value() << std::endl;

    std::cout << "Player hand: " << 
        game.get_player_hand().as_string() << std::endl;
    std::cout << "Value:" << 
        game.get_player_hand().get_value() << std::endl;

    std::cout << std::string(GAME_SEPERATOR_WIDTH, '-') << std::endl;
}

