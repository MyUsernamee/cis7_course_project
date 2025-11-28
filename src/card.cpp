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
            break;
        case Suit::SPADES: 
            switch(_rank) {
                case Rank::ACE:
                    result = "\U0001F0A1";
                    break;
                case Rank::TWO:
                    result = "\U0001F0A2";
                    break;
                case Rank::THREE:
                    result = "\U0001F0A3";
                    break;
                case Rank::FOUR:
                    result = "\U0001F0A4";
                    break;
                case Rank::FIVE:
                    result = "\U0001F0A5";
                    break;
                case Rank::SIX:
                    result = "\U0001F0A6";
                    break;
                case Rank::SEVEN:
                    result = "\U0001F0A7";
                    break;
                case Rank::EIGHT:
                    result = "\U0001F0A8";
                    break;
                case Rank::NINE:
                    result = "\U0001F0A9";
                    break;
                case Rank::TEN:
                    result = "\U0001F0AA";
                    break;
                case Rank::JACK:
                    result = "\U0001F0AB";
                    break;
                case Rank::QUEEN:
                    result = "\U0001F0AD";
                    break;
                case Rank::KING:
                    result = "\U0001F0AE";
                    break;
            }
            break;
        case Suit::HEARTS:
            switch(_rank) {
                case Rank::ACE:
                    result = "\U0001F0B1";
                    break;
                case Rank::TWO:
                    result = "\U0001F0B2";
                    break;
                case Rank::THREE:
                    result = "\U0001F0B3";
                    break;
                case Rank::FOUR:
                    result = "\U0001F0B4";
                    break;
                case Rank::FIVE:
                    result = "\U0001F0B5";
                    break;
                case Rank::SIX:
                    result = "\U0001F0B6";
                    break;
                case Rank::SEVEN:
                    result = "\U0001F0B7";
                    break;
                case Rank::EIGHT:
                    result = "\U0001F0B8";
                    break;
                case Rank::NINE:
                    result = "\U0001F0B9";
                    break;
                case Rank::TEN:
                    result = "\U0001F0BA";
                    break;
                case Rank::JACK:
                    result = "\U0001F0BB";
                    break;
                case Rank::QUEEN:
                    result = "\U0001F0BD";
                    break;
                case Rank::KING:
                    result = "\U0001F0BE";
                    break;
            }
            break;
        case Suit::DIAMONDS: 
            switch(_rank) {
                case Rank::ACE:
                    result = "\U0001F0C1";
                    break;
                case Rank::TWO:
                    result = "\U0001F0C2";
                    break;
                case Rank::THREE:
                    result = "\U0001F0C3";
                    break;
                case Rank::FOUR:
                    result = "\U0001F0C4";
                    break;
                case Rank::FIVE:
                    result = "\U0001F0C5";
                    break;
                case Rank::SIX:
                    result = "\U0001F0C6";
                    break;
                case Rank::SEVEN:
                    result = "\U0001F0C7";
                    break;
                case Rank::EIGHT:
                    result = "\U0001F0C8";
                    break;
                case Rank::NINE:
                    result = "\U0001F0C9";
                    break;
                case Rank::TEN:
                    result = "\U0001F0CA";
                    break;
                case Rank::JACK:
                    result = "\U0001F0CB";
                    break;
                case Rank::QUEEN:
                    result = "\U0001F0CD";
                    break;
                case Rank::KING:
                    result = "\U0001F0CE";
                    break;
            }
            break;
        case Suit::CLUBS:
            switch(_rank) {
                case Rank::ACE:
                    result = "\U0001F0D1";
                    break;
                case Rank::TWO:
                    result = "\U0001F0D2";
                    break;
                case Rank::THREE:
                    result = "\U0001F0D3";
                    break;
                case Rank::FOUR:
                    result = "\U0001F0D4";
                    break;
                case Rank::FIVE:
                    result = "\U0001F0D5";
                    break;
                case Rank::SIX:
                    result = "\U0001F0D6";
                    break;
                case Rank::SEVEN:
                    result = "\U0001F0D7";
                    break;
                case Rank::EIGHT:
                    result = "\U0001F0D8";
                    break;
                case Rank::NINE:
                    result = "\U0001F0D9";
                    break;
                case Rank::TEN:
                    result = "\U0001F0DA";
                    break;
                case Rank::JACK:
                    result = "\U0001F0DB";
                    break;
                case Rank::QUEEN:
                    result = "\U0001F0DD";
                    break;
                case Rank::KING:
                    result = "\U0001F0DE";
                    break;
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
        case Rank::TEN:
            return '0';
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
        case '0':
            return Rank::TEN;
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

std::string Card::as_text() {
    return std::string({rank_as_char(_rank), suit_as_char(_suit)});
}
