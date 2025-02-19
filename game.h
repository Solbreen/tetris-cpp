#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <array>
#include <chrono>
#include <future>
#include <opencv2/opencv.hpp>
#include "renderer.h"

class Game {
public:
    Game();
    void run();
private:
    void update();
    void render();
    void handleInput();
    bool isGameOver();
    void spawnTetromino(int& curTetromino);
    bool isTetrominoCanSpawn();
    
    //void spawnMini(int& nextTetramino);

    void moveTetromino();
    bool leftLegalMove();
    bool rightLegalMove();
    bool downLegalMove();

    void rotateTetromino();

    void isBlocked();
    void deleteLines();

    void fallTetromino();

    //void testGenerator();
    //void testNull();

    int nextTetrominoGenerator();

    std::array<std::array<int, 10>, 20> board{0};

    Renderer renderer;
    int key = 0;
    bool freeTetromino = false;
    int curTetromino = 0;
    int nextTetromino = 0;
    int curRotate = 1;

    int level = 1;
    int points = 0;
    int lines = 0;

    std::chrono::steady_clock::time_point lastFallTime;
    const std::chrono::milliseconds fallInterval{1000};

    std::future<void> isBlockFuture;
};
#endif //TETRIS_GAME_H