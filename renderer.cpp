#include "renderer.h"
#include <string>


Renderer::Renderer() = default;

void Renderer::clear() {
    (*this).image.setTo(cv::Scalar(0, 0, 0));
}

void Renderer::drawBoard(std::array<std::array<int, 10>, 20>& board, int points, int lines, int level, int nextTetromino) {
    (*this).image = cv::Mat::zeros(690, 660, CV_8UC3);
    (*this).image = cv::Scalar(0, 0, 0);
    cv::rectangle((*this).image, cv::Point(30, 30), cv::Point(330, 630), cv::Scalar(40, 40, 40), -1); // Игрровое поле

    cv::rectangle((*this).image, cv::Point(420, 90), cv::Point(600, 150), cv::Scalar(40, 40, 40), -1); // Очки
    cv::rectangle((*this).image, cv::Point(420, 210), cv::Point(600, 270), cv::Scalar(40, 40, 40), -1); // Убрано
    cv::rectangle((*this).image, cv::Point(420, 330), cv::Point(600, 390), cv::Scalar(40, 40, 40), -1); // Уровень
    cv::rectangle((*this).image, cv::Point(420, 480), cv::Point(540, 600), cv::Scalar(40, 40, 40), -1); // След. фигура

    cv::putText((*this).image, "Points:", cv::Point(420, 85), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2); // Очки
    cv::putText((*this).image, std::to_string(points), cv::Point(430, 140), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2); // Очки в цифре

    cv::putText((*this).image, "Lines:", cv::Point(420, 205), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2); // Убрано
    cv::putText((*this).image, std::to_string(lines), cv::Point(430, 260), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2); // Убрано в цифре

    cv::putText((*this).image, "Level:", cv::Point(420, 325), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2); // Уровень
    cv::putText((*this).image, std::to_string(level), cv::Point(430, 380), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2); // Уровень в цифре
    
    cv::putText((*this).image, "Next piece:", cv::Point(420, 465), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2); // След. фигура

    for (int i = 0; i < 20; ++i){
        for (int j = 0; j < 10; ++j) {
            cv::rectangle((*this).image, cv::Point((j+1)*(*this).cellSize, (i+1)*(*this).cellSize), cv::Point((j+2)*(*this).cellSize, (i+2)*(*this).cellSize), (*this).tColor[board[i][j] % 10], -1);
        }
    }

    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j) {
            cv::rectangle((*this).image, cv::Point((j+14)*(*this).cellSize, (i+16)*(*this).cellSize), cv::Point((j+15)*(*this).cellSize, (i+17)*(*this).cellSize), (*this).tColor[(*this).miniDeck[nextTetromino-1][i][j] % 10], -1);
        }
    }

    for (int x = 0; x < 9; ++x) {
        cv::line((*this).image, cv::Point((x+2)*(*this).cellSize, (*this).cellSize), cv::Point((x+2)*(*this).cellSize, 21*(*this).cellSize), cv::Scalar(127, 127, 127), 1);
    }
    for (int y = 0; y < 19; ++y) {
        cv::line((*this).image, cv::Point((*this).cellSize, (y+2)*(*this).cellSize), cv::Point(11*(*this).cellSize, (y+2)*(*this).cellSize), cv::Scalar(127, 127, 127), 1);
    }
    for (int x = 15; x < 18; ++x) {
        cv::line((*this).image, cv::Point(x*(*this).cellSize, 16*(*this).cellSize), cv::Point(x*(*this).cellSize, 20*(*this).cellSize), cv::Scalar(127, 127, 127), 1);
    }
    for (int y = 17; y < 20; ++y) {
        cv::line((*this).image, cv::Point(14*(*this).cellSize, y*(*this).cellSize), cv::Point(18*(*this).cellSize, y*(*this).cellSize), cv::Scalar(127, 127, 127), 1);
    }

    cv::line((*this).image, cv::Point((*this).cellSize, (*this).cellSize), cv::Point((*this).cellSize, 21*(*this).cellSize), cv::Scalar(255, 255, 255), 1);
    cv::line((*this).image, cv::Point((*this).cellSize, (*this).cellSize), cv::Point(11*(*this).cellSize, (*this).cellSize), cv::Scalar(255, 255, 255), 1);
    cv::line((*this).image, cv::Point((*this).cellSize, 21*(*this).cellSize), cv::Point(11*(*this).cellSize, 21*(*this).cellSize), cv::Scalar(255, 255, 255), 1);
    cv::line((*this).image, cv::Point(11*(*this).cellSize, (*this).cellSize), cv::Point(11*(*this).cellSize, 21*(*this).cellSize), cv::Scalar(255, 255, 255), 1);

    cv::line((*this).image, cv::Point(14*(*this).cellSize, 16*(*this).cellSize), cv::Point(14*(*this).cellSize, 20*(*this).cellSize), cv::Scalar(255, 255, 255), 1);
    cv::line((*this).image, cv::Point(14*(*this).cellSize, 16*(*this).cellSize), cv::Point(18*(*this).cellSize, 16*(*this).cellSize), cv::Scalar(255, 255, 255), 1);
    cv::line((*this).image, cv::Point(14*(*this).cellSize, 20*(*this).cellSize), cv::Point(18*(*this).cellSize, 20*(*this).cellSize), cv::Scalar(255, 255, 255), 1);
    cv::line((*this).image, cv::Point(18*(*this).cellSize, 16*(*this).cellSize), cv::Point(18*(*this).cellSize, 20*(*this).cellSize), cv::Scalar(255, 255, 255), 1);
}   

void Renderer::display() const {
    cv::imshow("Tetris", (*this).image);
    cv::waitKey(1);
}