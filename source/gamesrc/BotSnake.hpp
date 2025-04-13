#ifndef BOT_SNAKE_HPP
#define BOT_SNAKE_HPP

#include "../libs/myConst.hpp"
#include "MovingFood.hpp"
#include "Snake.hpp"
#include "SnakeFood.hpp"

class BotSnake : public Snake {
private:
  float calculateDistance(const sf::Vector2f &a, const sf::Vector2f &b) const {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
  }

  // Проверяет, безопасно ли двигаться в определенном направлении
  bool isSafeDirection(Direction dir, const sf::Vector2f &nextPos,
                       const Snake &playerSnake) const {
    // Проверка на выход за границы
    if (nextPos.x < 0 || nextPos.x >= myConst::app_window_size_x ||
        nextPos.y < 0 || nextPos.y >= myConst::app_window_size_y) {
      return false;
    }

    // Проверка на столкновение с собой
    for (size_t i = 1; i < this->getLength(); ++i) {
      if (nextPos == this->getSegmentPosition(i)) {
        return false;
      }
    }

    // Проверка на столкновение с игроком
    for (size_t i = 0; i < playerSnake.getLength(); ++i) {
      if (nextPos == playerSnake.getSegmentPosition(i)) {
        return false;
      }
    }

    return true;
  }

public:
  using Snake::Snake; // Наследуем конструкторы базового класса

  void makeDecision(const std::vector<SnakeFood> &foodArray,
                    const MovingFood &movingFood, const Snake &playerSnake) {
    if (!this->getAliveStatus())
      return;

    sf::Vector2f currentPos = this->getPosition();
    float minDist = std::numeric_limits<float>::max();
    sf::Vector2f targetPos;

    // Проверяем всю обычную еду
    for (const auto &food : foodArray) {
      float dist = calculateDistance(currentPos, food.getPosition());
      if (dist < minDist) {
        minDist = dist;
        targetPos = food.getPosition();
      }
    }

    // Проверяем движущуюся еду с повышенным приоритетом
    float distToMoving =
        calculateDistance(currentPos, movingFood.getPosition()) / 1.5f;
    if (distToMoving < minDist) {
      targetPos = movingFood.getPosition();
    }

    std::vector<std::pair<Direction, sf::Vector2f>> possibleMoves = {
        {Direction::UP, {currentPos.x, currentPos.y - myConst::cell_size}},
        {Direction::DOWN, {currentPos.x, currentPos.y + myConst::cell_size}},
        {Direction::LEFT, {currentPos.x - myConst::cell_size, currentPos.y}},
        {Direction::RIGHT, {currentPos.x + myConst::cell_size, currentPos.y}}};

    minDist = std::numeric_limits<float>::max();
    Direction bestDirection = this->getCurrentDirection();

    for (const auto &move : possibleMoves) {
      if (isSafeDirection(move.first, move.second, playerSnake)) {
        float distance = calculateDistance(move.second, targetPos);
        if (distance < minDist) {
          minDist = distance;
          bestDirection = move.first;
        }
      }
    }

    this->setDirection(bestDirection);
  }
};
#endif /* BOT_SNAKE_HPP */
