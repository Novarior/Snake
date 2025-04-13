#ifndef MOVINGFOOD_HPP
#define MOVINGFOOD_HPP

#include "../libs/helper.hpp"
#include "SnakeFood.hpp"
#include <cstdlib>
#include <iostream>

class MovingFood : public SnakeFood {
private:
  Direction currentDirection;
  int tickCounter = 0;
  int ticksToMove; // Количество тиков до следующего движения

  void updateTicksToMove() {
    // Генерируем случайное число тиков (2 или 3)
    ticksToMove = 2 + (rand() % 2);
  }

  Direction getRandomDirection() {
    switch (rand() % 4) {
    case 0:
      return Direction::UP;
    case 1:
      return Direction::DOWN;
    case 2:
      return Direction::LEFT;
    default:
      return Direction::RIGHT;
    }
  }

  bool isValidPosition(const sf::Vector2f &pos) const {
    return pos.x >= 0 && pos.x < myConst::app_window_size_x && pos.y >= 0 &&
           pos.y < myConst::app_window_size_y;
  }

public:
  MovingFood() : SnakeFood() {
    currentDirection = getRandomDirection();
    food.setFillColor(sf::Color(150, 75, 0)); // Коричневый цвет для мыши
    updateTicksToMove();
  }

  void move() {
    tickCounter++;
    if (tickCounter < ticksToMove) {
      std::cout << "[Mouse:" << tickCounter << "/" << ticksToMove << "] Waiting for next move\n";
      return;
    }

    // Сбрасываем счетчик и обновляем интервал
    tickCounter = 0;
    updateTicksToMove();
    std::cout << "[Mouse] New movement cycle, ticks to next move: " << ticksToMove << "\n";

    sf::Vector2f newPos = position;

    // С вероятностью 10% меняем направление
    if (rand() % 10 == 0) {
      Direction oldDir = currentDirection;
      currentDirection = getRandomDirection();
      std::cout << "[Mouse] Random direction change from " 
                << static_cast<int>(oldDir) << " to " 
                << static_cast<int>(currentDirection) << "\n";
    }

    // Двигаемся в текущем направлении
    switch (currentDirection) {
    case Direction::UP:
      newPos.y -= myConst::cell_size;
      break;
    case Direction::DOWN:
      newPos.y += myConst::cell_size;
      break;
    case Direction::LEFT:
      newPos.x -= myConst::cell_size;
      break;
    case Direction::RIGHT:
      newPos.x += myConst::cell_size;
      break;
    default:
      break;
    }

    std::cout << "[Mouse] Attempting move to position: " << newPos.x << "," << newPos.y << "\n";

    // Если новая позиция валидна, обновляем положение
    if (isValidPosition(newPos)) {
      position = newPos;
      food.setPosition(position);
      std::cout << "[Mouse] Move successful\n";
    } else {
      std::cout << "[Mouse] Hit wall, changing direction\n";
      // Если позиция невалидна, меняем направление на противоположное
      switch (currentDirection) {
      case Direction::UP:
        currentDirection = Direction::DOWN;
        newPos.y += myConst::cell_size;
        break;
      case Direction::DOWN:
        currentDirection = Direction::UP;
        newPos.y -= myConst::cell_size;
        break;
      case Direction::LEFT:
        currentDirection = Direction::RIGHT;
        newPos.x += myConst::cell_size;
        break;
      case Direction::RIGHT:
        currentDirection = Direction::LEFT;
        newPos.x -= myConst::cell_size;
        break;
      default:
        currentDirection = getRandomDirection();
        break;
      }

      if (isValidPosition(newPos)) {
        position = newPos;
        food.setPosition(position);
        std::cout << "[Mouse] Moved in opposite direction to: " 
                  << position.x << "," << position.y << "\n";
      }
    }
  }

  void regeneratePosition() override {
    SnakeFood::regeneratePosition();
    currentDirection = getRandomDirection();
    tickCounter = 0;
    updateTicksToMove();
  }
};
#endif