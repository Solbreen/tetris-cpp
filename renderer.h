#ifndef TETRIS_RENDERER_H
#define TETRIS_RENDERER_H

#include <opencv2/opencv.hpp>
#include <array>

class Renderer {
public:
    Renderer();
    void clear();
    void drawBoard(std::array<std::array<int, 10>, 20>& board, int points, int lines, int level, int nextTetromino);
    void display() const;

private:
    cv::Mat image;
    const int cellSize = 30;
    const std::array<cv::Scalar, 8> tColor = {
        cv::Scalar(40, 40, 40),
        cv::Scalar(255, 255, 0), // O 
        cv::Scalar(64, 224, 208), // I
        cv::Scalar(255, 165, 0), // L
        cv::Scalar(0, 0, 255), // J
        cv::Scalar(255, 0, 0), // Z
        cv::Scalar(0, 128, 0), // S
        cv::Scalar(128, 0, 128) // T
    };
    int miniDeck[7][4][4] = {
        // O
        {
            {0,0,0,0},
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        // I
        {
            {0,0,0,0},
            {2,2,2,2},
            {0,0,0,0},
            {0,0,0,0}
        },
        // L
        {
            {0,0,3,0},
            {3,3,3,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        // J
        {
            {0,4,0,0},
            {0,4,4,4},
            {0,0,0,0},
            {0,0,0,0}
        },
        // Z
        {
            {0,0,0,0},
            {5,5,0,0},
            {0,5,5,0},
            {0,0,0,0}
        },
        // S
        {
            {0,0,0,0},
            {0,0,6,6},
            {0,6,6,0},
            {0,0,0,0}
        },
        // T 
        {
            {0,0,0,0},
            {0,7,0,0},
            {7,7,7,0},
            {0,0,0,0},
        }
    };  
};
#endif //TETRIS_RENDERER_H