#include "card.hpp"

Card::Card(Card::Suit suit, Card::Rank rank) :  _suit(suit), _rank(rank) { }
Card::Card(std::string string_repr) {
    if (string_repr == "#") {
        _suit = Suit::HIDDEN;
        _rank = Rank::ACE;
        return;
    }

    _rank = rank_from_char(string_repr[0]).value();
    _suit = suit_from_char(string_repr[1]).value();
}

std::string Card::as_string() {

    // WARNING: VERY UGLY CODE AHEAD.
    // Appearantly there is no clean way to convert 
    // wide unicode characters to char arrays in c++.
    // The goto method "std::wstring_convert" is Appearantly
    // deprecated, and has many issues, so this is
    // probably the cleanest method I can do.
    std::string result;
    switch (_suit) {
        case Suit::HIDDEN:
            result = "\U0001F0A0";
        case Suit::SPADES: 
            switch(_rank) {
                case Rank::ACE:
                    result = "\U0001F0A1";
                case Rank::TWO:
                    result = "\U0001F0A2";
                case Rank::THREE:
                    result = "\U0001F0A3";
                case Rank::FOUR:
                    result = "\U0001F0A4";
                case Rank::FIVE:
                    result = "\U0001F0A5";
                case Rank::SIX:
                    result = "\U0001F0A6";
                case Rank::SEVEN:
                    result = "\U0001F0A7";
                case Rank::EIGHT:
                    result = "\U0001F0A8";
                case Rank::NINE:
                    result = "\U0001F0A9";
                case Rank::TEN:
                    result = "\U0001F0AA";
                case Rank::JACK:
                    result = "\U0001F0AB";
                case Rank::QUEEN:
                    result = "\U0001F0AD";
                case Rank::KING:
                    result = "\U0001F0AE";
            }
            break;

        case Suit::HEARTS:
            switch(_rank) {
                case Rank::ACE:
                    result = "\U0001F0B1";
                case Rank::TWO:
                    result = "\U0001F0B2";
                case Rank::THREE:
                    result = "\U0001F0B3";
                case Rank::FOUR:
                    result = "\U0001F0B4";
                case Rank::FIVE:
                    result = "\U0001F0B5";
                case Rank::SIX:
                    result = "\U0001F0B6";
                case Rank::SEVEN:
                    result = "\U0001F0B7";
                case Rank::EIGHT:
                    result = "\U0001F0B8";
                case Rank::NINE:
                    result = "\U0001F0B9";
                case Rank::TEN:
                    result = "\U0001F0BA";
                case Rank::JACK:
                    result = "\U0001F0BB";
                case Rank::QUEEN:
                    result = "\U0001F0BD";
                case Rank::KING:
                    result = "\U0001F0BE";
            }
            break;

        case Suit::DIAMONDS: 
            switch(_rank) {
                case Rank::ACE:
                    result = "\U0001F0C1";
                case Rank::TWO:
                    result = "\U0001F0C2";
                case Rank::THREE:
                    result = "\U0001F0C3";
                case Rank::FOUR:
                    result = "\U0001F0C4";
                case Rank::FIVE:
                    result = "\U0001F0C5";
                case Rank::SIX:
                    result = "\U0001F0C6";
                case Rank::SEVEN:
                    result = "\U0001F0C7";
                case Rank::EIGHT:
                    result = "\U0001F0C8";
                case Rank::NINE:
                    result = "\U0001F0C9";
                case Rank::TEN:
                    result = "\U0001F0CA";
                case Rank::JACK:
                    result = "\U0001F0CB";
                case Rank::QUEEN:
                    result = "\U0001F0CD";
                case Rank::KING:
                    result = "\U0001F0CE";
            }
            break;

        case Suit::CLUBS:
            switch(_rank) {
                case Rank::ACE:
                    result = "\U0001F0D1";
                case Rank::TWO:
                    result = "\U0001F0D2";
                case Rank::THREE:
                    result = "\U0001F0D3";
                case Rank::FOUR:
                    result = "\U0001F0D4";
                case Rank::FIVE:
                    result = "\U0001F0D5";
                case Rank::SIX:
                    result = "\U0001F0D6";
                case Rank::SEVEN:
                    result = "\U0001F0D7";
                case Rank::EIGHT:
                    result = "\U0001F0D8";
                case Rank::NINE:
                    result = "\U0001F0D9";
                case Rank::TEN:
                    result = "\U0001F0DA";
                case Rank::JACK:
                    result = "\U0001F0DB";
                case Rank::QUEEN:
                    result = "\U0001F0DD";
                case Rank::KING:
                    result = "\U0001F0DE";
            }
            break;

    }      

    return result;
}

char Card::suit_as_char(Suit rank) {
    switch (rank) {
        case Suit::DIAMONDS:
            return 'D';
        case Suit::CLUBS:
            return 'C';
        case Suit::HEARTS:
            return 'H';
        case Suit::SPADES:
            return 'S';
        default:
            return '#'; // Hidden
    }

    return '\0'; // Should be unreachable
}

char Card::rank_as_char(Rank rank) {
    switch (rank) {
        case Rank::JACK:
            return 'J';
        case Rank::QUEEN:
            return 'Q';
        case Rank::KING:
            return 'K';
        case Rank::ACE:
            return 'A';
        default:
            return char(rank) + '1';
    }
    
    return '\0'; // Should be unreachable
}

std::optional<Card::Rank> Card::rank_from_char(char crank) {
    switch (crank) {
        case 'J':
            return Rank::JACK;
        case 'Q':
            return Rank::QUEEN;
        case 'K':
            return Rank::KING;
        case 'A':
            return Rank::ACE;
        default:
            if (crank <= '1')
                return std::nullopt;

            int index = int(crank - '1');
            if (index >= 13)
                return std::nullopt;
            
            return Card::Rank(int(index));
    }

    return std::nullopt;
}

std::optional<Card::Suit> Card::suit_from_char(char csuit) {
    switch (csuit) {
        case 'D':
            return Card::DIAMONDS;
        case 'C':
            return Card::CLUBS;
        case 'S':
            return Card::SPADES;
        case 'H':
            return Card::HEARTS;
        case '#':
            return Card::HIDDEN;
    }

    return std::nullopt;
}
