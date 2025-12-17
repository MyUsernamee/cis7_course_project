#include <iostream>

#include "blackjack.hpp"
#include "terminal_renderer.hpp"

int main () {

    for (auto card: Deck().get_cards())
        std::cout << card.as_string();

    std::cout << std::endl;

    BlackJack game = BlackJack();
    
    for (int i = 0; i <= 100; i++) {
        game.step(false);
        render_terminal(game);
    }
    
    return 0;
}
