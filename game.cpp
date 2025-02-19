#include "game.h"
#include <iostream>
#include <utility>
#include <random>
#include <thread>


Game::Game() : lastFallTime(std::chrono::steady_clock::now()){};


void Game::run(){
    

    while (true) {
        handleInput();
        update();
        render();

        if (isGameOver()) {
            std::cout << "Game Over!" << std::endl;
            std::cout << "Final score: " << (*this).points << std::endl;
            std::cout << "Deleted lines count: " << (*this).lines << std::endl;
            break;
        }
    }
}

void Game::update(){
    if (!freeTetromino) {
        deleteLines();
        if((*this).nextTetromino == 0) {
            (*this).curTetromino = nextTetrominoGenerator();
        } else {
            (*this).curTetromino = (*this).nextTetromino;
        }
        (*this).nextTetromino = nextTetrominoGenerator();
        if (isTetrominoCanSpawn()) {
            spawnTetromino((*this).curTetromino);
            (*this).freeTetromino = true;
        }
    }
    if (freeTetromino) {
        if (!(*this).isBlockFuture.valid() || (*this).isBlockFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            (*this).isBlockFuture = std::async(std::launch::async, [this]() {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                isBlocked();
            });
        }
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - (*this).lastFallTime) >= (*this).fallInterval) {
            fallTetromino();
            (*this).lastFallTime = now;
        }
        moveTetromino();
        rotateTetromino();
    }
    
}

void Game::handleInput(){
    (*this).key = cv::waitKey(100);
}

void Game::render(){
    (*this).renderer.clear();
    (*this).renderer.drawBoard((*this).board, (*this).points, (*this).lines, (*this).level, (*this).nextTetromino);
    (*this).renderer.display();
}

bool Game::isGameOver(){
    if ((*this).key == 113) {
        return true;
    }
    if(!isTetrominoCanSpawn() && (*this).freeTetromino == false) {
        return true;
    }
    for (int x = 0; x < 10; ++x) {
        if ((*this).board[0][x] > 0 && (*this).board[0][x] < 10) {
            return true;
        }
    }
    return false;
}

bool Game::isTetrominoCanSpawn(){
    switch((*this).curTetromino){
        //O
        case 1:
            if((*this).board[0][4] == 0 && (*this).board[0][5] == 0 && (*this).board[1][4] == 0 && (*this).board[1][5] == 0){
                return true;
            }
            break;
        //I
        case 2:
            if ((*this).board[0][3] == 0 && (*this).board[0][4] == 0 && (*this).board[0][5] == 0 && (*this).board[0][6] == 0){
                return true;
            }
            break;
        //L
        case 3:
            if((*this).board[0][5] == 0 && (*this).board[1][3] == 0 && (*this).board[1][4] == 0 && (*this).board[1][5] == 0){
                return true;
            }
            break;
        //J
        case 4:
            if((*this).board[0][3] == 0 && (*this).board[1][3] == 0 && (*this).board[1][4] == 0 && (*this).board[1][5] == 0){
                return true;
            }
            break;
        //Z
        case 5:
            if((*this).board[0][3] == 0 && (*this).board[0][4] == 0 && (*this).board[1][4] == 0 && (*this).board[1][5] == 0){
                return true;
            }
            break;
        //S
        case 6:
            if((*this).board[1][3] == 0 && (*this).board[1][4] == 0 && (*this).board[0][4] == 0 && (*this).board[0][5] == 0){
                return true;
            }
            break;
        //T
        case 7:
            if((*this).board[0][4] == 0 && (*this).board[1][3] == 0 && (*this).board[1][4] == 0 && (*this).board[1][5] == 0){
                return true;
            }
            break;
    }   
    return false;
}

