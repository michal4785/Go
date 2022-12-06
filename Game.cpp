
//
// Created by Michał on 10.11.2022.
//

#include "Game.h"
#include <cstdio>
#include <cstdlib>


Game::Game(){
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
    kOfight = false;
    selectSize();
    board = new char * [size];
    for (int i = 0; i < size; ++i) {
        board[i] = new char[size];
        for (int j = 0; j < size; ++j) {
            board[i][j] = '0';
        }
    }
    boardLeftCornerX = DEFAULT_BOARD_X;
    boardLeftCornerY = DEFAULT_BOARD_Y;
    initialisedBlackStones = 0;
}


void Game::selectSize(){
    int y = 5;
    int x = 5;
    printClearBoard(y, x, 9);
    x += 22;
    printClearBoard(y, x, 13);
    x += 30;
    printClearBoard(y, x, 19);
    gotoxy(5, 30);
    cputs("Select the size of the board:");
    gotoxy(5, 31);
    cputs("1) 9 x 9 (press 1)");
    gotoxy(5, 32);
    cputs("2) 13 x 13 (press 2)");
    gotoxy(5, 33);
    cputs("3) 19 x 19 (press 3)");
    gotoxy(5, 34);
    cputs("4) Custom (press 4)");
    zn = getchar();
    while(zn != '1' && zn != '2' && zn != '3' && zn != '4'){
        zn = getchar();
    }
    switch(zn){
        case '1':
            size = 9;
            break;
        case '2':
            size = 13;
            break;
        case '3':
            size = 19;
            break;
        case '4':
            gotoxy(5, 35);
            puts( "Input the custom size: ");
            size = 0;
            do{
                zn =  getch();
                if(int(zn) >= int('0') && int(zn) <= int('9')){
                    size *= 10;
                    size += int(zn) - int('0');
                }
                gotoxy(5, 35);
                sprintf(txt, "Input the custom size: %d", size);
                cputs(txt);
            }while(zn != 0x0d);
            break;
    }
    clrscr();
}


void Game::printClearBoard(int y, int x, int givenSize){
    gotoxy(x, y);
    putch('+');
    gotoxy(x + 2 * givenSize + 1, y);
    putch('+');
    gotoxy(x , y + givenSize + 1);
    putch('+');
    gotoxy(x + 2 * givenSize + 1, y + givenSize + 1);
    putch('+');
    for (int i = 0; i < givenSize; ++i) {
        gotoxy(x, y + i + 1);
        putch('|');
    }
    for (int i = 0; i < givenSize; ++i) {
        gotoxy(x + i*2 + 1, y);
        putch('-');
    }
    for (int i = 0; i < givenSize; ++i) {
        gotoxy(x + 2 * givenSize + 1, y + i + 1);
        putch('|');
    }
    for (int i = 0; i < givenSize; ++i) {
        gotoxy(x + i*2 + 1, y + givenSize + 1);
        putch('-');
    }
    for (int i = 0; i < givenSize; ++i) {
        for (int j = 0; j < givenSize; ++j) {
            gotoxy(x + j*2 + 1, y + i + 1);
            putch(' ');
        }
    }
    gotoxy(x, y + givenSize + 3);
    sprintf(txt, "Size:    %d x %d", givenSize, givenSize);
    cputs(txt);
}


void Game::printMenu(int y, int x){
    gotoxy(x, y);
    FILE *file;
    file = fopen("./../menu.txt", "r");
    char    line[BUFFER];
    if(file == nullptr) {
        cputs("Problem with loading menu...");
    }
    else{
        while(fgets(line, BUFFER, file)){
            cputs(line);
            gotoxy(x, ++y);
        }
        fclose(file);
    }
    if(zero) sprintf(txt, "key code: 0x00 0x%02x", zn);
    else sprintf(txt, "key code: 0x%02x", zn);
    gotoxy(x, y++);
    cputs(txt);
    gotoxy(x, y++);
    sprintf(txt, "Y: %d\tX: %d", cursorY, cursorX);
    cputs(txt);
    gotoxy(x, y++);
    sprintf(txt, "White: %d\tBlack: %d", whitePlayerScoresNumber, blackPlayerScoresNumber);
    cputs(txt);
    gotoxy(x, y++);
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
    setInitialisation();
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
        /*if(!findWayOut(y, x) || checkKO()){
            board[y][x] = '0';
            return false;
        }*/
        // else{
        if(!findWayOut(y, x) && !kOfight){
            saveGameState("tmp");
            checking = true;
        }
            switchPlayer();
            int arg[4][2] = {{y+1, x}, {y-1, x},
                             {y, x+1}, {y, x-1}};
            checkTiles(arg);
            return true;
        //}
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
    int counter = 0;
    for (int i = 0; i < 4; ++i) {
        int y = coords[i][0];
        int x = coords[i][1];
        if(checkCoordsValidity(y, x) && board[y][x] == playerCh && !findWayOut(y,x)){
            capture(y,x);
            counter ++;
        }
    }
    if(counter == 0 && checking){
        loadGameState("tmp");
        checking = false;
    }
    else if(checking){
        kOfight = true;
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
    if(file == nullptr){
        gotoxy(boardLeftCornerX - 20, boardLeftCornerY + size + 2);
        cputs("Loading the game failed.\nCheck if the name of the file is proper.");
        return false;
    }
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


void Game::setInitialisation(){
    char * number = new char[10];
    gotoxy(10,10);
    cputs("How many black stones would you like to initialise (end with Enter): ");
    for (int i = 0; i < LITE_BUFFER; ++i) {
        zn =  getch();
        if(zn == 0x0d){
            break;
        }
        number[i] = zn;
        clrscr();
        sprintf( txt, "How many black stones would you like to initialise (end with Enter): %s\n", number);
        cputs(txt);
    }
    int n = atoi(txt);
    initialiseBlackStones(n);
}

void Game::initialiseBlackStones(int n){
    for (int i = 0; i < n; ++i) {
        clrscr();
        printMenu(DEFAULT_MENU_Y, DEFAULT_MENU_X);
        printBoard(DEFAULT_BOARD_Y, DEFAULT_BOARD_X);
        zero = 0;
        zn = getch();
        if(zn == 0) {
            zero = 1;
            zn = getch();
            if(zn == 0x48 && cursorY > 1) cursorY--;
            else if(zn == 0x50 && cursorY < size) cursorY++;
            else if(zn == 0x4b && cursorX > 1) cursorX--;
            else if(zn == 0x4d && cursorX < size) cursorX++;
        } else if(zn == 'i') board[cursorY][cursorX] = '1';
    }
}
