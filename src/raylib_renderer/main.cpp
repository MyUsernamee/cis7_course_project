/// This is a super simple file that just renders the game with a raylib output for fun :)
/// The code is kind of a mess in this file, but it is there if you want to play with it.
#include <algorithm>
#include <cmath>
#include <raylib.h>
#include <map>
#include <vector>

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
#define GAME_UI_LINE_SPACING 2
#define GAME_UI_MARAGIN 30
#define GAME_COLORS_SATURATION 0.5
#define GAME_MONEY_COLOR ColorFromHSV(60, GAME_COLORS_SATURATION, 1.0) 
#define GAME_BET_COLOR ColorFromHSV(0.0, GAME_COLORS_SATURATION, 1.0) 
#define GAME_TABLE_COLOR ColorFromHSV(120.0, GAME_COLORS_SATURATION, 0.5)
#define GAME_BUST_MESSAGE_COLOR ColorFromHSV(0, GAME_COLORS_SATURATION, 1.0)
#define GAME_DEALER_MESSAGE_COLOR ColorFromHSV(240, GAME_COLORS_SATURATION, 1.0)

#define GAME_MINIMUM_BET 5

#define GAME_BUST_TEXT "BUST!"
#define GAME_DEALER_BUST_TEXT "DEALER BUST!"

#define GAME_TIE_TEXT "TIE!"
#define GAME_TIE_TEXT_COLOR ColorFromHSV(240.0, GAME_COLORS_SATURATION, 1.0)
#define GAME_WIN_TEXT "YOU WIN!"
#define GAME_WIN_TEXT_COLOR ColorFromHSV(120.0, GAME_COLORS_SATURATION, 1.0)
#define GAME_LOSS_TEXT "YOU LOOSE!"
#define GAME_LOSS_TEXT_COLOR ColorFromHSV(0.0, GAME_COLORS_SATURATION, 1.0)

#define GAME_BUTTON_WIDTH 120
#define GAME_BUTTON_HEIGHT 80
#define GAME_BUTTON_CENTER_X GetRenderWidth() - GAME_UI_MARAGIN - GAME_BUTTON_WIDTH / 2

void render_game();
void draw_center_aligned_text(std::string, int x, int y, Color color);

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

struct Button {
    int _x;
    int _y;
    int _w;
    int _h;
    std::string _text;
    Color _color;
    bool _active;

    static std::vector<Button> button_queue;

    static void render_buttons() {
        for (auto button : button_queue) {
            button.draw();
        }

        button_queue.clear();
    }

    static bool mouse_inside(int x, int y, int w, int h) {
        return GetMouseX() > x - w/2 && GetMouseX() < x + w/2 && GetMouseY() > y - h/2 && GetMouseY() < y + h/2;
    }

    static bool button(int x, int y, int w, int h, std::string text, Color color, bool active) {
        button_queue.emplace_back(Button(x, y, w, h, text, color, active)); 

        return mouse_inside(x, y, w, h) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && active;
    }

    void draw() {
        int width = MeasureText(_text.c_str(), GAME_UI_TEXT_SIZE);
        bool hover = mouse_inside(_x, _y, _w, _h);
        DrawRectangle(_x - _w / 2, _y - _h / 2, _w, _h, 
                ColorBrightness(_color, 
                    hover && IsMouseButtonDown(MOUSE_BUTTON_LEFT) || !_active ? -0.5f : hover ? 0.5 : 0.0));
        draw_center_aligned_text(_text, _x, _y, 
                ColorAlpha(WHITE, _active ? 1.0 : 0.1f));

    }
};

double CardGraphic::next_card_birth = 0.0;
std::vector<Button> Button::button_queue;
static std::map<Card, CardGraphic> card_graphics;

static BlackJack game = BlackJack();
static double last_step_time = 0.0;
static double step_delay = 0.0;
static BlackJack::GameState state;


