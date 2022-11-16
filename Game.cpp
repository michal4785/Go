#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-branch-clone"
#pragma ide diagnostic ignored "OCDFAInspection"
#pragma ide diagnostic ignored "misc-no-recursion"
//
// Created by Michał on 10.11.2022.
//

#include "Game.h"
#include <cstdio>
#include <cstdlib>

Game::Game(int size){
    this->size = size;
    zn = 0;
    cursorX =  1;
    cursorY = 1;
    attr = 7;
    back = 0;
    zero = 0;
    player = 1;
    opponent = 2;
    playerCh = '1';
    opponentCh = '2';
    txt = new char[BUFFER];
    whitePlayerScoresNumber = 0;
    blackPlayerScoresNumber = 0;
    board = new char * [size];
    for (int i = 0; i < size; ++i) {
        board[i] = new char[size];
        for (int j = 0; j < size; ++j) {
            board[i][j] = '0';
        }
    }
    boardLeftCornerX = DEFAULT_BOARD_X;
    boardLeftCornerY = DEFAULT_BOARD_Y;
}
void Game::printMenu(int y, int x){
    gotoxy(x, y);
    cputs("Name: Michal");
    gotoxy(x, y + 1);
    cputs("Surname: Ganczarenko");
    gotoxy(x, y + 2);
    cputs("Index number: 188818");
    gotoxy(x, y + 3);
    cputs("arrows = cursor moving");
    gotoxy(x, y + 4);
    cputs("q       = exit");
    gotoxy(x, y + 5);
    cputs("n       = new game (not implemented)");
    gotoxy(x, y + 6);
    cputs("enter   = confirm choice and end turn (not implemented)");
    gotoxy(x, y + 7);
    cputs("esc       = cancel  (not implemented)");
    gotoxy(x, y + 8);
    cputs("i       = place a stone (not implemented)");
    gotoxy(x, y + 9);
    cputs("s       = save game (not implemented)");
    gotoxy(x, y + 10);
    cputs("l       = load game (not implemented)");
    gotoxy(x, y + 11);
    cputs("f       = finish game (not implemented)");
    gotoxy(x, y + 12);
    cputs("space   = change color");
    gotoxy(x, y + 13);
    cputs("enter   = change background color");
    if(zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
    else sprintf(txt, "key code: 0x%02x", zn);
    gotoxy(x, y + 14);
    cputs(txt);
    gotoxy(x, y + 15);
    sprintf(txt, "Y: %d\tX: %d", cursorY, cursorX);
    cputs(txt);
    gotoxy(x, y + 16);
    sprintf(txt, "White: %d\tBlack: %d", whitePlayerScoresNumber, blackPlayerScoresNumber);
    cputs(txt);
    gotoxy(x, y + 17);
    if(player == 1){
        cputs("Current player: BLACK");
    }
    else{
        cputs("Current player: WHITE");
    }

}

void Game::printBoard(int y, int x){
    gotoxy(x, y);
    putch('+');
    gotoxy(x + 2*size + 1, y);
    putch('+');
    gotoxy(x , y + size + 1);
    putch('+');
    gotoxy(x + 2*size + 1, y + size + 1);
    putch('+');
    for (int i = 0; i < size; ++i) {
        gotoxy(x, y + i + 1);
        putch('|');
    }
    for (int i = 0; i < size; ++i) {
        gotoxy(x + i*2 + 1, y);
        putch('-');
    }
    for (int i = 0; i < size; ++i) {
        gotoxy(x + 2*size + 1, y + i + 1);
        putch('|');
    }
    for (int i = 0; i < size; ++i) {
        gotoxy(x + i*2 + 1, y + size + 1);
        putch('-');
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            gotoxy(x + j*2 + 1, y + i + 1);
            if(board[i][j] != '0'){
                putch(board[i][j]);
            }
        }
    }
    gotoxy(x+2*cursorX-1, y+cursorY);
    textcolor(attr);
    textbackground(back);
    putch('*');
}

void Game::listenKeys(){
    zero = 0;
    zn = getch();
    if(zn == 0) {
        zero = 1;
        zn = getch();
        if(zn == 0x48 && cursorY > 1) cursorY--;
        else if(zn == 0x50 && cursorY < size) cursorY++;
        else if(zn == 0x4b && cursorX > 1) cursorX--;
        else if(zn == 0x4d && cursorX < size) cursorX++;
    } else if(zn == ' ') attr = (attr + 1) % 16;
    else if(zn == 'n') restart();
    else if(zn == 'i') tryToPlaceStone(cursorY - 1, cursorX - 1);
    else if(zn == 's' || zn == 'l') inputFileName();
    else if(zn == 0x1b) loadGameState("beforeLastMove");
    else if(zn == 0x0d) back = (back + 1) % 16;
}
void Game::start() {
    saveGameState("beforeLastMove");
    saveGameState("beforeBeforeLastMove");
#ifndef __cplusplus
    Conio2_Init();
#endif
    settitle("Michal, Ganczarenko, 188818");
    _setcursortype(_NOCURSOR);
    do {
        textbackground(BLACK);
        clrscr();
        textcolor(7);
        printMenu(DEFAULT_MENU_Y, DEFAULT_MENU_X);
        printBoard(DEFAULT_BOARD_Y, DEFAULT_BOARD_X);
        listenKeys();

    } while (zn != 'q');
    _setcursortype(_NORMALCURSOR);
}


void Game::switchPlayer(){
    opponent = player;
    player = player % 2 + 1;
    playerCh = char(player + int('0'));
    opponentCh = char(opponent + int('0'));
}

void Game::clearBoard() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            board[i][j] = '0';
        }
    }
}


