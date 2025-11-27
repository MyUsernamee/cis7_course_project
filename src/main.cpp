#include <iostream>

#include "card.hpp"

int main () {
    std::cout << Card(Card::DIAMONDS, Card::KING).as_string() << std::endl;
    return 0;
}
