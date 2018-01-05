/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GameOfLife.cpp
 * Author: dennis
 * 
 * Created on December 24, 2017, 4:34 PM
 */

#include "GameOfLife.h"
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <curses.h>
#include "Bitmap.h"

using std::cout;
using std::left;
using std::setw;



GameOfLife::GameOfLife(int** arr, int width, int height) : m_width{width}, m_height{height}, edges{false}, torus{false}
{
    m_data = new int*[m_height];
    m_data_new = new int*[m_height];
    for (int i = 0; i < m_height; ++i) {
        m_data[i] = new int[m_width];
        m_data_new[i] = new int[m_width];
    }
    for (int i = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j)
            m_data[i][j] = m_data_new[i][j] = arr[i][j];
}

GameOfLife::GameOfLife(const std::string& fname, int height, int width, int img_scaling) : m_width{width}, m_height{height}, edges{false}, torus{false}
{
    GreyImage img(m_width, m_height);
    img.load(fname);

    m_data = new int*[m_height];
    m_data_new = new int*[m_height];
    for (int i = 0; i < m_height; ++i) {
        m_data[i] = new int[m_width];
        m_data_new[i] = new int[m_width];
    }
    for (int i = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j)
            if (static_cast<int> (img.getPixel(j * img_scaling, i * img_scaling).value) < 128) m_data[i][j] = m_data_new[i][j] = 1;
            else m_data[i][j] = m_data_new[i][j] = 0;
}

GameOfLife::~GameOfLife() {
    for (int i = 0; i < m_height; ++i) {
        delete[] m_data[i];
        delete[] m_data_new[i];
    }
    delete[] m_data;
    delete[] m_data_new;
}

bool GameOfLife::checkNeighbors(int height, int width, bool populated) {
    int cnt = 0;
    if (populated) --cnt;

    if (torus) {
        for (int i = height - 1; i <= height + 1; ++i)
            for (int j = width - 1; j <= width + 1; ++j)
                if (m_data[torus_border(i, m_height)][torus_border(j, m_width)] == 1) ++cnt;
    } else {
        for (int i = low_border(height); i <= high_border(height, m_height - 1); ++i) {
            for (int j = low_border(width); j <= high_border(width, m_width - 1); ++j)
                if (m_data[i][j] == 1) ++cnt;
        }
    }
    /*for (int i = low_border(height); i <= high_border(height, m_height); ++i) {
        for (int j = low_border(width); j <= high_border(width, m_width); ++j)
            if (m_data[i][j] == 1) ++cnt;
    }*/

    if (populated && (cnt == 2 || cnt == 3)) return true;
    if (!populated && cnt == 3) return true;
    return false;
}

void GameOfLife::newGeneration() {


    if (!edges) {
        for (int i = 1; i < m_height - 1; ++i)
            for (int j = 1; j < m_width - 1; ++j) {
                if (checkNeighbors(i, j, (m_data[i][j] == 1))) m_data_new[i][j] = 1;
                else m_data_new[i][j] = 0;
            }
    } else {
        for (int i = 0; i < m_height; ++i)
            for (int j = 0; j < m_width; ++j)
                if (checkNeighbors(i, j, (m_data[i][j] == 1))) m_data_new[i][j] = 1;
                else m_data_new[i][j] = 0;
    }
    for (int i = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j)
            m_data[i][j] = m_data_new[i][j];
}

void GameOfLife::showWorldConsole() {
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            if (m_data[i][j] == 1) cout << setw(2) << left << "o";
            else cout << setw(2) << left << ".";
        }
        cout << "\n";
    }
    cout << "\n";
}

void GameOfLife::simulateConsole() {

    showWorldConsole();
    int i = 0;
    while (i < 140) {
        usleep(150000);
        newGeneration();
        showWorldConsole();
        ++i;
    }
}

void GameOfLife::showWorldNCurses() {

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    for (int i = 0; i < m_height; ++i)
        for (int j = 0; j < m_width; ++j) {
            if (m_data[i][j] == 0) {
                attron(COLOR_PAIR(1));
                mvaddch(i + 2, j + 2, ' ');
                attron(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                mvaddch(i + 2, j + 2, ' ');
                attroff(COLOR_PAIR(2));
            }
        }
    attron(COLOR_PAIR(2));
    mvaddstr(m_height + 2, 2, "Press ENTER to quit");
    attroff(COLOR_PAIR(2));

}

void GameOfLife::simulateCurses() {

    int key = 0;
    const int ENTER = 10;
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    start_color();
    showWorldNCurses();

    while (key != ENTER) {
        usleep(150000);
        newGeneration();
        showWorldNCurses();
        refresh();
        timeout(0);
        key = getch();
    }

    endwin();
}

void GameOfLife::set_mode(int index) {
    switch (index) {
        case 0:{ edges = false; torus = false; break;}
        case 1: {edges = true; torus = false; break; }
        case 2: {edges = true; torus = true; break;}
    }
}

int low_border(int index) {
    if (index == 0) return 0;
    return index - 1;
}

int high_border(int index, int max) {
    if (index == max) return max;
    return index + 1;
};

int torus_border(int index, int max) {
    if (index == -1) return max - 1;
    else if (index == max) return 0;
    return index;
};

void loesung() {
    int arr[32][32] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
        {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0},
        {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    int** arr2 = new int*[32];
    for (int i = 0; i < 32; ++i)
        arr2[i] = new int[32];

    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            arr2[i][j] = arr[i][j];

    GameOfLife g(arr2, 32, 32);

    g.simulateCurses();

    GameOfLife g2(arr2, 32, 32);
    g2.simulateConsole();

    for (int i = 0; i < 32; ++i)
        delete[] arr2[i];
    delete[] arr2;

}

