#pragma once
#include <string>

///
/// A simple data class that represents a singular card.
///
struct Card {

    enum Suit {
        DIAMONDS,
        CLUBS,
        HEARTS,
        SPADES
    };

    enum Rank {
        TWO=2,
        THREE=3,
        FOUR=4,
        FIVE=5,
        SIX=6,
        SEVEN=7,
        EIGHT=8,
        NINE=9,
        TEN=10,
        JACK=11,
        QUEEN=12,
        KING=13,
        ACE=14
    };

    Suit _suit; /// The suit of the card.
    Rank _rank; /// The rank of the card.

    std::string as_string(); /// Returns a nices unicode representation of this card for printing.
};