int main() {
    
    InitWindow(1280, 720, "Black Jack");

    game.set_money(100.0);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(GAME_TABLE_COLOR);

        bool can_double_down = state == BlackJack::GameState::PLAYER_TURN_DOUBLE_DOWN;
        bool player_turn = state == BlackJack::GameState::PLAYER_TURN || can_double_down;

        bool hit_button = Button::button(
                GAME_BUTTON_CENTER_X,
                GetRenderHeight() / 2,
                GAME_BUTTON_WIDTH, GAME_BUTTON_HEIGHT, "Hit Me!", BLUE, player_turn);

        bool stand_button = Button::button(
                GAME_BUTTON_CENTER_X,
                GetRenderHeight() / 2 + GAME_BUTTON_HEIGHT + 4,
                GAME_BUTTON_WIDTH, GAME_BUTTON_HEIGHT, "Stand.", BLUE, player_turn);

        bool double_down_button = Button::button(
                GAME_BUTTON_CENTER_X,
                GetRenderHeight() / 2 - GAME_BUTTON_HEIGHT - 4,
                GAME_BUTTON_WIDTH, GAME_BUTTON_HEIGHT, "DOUBLE\nDOWN", PURPLE, can_double_down);

        switch (state) {
            case BlackJack::PLAYER_TURN_DOUBLE_DOWN: 
            case BlackJack::PLAYER_TURN: {
                step_delay = 0.0;
                last_step_time = GetTime();

                    if (double_down_button) {
                    state = game.double_down();
                    step_delay = GAME_STEP_TIME;
                }

                if (hit_button || stand_button) {
                    state = game.step(hit_button);
                    step_delay = GAME_STEP_TIME;
                }
                break;
            }
            case BlackJack::PLACE_BETS:
                step_delay = 0.0;
                last_step_time = GetTime();

                game.set_bet(std::clamp(game.get_bet() + int(GetMouseWheelMove()), GAME_MINIMUM_BET, game.get_money()));

                if (Button::button(GAME_UI_MARAGIN + GAME_BUTTON_WIDTH / 2, 
                            GetRenderHeight() - GAME_UI_TEXT_SIZE * 5 - GAME_UI_LINE_SPACING * 2
                            - GAME_BUTTON_HEIGHT / 2,
                            GAME_BUTTON_WIDTH, GAME_BUTTON_HEIGHT, "Place Bet.", ORANGE, true)) {
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

void draw_hand(int x, int y, Hand hand) {
    int card_x = -(hand.get_cards().size() - 1) * CARD_WIDTH / 2 + x;
    for (auto card : hand.get_cards()) {
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
        auto card = card_graphic.second;
        auto distance = std::sqrt(card._x * card._x + card._y * card._y);
        if (!card._alive) {
            if(distance > 1.0) {
                draw_card(0, 0, card._card);
                continue;
            }
            Card card = card_graphic.first; 
            keys_to_delete.insert(card);
        }
    }
    
    for (auto key: keys_to_delete) {
        card_graphics.erase(key);
    }
}

void draw_ui() {
    DrawText(TextFormat("$%d", game.get_money()), GAME_UI_MARAGIN, GetRenderHeight() - (GAME_UI_TEXT_SIZE + GAME_UI_MARAGIN), GAME_UI_TEXT_SIZE, GAME_MONEY_COLOR);
    DrawText(TextFormat("-$%d", game.get_bet()), GAME_UI_MARAGIN, GetRenderHeight() - (GAME_UI_TEXT_SIZE * 2.0 + GAME_UI_MARAGIN + GAME_UI_LINE_SPACING), GAME_UI_TEXT_SIZE, GAME_BET_COLOR);

    auto probability_text = TextFormat("Probability Of Bust: %d", int(game.get_player_hand().get_bust_probability(game.get_deck()) * 100.0));
    DrawText(probability_text, 
            GetRenderWidth() - MeasureText(probability_text, GAME_UI_TEXT_SIZE) - GAME_UI_TEXT_SIZE,
            GetRenderHeight() - (GAME_UI_TEXT_SIZE + GAME_UI_MARAGIN), GAME_UI_TEXT_SIZE, WHITE);
    probability_text = TextFormat("Probability Of Bust: %d", int(game.get_dealer_hand().get_bust_probability(game.get_deck()) * 100.0));
    DrawText(probability_text, 
            GetRenderWidth() - MeasureText(probability_text, GAME_UI_TEXT_SIZE) - GAME_UI_TEXT_SIZE,
            (GAME_UI_MARAGIN), GAME_UI_TEXT_SIZE, WHITE);

    auto win_probability_no_hit = game.get_player_wining_probability(false);
    auto win_probability_hit = game.get_player_wining_probability(true);

    auto win_probability_text_no_hit = TextFormat("Probability Of Wining: %d", int(win_probability_no_hit * 100.0));
    auto win_probability_text_hit = TextFormat("Probability Of Wining If Hit: %d", int(win_probability_hit * 100.0));
    DrawText(win_probability_text_no_hit, 
            GetRenderWidth() - MeasureText(win_probability_text_no_hit, GAME_UI_TEXT_SIZE) - GAME_UI_TEXT_SIZE,
            GetRenderHeight() - (GAME_UI_TEXT_SIZE + GAME_UI_MARAGIN * 2), GAME_UI_TEXT_SIZE, WHITE);
    DrawText(win_probability_text_hit, 
            GetRenderWidth() - MeasureText(win_probability_text_hit, GAME_UI_TEXT_SIZE) - GAME_UI_TEXT_SIZE,
            GetRenderHeight() - (GAME_UI_TEXT_SIZE + GAME_UI_MARAGIN * 3), GAME_UI_TEXT_SIZE, WHITE);


    if (!game.get_player_hand().empty() && !game.get_dealer_hand().empty()) {
        auto hand_score_text = TextFormat("%d", game.get_player_hand().get_value());
        DrawText(hand_score_text, GetRenderWidth() / 2 - MeasureText(hand_score_text, GAME_UI_TEXT_SIZE) / 2,
                GetRenderHeight() - CARD_HEIGHT * 3 / 2 - GAME_UI_MARAGIN * 2 - GAME_UI_TEXT_SIZE / 2, 
                GAME_UI_TEXT_SIZE, WHITE);
        hand_score_text = TextFormat("%d", game.get_dealer_hand().get_value());
        DrawText(hand_score_text, GetRenderWidth() / 2 - MeasureText(hand_score_text, GAME_UI_TEXT_SIZE) / 2,
                CARD_HEIGHT * 3 / 2 + GAME_UI_MARAGIN * 2 - GAME_UI_TEXT_SIZE / 2, 
                GAME_UI_TEXT_SIZE, WHITE);
    }

    Button::render_buttons();
   
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
        case BlackJack::COUNT: 
            {
                
                std::string end_text;
                Color end_text_color;
                switch (game.get_results().value()) {
                    case BlackJack::TIE:
                        end_text = GAME_TIE_TEXT;
                        end_text_color = GAME_TIE_TEXT_COLOR;
                        break;
                    case BlackJack::WIN:
                        end_text = GAME_WIN_TEXT;
                        end_text_color = GAME_WIN_TEXT_COLOR;
                        break;
                    case BlackJack::LOSS:
                        end_text = GAME_LOSS_TEXT;
                        end_text_color = GAME_LOSS_TEXT_COLOR;
                        break;
                }

                width = MeasureText(end_text.c_str(), GAME_UI_TEXT_SIZE);
                DrawText(end_text.c_str(), GetRenderWidth() / 2 - width / 2, GetRenderHeight() / 2 - GAME_UI_TEXT_SIZE / 2, GAME_UI_TEXT_SIZE, end_text_color);

                break;
            }
        default:
            break;
    }
}

void render_game() {
    queue_card_graphic_death();

    draw_hand(GetRenderWidth() / 2, CARD_HEIGHT, game.get_dealer_hand());
    draw_hand(GetRenderWidth() / 2, GetRenderHeight() - CARD_HEIGHT, game.get_player_hand());

    draw_ui();

    cleanup_card_graphics();
}

int num_lines(std::string str) { // I don't get why we can have over complicated things like
                             // std::cout and std::random, but we can't have a simple
                             // count / find function. Like bruh :(
    int count = 1;
    for (auto c : str)
        count += c == '\n';

    return count;

}
void draw_center_aligned_text(std::string text, int x, int y, Color color) {
    std::string text_buffer = text;
    int n_lines = num_lines(text);
    int line_height = (GAME_UI_TEXT_SIZE + GAME_UI_LINE_SPACING);
    int total_height = line_height * (n_lines - 1);
    int line_y = y - GAME_UI_TEXT_SIZE / 2 - total_height / 2;

    while (text_buffer.size() > 0)
    {

        int new_line_index = text_buffer.find('\n');
        new_line_index = std::string::npos == new_line_index ? text_buffer.size() : new_line_index;

        auto line = text_buffer.substr(0, new_line_index);
        text_buffer.erase(text_buffer.begin(), text_buffer.begin() + new_line_index + (n_lines > 1));

        int width = MeasureText(line.c_str(), GAME_UI_TEXT_SIZE);
        DrawText(line.c_str(), x - width / 2, line_y, GAME_UI_TEXT_SIZE, color);
        n_lines -= 1;
        line_y += line_height;

    }
}
