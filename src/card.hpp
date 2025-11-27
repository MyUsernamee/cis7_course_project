#pragma once
#include <string>
#include <optional>

///
/// \brief A simple data class that represents a singular card.
///
struct Card {

    enum Suit {
        HIDDEN=-1,
        DIAMONDS=0,
        CLUBS=1,
        HEARTS=2,
        SPADES=3,
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
    Card(std::string string_repr); ///< This is can throw an exception.

    const bool operator< (Card const& b) const { 
        return int(_rank) + int(_suit) * 13 < int(b._rank) + int(b._suit) * 13;
    }

    Suit _suit; ///< The suit of the card.
    Rank _rank; ///< The rank of the card.

    std::string as_string(); ///< Returns a nices unicode representation of this card for printing.
    static char rank_as_char(Rank rank);
    static char suit_as_char(Suit suit);
    static std::optional<Rank> rank_from_char(char crank);
    static std::optional<Suit> suit_from_char(char csuit);

};