void Game::spawnTetromino(int& curTetromino){
    (*this).curRotate = 1;
    switch(curTetromino){
        //O
        case 1:
            (*this).board[0][4] = 11;
            (*this).board[0][5] = 11;
            (*this).board[1][4] = 11;
            (*this).board[1][5] = 11;
            break;
        //I
        case 2:
            (*this).board[0][3] = 12;
            (*this).board[0][4] = 22;
            (*this).board[0][5] = 12;
            (*this).board[0][6] = 12;
            break;
        //L
        case 3:
            (*this).board[0][5] = 13;
            (*this).board[1][3] = 13;
            (*this).board[1][4] = 23;
            (*this).board[1][5] = 13;
            break;
        //J
        case 4:
            (*this).board[0][3] = 14;
            (*this).board[1][3] = 14;
            (*this).board[1][4] = 24;
            (*this).board[1][5] = 14;
            break;
        //Z
        case 5:
            (*this).board[0][3] = 15;
            (*this).board[0][4] = 25;
            (*this).board[1][4] = 15;
            (*this).board[1][5] = 15;
            break;
        //S
        case 6:
            (*this).board[1][3] = 16;
            (*this).board[1][4] = 16;
            (*this).board[0][4] = 26;
            (*this).board[0][5] = 16;
            break;
        //T
        case 7:
            (*this).board[0][4] = 17;
            (*this).board[1][3] = 17;
            (*this).board[1][4] = 27;
            (*this).board[1][5] = 17;
            break;
    }
}

int Game::nextTetrominoGenerator(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 7);

    return dist(gen); 
}

bool Game::leftLegalMove(){
    int count = 4;
    for (int i = 0; i < 20; ++i){
        for (int j = 0; j < 10; ++j) {
            if (j > 0 && (*this).board[i][j] > 10 && ((*this).board[i][j-1] == 0 || (*this).board[i][j-1] > 10)) {
                --count;
            }
        }
    }
    if (count > 0) {
        return false;
    } else {
        return true;
    }
}

bool Game::downLegalMove(){
    int count = 4;
    for (int i = 19; i >= 0; --i){
        for (int j = 0; j < 10; ++j) {
            if (i < 19 && (*this).board[i][j] > 10 && ((*this).board[i+1][j] == 0 || (*this).board[i+1][j] > 10)) {
                --count;
            }
        }
    }
    if (count > 0) {
        return false;
    } else {
        return true;
    }
}

bool Game::rightLegalMove(){
    int count = 4;
    for (int i = 0; i < 20; ++i){
        for (int j = 9; j >= 0; --j) {
            if (j < 9 && (*this).board[i][j] > 10 && ((*this).board[i][j+1] == 0 || (*this).board[i][j+1] > 10)) {
                --count;
            }
        }
    }
    if (count > 0) {
        return false;
    } else {
        return true;
    }
}

void Game::moveTetromino(){
    switch((*this).key) {
        case 120:
            if (leftLegalMove()) {
                for (int i = 0; i < 20; ++i){
                    for (int j = 0; j < 10; ++j) {
                        if (j > 0 && (*this).board[i][j] > 10) {
                            std::swap((*this).board[i][j], (*this).board[i][j-1]);
                        }
                    }
                }
            }
            break;
        /*case 82: // ПОТОМ УДАЛИТЬ!!!
            for (int i = 0; i < 20; ++i){
                for (int j = 0; j < 10; ++j) {
                    if (i > 0 && (*this).board[i][j] > 10) {
                        std::swap((*this).board[i][j], (*this).board[i-1][j]);
                    }
                }
            }
        break;*/
        case 99:
            if (rightLegalMove()) {
                for (int i = 0; i < 20; ++i){
                    for (int j = 9; j >= 0; --j) {
                        if (j - 9 && (*this).board[i][j] > 10) {
                            std::swap((*this).board[i][j], (*this).board[i][j+1]);
                        }
                    }
                }
            }
            break;
        case 118:
            if (downLegalMove()) {
                for (int i = 19; i >= 0; --i){
                    for (int j = 9; j >= 0; --j) {
                        if (i < 19 && (*this).board[i][j] > 10) {
                            std::swap((*this).board[i][j], (*this).board[i+1][j]);
                        }
                    }
                }
                (*this).points += 1;
            }
            break;
    }
}

