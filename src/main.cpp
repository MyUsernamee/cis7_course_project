#include <iostream>

#include "blackjack.hpp"

int main () {

    for (auto card: Deck().get_cards())
        std::cout << card.as_string();

    std::cout << std::endl;

    BlackJack game = BlackJack();
    
    for (int i = 0; i <= 100; i++) {
        game.step(false, 100.0);
        game.render();
    }
    
    return 0;
}
