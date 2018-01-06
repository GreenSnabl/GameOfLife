/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "GameCurses.h"
#include "GameOfLife.h"
#include <curses.h>
#include <unistd.h>


GameCurses::GameCurses(int** arr, int height, int width) : speed{150000}, game(arr, height, width), mode{1}
{
}

GameCurses::GameCurses(const std::string &fname, int height, int width, int scaling = 1) : speed{150000}, game(fname, height, width, scaling), mode{1}
{
}

GameCurses::~GameCurses() {}


void GameCurses::printFrame() {
    
    std::string selected_mode = "Mode: \t" + get_mode();
    std::string selected_speed = "Speed:\t" + std::to_string(speed/ 1000) + "ms per frame";
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    for (int i = 0; i < game.get_height(); ++i)
        for (int j = 0; j < game.get_width(); ++j) {
            if (game.is_alive(i,j)) {
                attron(COLOR_PAIR(2));
                mvaddch(i + 2, j + 2, ' ');
                attron(COLOR_PAIR(2));
            } else {
                attron(COLOR_PAIR(1));
                mvaddch(i + 2, j + 2, ' ');
                attroff(COLOR_PAIR(1));
            }
        }
    attron(COLOR_PAIR(2));
    mvaddstr(game.get_height() / 2 - 4, game.get_width() +  2, "\"ENTER\"\tquit");
    mvaddstr(game.get_height() / 2 - 3, game.get_width() +  2, "\"UP\" or \"+\" \tspeed up");
    mvaddstr(game.get_height() / 2 - 2, game.get_width() +  2, "\"DOWN\" or \"-\"\tspeed down");
    mvaddstr(game.get_height() / 2 - 1, game.get_width() +  2, "\"1\",\"2\",\"3\"\tmode select");
    mvaddstr(game.get_height() / 2    , game.get_width() +  2, "\"1\"\t\tframe on");
    mvaddstr(game.get_height() / 2 + 1, game.get_width() +  2, "\"2\"\t\tframe off");
    mvaddstr(game.get_height() / 2 + 2, game.get_width() +  2, "\"3\"\t\tno borders");
    mvaddstr(game.get_height() / 2 + 3, game.get_width() +  2, selected_mode.c_str());
    mvaddstr(game.get_height() / 2 + 4, game.get_width() +  2, selected_speed.c_str());
    attroff(COLOR_PAIR(2));

}


void GameCurses::play() {

    int key = -1;
    const int ENTER = 10;
    const int PLUS = 43;
    const int MINUS = 45;
    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    start_color();

    while (key != ENTER) {
        clear();
        
        printFrame();
        refresh();
        usleep(speed);
        game.newGeneration();
        key = getch();
        if (key == KEY_UP || key == PLUS) {speed_up(); key = -1;}
        if (key == KEY_DOWN || key == MINUS) {speed_down(); key = -1;}
        if (key == KEY_LEFT) {cycle_mode_up(); key = -1;}
        if (key == KEY_RIGHT) {cycle_mode_down(); key = -1;}
        if (key == 49 || key == 50 || key == 51) {set_mode(key-48); key = -1;}
    }
    endwin();
}


void GameCurses::set_mode(int index) {
    if (index >= 1 && index <= 3) {
        mode = index; 
        game.set_mode(index);
    }    
}

void GameCurses::cycle_mode_up()
{
    if (mode = 3) mode = 1;
    else ++mode;

}
void GameCurses::cycle_mode_down()
{
    if (mode = 1) mode = 3;
    else --mode;
}

void GameCurses::speed_up() {
    if (speed > 10000) speed -= 10000;
}
void GameCurses::speed_down(){
    if (speed < 500000) speed += 20000;
}

const std::string GameCurses::get_mode() const
{
    switch (mode){
        case 1: return "frame on";
        case 2: return "frame off";
        case 3: return "no borders";
    }


}