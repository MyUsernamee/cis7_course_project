/// This is a super simple file that just renders the game with a raylib output for fun :)
#include <algorithm>
#include <raylib.h>
#include <map>

#include "blackjack.hpp"
#include "card.hpp"

#define CARD_WIDTH 60
#define CARD_HEIGHT CARD_WIDTH * (16.0 / 9.0) 
#define CARD_SPRING_CONSTANT 1000.0
#define CARD_DAMPING_FACTOR 40.0
#define CARD_DEAL_DELAY 0.1 
#define CARD_TEXT_SIZE 16

#define GAME_STEP_TIME 2.0
#define GAME_DEALER_TURN_DELAY 0.5
#define GAME_UI_TEXT_SIZE 20
#define GAME_COLORS_SATURATION 0.5
#define GAME_MONEY_COLOR ColorFromHSV(60, GAME_COLORS_SATURATION, 1.0) 
#define GAME_BET_COLOR ColorFromHSV(0.0, GAME_COLORS_SATURATION, 1.0) 
#define GAME_TABLE_COLOR ColorFromHSV(120.0, GAME_COLORS_SATURATION, 0.5)
#define GAME_BUST_MESSAGE_COLOR ColorFromHSV(0, GAME_COLORS_SATURATION, 1.0)
#define GAME_DEALER_MESSAGE_COLOR ColorFromHSV(240, GAME_COLORS_SATURATION, 1.0)

#define GAME_MINIMUM_BET 5.0

#define GAME_BUST_TEXT "BUST!"
#define GAME_DEALER_BUST_TEXT "DEALER BUST!"

Color get_suit_color(Card::Suit suit) {
    switch(suit) {
        case Card::DIAMONDS:
            return ORANGE;
        case Card::HEARTS:
            return RED;    
        case Card::CLUBS:
            return BLUE;  
        case Card::SPADES:
            return BLACK;  
        case Card::HIDDEN:
            return GRAY;
    }
}

struct CardGraphic {
    double _x;
    double _y;
    double _vx;
    double _vy;
    Card _card;
    bool _alive; // We will always try to set this to false at the begining of every frame.
                // At the end if it is still false, we delete it.
    double _birth_timer;

    static double next_card_birth;

    CardGraphic(Card card): _card(card) {
        _x = 0.0;
        _y = 0.0;
        _vx = 0.0;
        _vx = 0.0;

        _alive = true;
        next_card_birth = std::max(next_card_birth, GetTime()) + CARD_DEAL_DELAY;
        _birth_timer = next_card_birth;
    }

    void draw() {
        double w = CARD_WIDTH;
        double h = CARD_HEIGHT;
        DrawRectangle(_x - w / 2, _y - h / 2, w, h, WHITE);
        Color suit_color = get_suit_color(_card._suit);

        int text_width = MeasureText(_card.as_text().c_str(), CARD_TEXT_SIZE);
        DrawText(_card.as_text().c_str(), _x - text_width / 2, _y - CARD_TEXT_SIZE / 2, CARD_TEXT_SIZE, suit_color);
    }

    void queue_death() {
        _alive = false;
    }

    void update(int x, int y) {
        _alive = true;

        if(GetTime() - _birth_timer < 0.0)
            return;

        draw();
        
        _vx += (double(x - _x) * CARD_SPRING_CONSTANT - _vx * CARD_DAMPING_FACTOR) * GetFrameTime();
        _vy += (double(y - _y) * CARD_SPRING_CONSTANT - _vy * CARD_DAMPING_FACTOR) * GetFrameTime();
        _x += int(_vx) * GetFrameTime();
        _y += int(_vy) * GetFrameTime();
    }
};

double CardGraphic::next_card_birth = 0.0;
static std::map<Card, CardGraphic> card_graphics;

static BlackJack game = BlackJack();
static double last_step_time = 0.0;
static double step_delay = 0.0;
static BlackJack::GameState state;

void render_game();