void Game::fallTetromino(){
    if (downLegalMove()) {
        for (int i = 19; i >= 0; --i){
            for (int j = 9; j >= 0; --j) {
                if (i < 19 && (*this).board[i][j] > 10) {
                    std::swap((*this).board[i][j], (*this).board[i+1][j]);
                }
            }
        }
    }
}

void Game::rotateTetromino(){
    if ((*this).key == 122) {
        bool exit = false;
        switch ((*this).curTetromino) {
            //I
            case 2:
                switch ((*this).curRotate) {
                    case 1:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i < 18 && i > 0 && (*this).board[i-1][j] == 0 && (*this).board[i+1][j] == 0 && (*this).board[i+2][j] == 0) {
                                        std::swap((*this).board[i-1][j], (*this).board[i][j-1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i][j+1]);
                                        std::swap((*this).board[i+2][j], (*this).board[i][j+2]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 2:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j < 9 && j > 1 && (*this).board[i][j+1] == 0 && (*this).board[i][j-1] == 0 && (*this).board[i][j-2] == 0) {
                                        std::swap((*this).board[i][j+1], (*this).board[i-1][j]);
                                        std::swap((*this).board[i][j-1], (*this).board[i+1][j]);
                                        std::swap((*this).board[i][j-2], (*this).board[i+2][j]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 3:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i < 19 && i > 1 && (*this).board[i-1][j] == 0 && (*this).board[i+1][j] == 0 && (*this).board[i-2][j] == 0) {
                                        std::swap((*this).board[i-1][j], (*this).board[i][j-1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i][j+1]);
                                        std::swap((*this).board[i-2][j], (*this).board[i][j-2]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 4:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j < 8 && j > 0 && (*this).board[i][j+1] == 0 && (*this).board[i][j-1] == 0 && (*this).board[i][j+2] == 0) {
                                        std::swap((*this).board[i][j+1], (*this).board[i-1][j]);
                                        std::swap((*this).board[i][j-1], (*this).board[i+1][j]);
                                        std::swap((*this).board[i][j+2], (*this).board[i-2][j]);
                                        (*this).curRotate = 1;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                }
                break;
            //L
            case 3:
                switch ((*this).curRotate) {
                    case 1:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i < 19 && (*this).board[i-1][j] == 0 && (*this).board[i-1][j-1] == 0 && (*this).board[i+1][j] == 0) {
                                        std::swap((*this).board[i-1][j+1], (*this).board[i-1][j-1]);
                                        std::swap((*this).board[i-1][j], (*this).board[i][j+1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i][j-1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 2:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j < 9 && (*this).board[i][j+1] == 0 && (*this).board[i][j-1] == 0 && (*this).board[i+1][j-1] == 0) {
                                        std::swap((*this).board[i-1][j-1], (*this).board[i+1][j-1]);
                                        std::swap((*this).board[i-1][j], (*this).board[i][j-1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i][j+1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 3:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i < 19 && (*this).board[i-1][j] == 0 && (*this).board[i+1][j+1] == 0 && (*this).board[i+1][j] == 0) {
                                        std::swap((*this).board[i+1][j+1], (*this).board[i+1][j-1]);
                                        std::swap((*this).board[i-1][j], (*this).board[i][j+1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i][j-1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 4:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j > 0 && (*this).board[i][j-1] == 0 && (*this).board[i-1][j+1] == 0 && (*this).board[i][j+1] == 0) {
                                        std::swap((*this).board[i-1][j], (*this).board[i][j-1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i][j+1]);
                                        std::swap((*this).board[i+1][j+1], (*this).board[i-1][j+1]);
                                        (*this).curRotate = 1;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                }
                break;
            //J
            case 4:
                switch ((*this).curRotate) {
                    case 1:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i < 19 && (*this).board[i-1][j] == 0 && (*this).board[i+1][j-1] == 0 && (*this).board[i+1][j] == 0) {
                                        std::swap((*this).board[i+1][j-1], (*this).board[i-1][j-1]);
                                        std::swap((*this).board[i-1][j], (*this).board[i][j+1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i][j-1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 2:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j < 9 && (*this).board[i][j+1] == 0 && (*this).board[i][j-1] == 0 && (*this).board[i+1][j+1] == 0) {
                                        std::swap((*this).board[i+1][j+1], (*this).board[i+1][j-1]);
                                        std::swap((*this).board[i-1][j], (*this).board[i][j-1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i][j+1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 3:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i < 19 && (*this).board[i-1][j] == 0 && (*this).board[i-1][j+1] == 0 && (*this).board[i+1][j] == 0) {
                                        std::swap((*this).board[i+1][j+1], (*this).board[i-1][j+1]);
                                        std::swap((*this).board[i-1][j], (*this).board[i][j+1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i][j-1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 4:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j > 0 && (*this).board[i][j-1] == 0 && (*this).board[i-1][j-1] == 0 && (*this).board[i][j+1] == 0) {
                                        std::swap((*this).board[i-1][j], (*this).board[i][j-1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i][j+1]);
                                        std::swap((*this).board[i-1][j+1], (*this).board[i-1][j-1]);
                                        (*this).curRotate = 1;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                }
                break;
            //Z
            case 5:
                switch ((*this).curRotate) {
                    case 1:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i > 0 && (*this).board[i-1][j+1] == 0 && (*this).board[i][j+1] == 0) {
                                        std::swap((*this).board[i+1][j+1], (*this).board[i-1][j+1]);
                                        std::swap((*this).board[i][j-1], (*this).board[i][j+1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 2:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j > 0 && (*this).board[i-1][j] == 0 && (*this).board[i-1][j-1] == 0) {
                                        std::swap((*this).board[i+1][j], (*this).board[i-1][j]);
                                        std::swap((*this).board[i-1][j-1], (*this).board[i-1][j+1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 3:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i < 19 && (*this).board[i+1][j-1] == 0 && (*this).board[i][j-1] == 0) {
                                        std::swap((*this).board[i+1][j-1], (*this).board[i-1][j-1]);
                                        std::swap((*this).board[i][j-1], (*this).board[i][j+1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 4:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j < 9 && (*this).board[i+1][j] == 0 && (*this).board[i+1][j+1] == 0) {
                                        std::swap((*this).board[i+1][j], (*this).board[i-1][j]);
                                        std::swap((*this).board[i+1][j-1], (*this).board[i+1][j+1]);
                                        (*this).curRotate = 1;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                }
                break;
            //S
            case 6:
                switch ((*this).curRotate) {
                    case 1:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i > 0 && (*this).board[i+1][j+1] == 0 && (*this).board[i-1][j] == 0) {
                                        std::swap((*this).board[i+1][j-1], (*this).board[i+1][j+1]);
                                        std::swap((*this).board[i+1][j], (*this).board[i-1][j]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 2:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j > 0 && (*this).board[i-1][j+1] == 0 && (*this).board[i][j-1] == 0) {
                                        std::swap((*this).board[i-1][j+1], (*this).board[i+1][j+1]);
                                        std::swap((*this).board[i][j-1], (*this).board[i][j+1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 3:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i < 19 && (*this).board[i+1][j] == 0 && (*this).board[i-1][j-1] == 0) {
                                        std::swap((*this).board[i+1][j], (*this).board[i-1][j]);
                                        std::swap((*this).board[i-1][j-1], (*this).board[i-1][j+1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 4:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j < 9 && (*this).board[i+1][j-1] == 0 && (*this).board[i][j+1] == 0) {
                                        std::swap((*this).board[i+1][j-1], (*this).board[i-1][j-1]);
                                        std::swap((*this).board[i][j-1], (*this).board[i][j+1]);
                                        (*this).curRotate = 1;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                }
                break;
            //T
            case 7:
                switch ((*this).curRotate) {
                    case 1:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i < 19 && (*this).board[i+1][j] == 0) {
                                        std::swap((*this).board[i+1][j], (*this).board[i][j+1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 2:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j < 9 && (*this).board[i][j+1] == 0) {
                                        std::swap((*this).board[i-1][j], (*this).board[i][j+1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 3:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(i > 0 && (*this).board[i-1][j] == 0) {
                                        std::swap((*this).board[i-1][j], (*this).board[i][j-1]);
                                        ++(*this).curRotate;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                    case 4:
                        for (int i = 0; i < 20; ++i) {
                            for (int j = 0; j < 10; ++j) {
                                if ((*this).board[i][j] > 20) {
                                    if(j > 0 && (*this).board[i][j-1] == 0) {
                                        std::swap((*this).board[i+1][j], (*this).board[i][j-1]);
                                        (*this).curRotate = 1;
                                        exit = true;
                                        break;
                                    }
                                }
                            }
                            if (exit) {break;}
                        }
                        break;
                }
                break;
        }
    }
}

void Game::isBlocked(){
    if (!downLegalMove()) {
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 10; ++j) {
                if ((*this).board[i][j] > 10) {
                    (*this).board[i][j] %= 10;
                }
            }
        }
        (*this).freeTetromino = false;
    }
}

void Game::deleteLines(){
    int curCountDeletedLines = 0;
    for (int i = 19; i >= 0; --i){
        for (int j = 0; j < 10; ++j){
            if ((*this).board[i][j] == 0) {break;}
            if (j == 9 && (*this).board[i][j] != 0) {
                curCountDeletedLines += 1;
                (*this).board[i].fill(0);
            }
        }
    }

    int writeIndex = 19;

    for (int i = 19; i >= 0; --i){
        for (int j = 0; j < 10; ++j){
            if ((*this).board[i][j] != 0 && i == writeIndex) {
                --writeIndex;
                break;
            }
            if ((*this).board[i][j] != 0 && i < writeIndex) {
                (*this).board[writeIndex--] = (*this).board[i];
                (*this).board[i].fill(0);
                break;
            }
            if (j == 9 && (*this).board[i][j] == 0) {break;}
        }
    }
    switch(curCountDeletedLines){
        case 1:
            (*this).points += 100;
            break;
        case 2:
            (*this).points += 300;
            break;
        case 3:
            (*this).points += 500;
            break;
        case 4:
            (*this).points += 800;
            break;
    }
    (*this).lines += curCountDeletedLines;
}


/*
void Game::testNull(){
    if ((*this).key == 120) {
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 10; ++j) {
                (*this).board[i][j] = 0;
            }
        }
    }
}
void Game::testGenerator(){
    switch ((*this).key) {
        case 49:
            //testNull();
            (*this).freeTetromino = false;
            (*this).nextTetromino = 1;
            break;
        case 50:
            //testNull();
            (*this).freeTetromino = false;
            (*this).nextTetromino = 2;
            break;
        case 51:
            //testNull();
            (*this).freeTetromino = false;
            (*this).nextTetromino = 3;
            break;
        case 52:
            //testNull();
            (*this).freeTetromino = false;
            (*this).nextTetromino = 4;
            break;
        case 53:
            //testNull();
            (*this).freeTetromino = false;
            (*this).nextTetromino = 5;
            break;
        case 54:
            //testNull();
            (*this).freeTetromino = false;
            (*this).nextTetromino = 6;
            break;
        case 55:
            //testNull();
            (*this).freeTetromino = false;
            (*this).nextTetromino = 7;
            break;
    }
}
*/
