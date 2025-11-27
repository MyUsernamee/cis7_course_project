#pragma once
#include <string>

///
/// A simple data class that represents a singular card.
///
struct Card {

    enum Suit {
        HIDDEN,
        DIAMONDS,
        CLUBS,
        HEARTS,
        SPADES,
    };

    enum Rank {
        ACE=0,
        TWO=1,
        THREE=2,
        FOUR=3,
        FIVE=4,
        SIX=5,
        SEVEN=6,
        EIGHT=7,
        NINE=8,
        TEN=9,
        JACK=10,
        QUEEN=11,
        KING=12,
    };

    Card(Suit suit, Rank rank);

    Suit _suit; /// The suit of the card.
    Rank _rank; /// The rank of the card.

    std::string as_string(); /// Returns a nices unicode representation of this card for printing.
    static char rank_as_char(Rank rank);
    static char suit_as_char(Suit rank);
};
