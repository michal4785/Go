//
// Created by Michał on 16.11.2022.
//

#ifndef GO_GAME_H
#define GO_GAME_H

#include  <cstdio>
#include "conio2.h"


class Game {
public:
    explicit Game(int size);
    void printMenu(int y, int x);
    void printBoard(int y, int x);
    void listenKeys();
    void start();
    void switchPlayer();
    void clearBoard();
    void restart();
    bool tryToPlaceStone(int y, int x);
    bool checkKO();
    bool findWayOut(int y, int x);
    void capture(int y, int x);
    bool checkCoordsValidity(int y, int x) const;
    void checkTiles(const int coords[4][2]);
    void updateSaves();
    void saveGameState(char * fileName);
    bool loadGameState(char * fileName);
    void inputFileName();

private:
    int zn;
    char * txt;
    int zero;
    int cursorY;
    int cursorX;
    int attr;
    int back;
    int size;
    int player;
    int opponent;
    char playerCh;
    char opponentCh;
    int whitePlayerScoresNumber;
    int blackPlayerScoresNumber;
    char ** board;
    int boardLeftCornerX;
    int boardLeftCornerY;
    static const int DEFAULT_MENU_X = 5;
    static const int DEFAULT_MENU_Y = 2;
    static const int DEFAULT_BOARD_X = 100;
    static const int DEFAULT_BOARD_Y = 2;
    static const int BUFFER = 50;
    static const int LITE_BUFFER = 10;
};

#endif //GO_GAME_H