void Game::restart(){
    cursorX =  1;
    cursorY = 1;
    player = 1;
    whitePlayerScoresNumber = 0;
    blackPlayerScoresNumber = 0;
    clearBoard();
}


bool Game::tryToPlaceStone(int y, int x){
    if(board[y][x] != '0'){
        return false;
    }
    else{
        updateSaves();
        board[y][x] = playerCh;
        if(!findWayOut(y, x) || checkKO()){
            board[y][x] = '0';
            return false;
        }
        else{
            switchPlayer();
            int arg[4][2] = {{y+1, x}, {y-1, x},
                             {y, x+1}, {y, x-1}};
            checkTiles(arg);
            return true;
        }
    }
}

bool Game::checkKO(){
    saveGameState("tmp");
    FILE    *file, *otherFile;
    sprintf(txt, "./../%s.txt", "tmp");
    file = fopen(txt, "r");
    sprintf(txt, "./../%s.txt", "beforeLastMove");
    otherFile = fopen(txt, "r");
    char ch1, ch2;
    do{
        ch1 = getc(file);
        ch2 = getc(otherFile);
        if(ch1 != ch2){
            fclose(file);
            fclose(otherFile);
            return false;
        }
    }
    while(ch1 != EOF && ch2 != EOF);
    fclose(file);
    fclose(otherFile);
    return true;
}

bool Game::checkCoordsValidity(int y, int x) const{
    return y >= 0 && y < size && x >= 0 && x < size;
}

void Game::checkTiles(const int coords[4][2]){
    for (int i = 0; i < 4; ++i) {
        int y = coords[i][0];
        int x = coords[i][1];
        if(checkCoordsValidity(y, x) && board[y][x] == playerCh && !findWayOut(y,x)){
            capture(y,x);
        }
    }
}

bool Game::findWayOut(int y, int x){
    if(!checkCoordsValidity(y, x)){
        return false;
    }
    else if(board[y][x] == '0'){
        return true;
    }
    else if(board[y][x] == opponentCh){
        return false;
    }
    else{
        board[y][x] = opponentCh;
        bool result =  findWayOut(y+1, x) || findWayOut(y-1, x)
                       || findWayOut(y, x+1) || findWayOut(y, x-1);
        board[y][x] = playerCh;
        return result;
    }
}

void Game::capture(int y, int x){
    if(checkCoordsValidity(y, x) && board[y][x] == playerCh){
        if(player != 1){
            blackPlayerScoresNumber++;
        }
        else{
            whitePlayerScoresNumber++;
        }
        board[y][x] = '0';
        capture(y+1, x);
        capture(y-1, x);
        capture(y, x+1);
        capture(y, x-1);
    }
}



void Game::updateSaves(){
    saveGameState("tmp");
    loadGameState("beforeLastMove");
    saveGameState("beforeBeforeLastMove");
    loadGameState("tmp");
    saveGameState("beforeLastMove");
}

void Game::saveGameState(char * fileName){
    sprintf(txt, "./../%s.txt", fileName);
    FILE *file;
    file = fopen(txt, "w");

    if(file == nullptr) {
        printf("file couldn't be opened\n");
        exit(1);
    }
    sprintf(txt, "%d\n", player);
    fputs(txt,file);
    sprintf(txt, "%d\n", blackPlayerScoresNumber);
    fputs(txt,file);
    sprintf(txt, "%d\n", whitePlayerScoresNumber);
    fputs(txt,file);
    sprintf(txt, "%d\n", size);
    fputs(txt,file);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            sprintf(txt, "%c", board[i][j]);
            fputs(txt, file);
        }
        fputs("\n", file);
    }
    fclose(file);
}

bool Game::loadGameState(char * fileName){
    FILE    *file;
    char    line[BUFFER];
    sprintf(txt, "./../%s.txt", fileName);
    file = fopen(txt, "r");
    if(file == nullptr)
        return false;
    else{

        fgets(line, BUFFER, file);
        player = atoi(line);
        fgets(line, BUFFER, file);
        blackPlayerScoresNumber = atoi(line);
        fgets(line, BUFFER, file);
        whitePlayerScoresNumber = atoi(line);
        fgets(line, BUFFER, file);
        size = atoi(line);
        for (int i = 0; i < size; ++i) {
            fgets(line, 100, file);
            for (int j = 0; j < size; ++j) {
                board[i][j] = line[j];
            }
        }
        fclose(file);
        switchPlayer();
        switchPlayer();
        return true;
    }
}

void Game::inputFileName(){
    int y = boardLeftCornerY + size + 2;
    int x = boardLeftCornerX - 20;
    gotoxy(x, y);
    char * fileName = new char[LITE_BUFFER];
    for (int i = 0; i <LITE_BUFFER ; ++i) {
        fileName[i] = 0;
    }
    if(zn == 's'){
        cputs("Input the name of file to save: ");
        for (int i = 0; i < LITE_BUFFER; ++i) {
            zn =  getch();
            if(zn == 0x0d){
                break;
            }
            fileName[i] = zn;
            gotoxy(x, y);
            sprintf(txt, "Input the name of file to save: %s\n", fileName);
            cputs(txt);
        }
        saveGameState(fileName);
    }
    else if(zn == 'l'){
        cputs("Input the name of file to load: ");
        for (int i = 0; i < LITE_BUFFER; ++i) {
            zn =  getch();
            if(zn == 0x0d){
                break;
            }
            fileName[i] = zn;
            gotoxy(x, y);
            sprintf(txt, "Input the name of file to load: %s\n", fileName);
            cputs(txt);
        }
        loadGameState(fileName);
    }
}
#pragma clang diagnostic pop