int main() {
    
    InitWindow(1280, 720, "Black Jack");

    game.set_money(100.0);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GAME_TABLE_COLOR);

        switch (state) {
            case BlackJack::PLAYER_TURN:
                step_delay = 0.0;
                last_step_time = GetTime();
                if (IsKeyPressed(KEY_H) || IsKeyPressed(KEY_S)) {
                    state = game.step(IsKeyPressed(KEY_H));
                    step_delay = GAME_STEP_TIME;
                }
                break;
            case BlackJack::PLACE_BETS:
                step_delay = 0.0;
                last_step_time = GetTime();

                game.set_bet(std::clamp(game.get_bet() + GetMouseWheelMove(), GAME_MINIMUM_BET, game.get_money()));

                if (IsKeyPressed(KEY_SPACE)) {
                    state = game.bet();
                }
                break;

            case BlackJack::DEALER_TURN:
                step_delay = GAME_DEALER_TURN_DELAY;
            default:
                if ((last_step_time + step_delay - GetTime() < 0.0 && step_delay > 0.0) || IsKeyPressed(KEY_SPACE)) {
                    state = game.step(false);
                    last_step_time = GetTime();
                    step_delay = GAME_STEP_TIME;
                }
                break;
        }

        render_game();
        EndDrawing();
    }
    
    CloseWindow();

}


void draw_card(int x, int y, Card card) {
    if (card_graphics.contains(card)) {
        card_graphics.at(card).update(x, y);
        return;
    }
    
    card_graphics.insert(std::pair<Card, CardGraphic>(card, CardGraphic(card)));
}


void draw_hand(int x, int y, std::set<Card> hand) {
    int card_x = -(hand.size() - 1) * CARD_WIDTH / 2 + x;
    for (auto card : hand) {
        draw_card(card_x, y, card);
        card_x += CARD_WIDTH + 2;
    }
}

void queue_card_graphic_death() {
    for (auto& card_graphic: card_graphics) 
        card_graphic.second.queue_death();
}

void cleanup_card_graphics() {
    std::set<Card> keys_to_delete;
    for (auto card_graphic: card_graphics) {
       if (!card_graphic.second._alive) {
           Card card = card_graphic.first; 
           keys_to_delete.insert(card);
           TraceLog(LOG_ALL, "Deleted Card");
        }
    }
    
    for (auto key: keys_to_delete) {
        card_graphics.erase(key);
    }
}

void draw_ui() {
    DrawText(TextFormat("$%f", game.get_money()), (GAME_UI_TEXT_SIZE + 10), GetRenderHeight() - (GAME_UI_TEXT_SIZE + 10), GAME_UI_TEXT_SIZE, GAME_MONEY_COLOR);
    DrawText(TextFormat("-$%f", game.get_bet()), (GAME_UI_TEXT_SIZE + 10), GetRenderHeight() - (GAME_UI_TEXT_SIZE + 10) * 2.0, GAME_UI_TEXT_SIZE, GAME_BET_COLOR);
    
    int width;

    switch(game.get_state()) {
        case BlackJack::PLAYER_BUST:
            width = MeasureText(GAME_BUST_TEXT, GAME_UI_TEXT_SIZE);
            DrawText(GAME_BUST_TEXT, GetRenderWidth() / 2 - width / 2, GetRenderHeight() / 2 - GAME_UI_TEXT_SIZE / 2, GAME_UI_TEXT_SIZE, GAME_BUST_MESSAGE_COLOR);
            break;
        case BlackJack::DEALER_BUST:
            width = MeasureText(GAME_DEALER_BUST_TEXT, GAME_UI_TEXT_SIZE);
            DrawText(GAME_DEALER_BUST_TEXT, GetRenderWidth() / 2 - width / 2, GetRenderHeight() / 2 - GAME_UI_TEXT_SIZE / 2, GAME_UI_TEXT_SIZE, GAME_DEALER_MESSAGE_COLOR);
            break;
        default:
            break;
    }
}

void render_game() {
    queue_card_graphic_death();

    draw_ui();

    draw_hand(GetRenderWidth() / 2, CARD_HEIGHT, game.get_dealer_hand());
    draw_hand(GetRenderWidth() / 2, GetRenderHeight() - CARD_HEIGHT, game.get_hand());

    cleanup_card_graphics();
}

