#ifndef SNAKEWORLD_HPP
#define SNAKEWORLD_HPP

#include "BotSnake.hpp"
#include "MovingFood.hpp"
#include "Snake.hpp"
#include "SnakeFood.hpp"
#include <iostream>

class World : public sf::Drawable {
private:
  static const size_t FOOD_COUNT = 5; // Увеличиваем количество обычной еды
  static int callIndex; // Добавляем счетчик вызовов

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    // Отрисовываем тайлы
    for (const auto &row : tiles) {
      for (const auto &tile : row) {
        target.draw(tile, states);
      }
    }
    // Отрисовываем обычную еду
    for (const auto &food : mFoodArray) {
      target.draw(food);
    }
    // Отрисовываем движущуюся еду
    target.draw(mMovingFood);
    // Отрисовываем змей
    target.draw(mPlayerSnake);
    target.draw(mBotSnake);
  }

  bool isValidDirection(Direction newDir, const Snake &snake) const {
    Direction currentDir = snake.getCurrentDirection();
    if (currentDir == Direction::UP && newDir == Direction::DOWN)
      return false;
    if (currentDir == Direction::DOWN && newDir == Direction::UP)
      return false;
    if (currentDir == Direction::LEFT && newDir == Direction::RIGHT)
      return false;
    if (currentDir == Direction::RIGHT && newDir == Direction::LEFT)
      return false;
    return true;
  }

  void moveSnake(Snake &snake) {
    if (!snake.getAliveStatus() ||
        snake.getCurrentDirection() == Direction::IDLE) {
      std::cout << "[" << ++callIndex << "] moveSnake: Movement skipped. Alive="
                << snake.getAliveStatus() << ", Direction="
                << static_cast<int>(snake.getCurrentDirection()) << "\n";
      return;
    }

    std::cout << "[" << ++callIndex
              << "] moveSnake: Moving snake. Length=" << snake.getLength()
              << "\n";
    std::vector<sf::Vector2f> prevPositions;
    for (size_t i = 0; i < snake.getLength(); ++i) {
      prevPositions.push_back(snake.getSegmentPosition(i));
    }

    sf::Vector2f newHeadPos = prevPositions[0];
    switch (snake.getCurrentDirection()) {
    case Direction::UP:
      newHeadPos.y -= myConst::cell_size;
      break;
    case Direction::DOWN:
      newHeadPos.y += myConst::cell_size;
      break;
    case Direction::LEFT:
      newHeadPos.x -= myConst::cell_size;
      break;
    case Direction::RIGHT:
      newHeadPos.x += myConst::cell_size;
      break;
    default:
      break;
    }
    std::cout << "[" << ++callIndex
              << "] moveSnake: Moving head to position: " << newHeadPos.x << ","
              << newHeadPos.y << "\n";
    snake.moveSegment(0, newHeadPos);

    for (size_t i = 1; i < snake.getLength(); ++i) {
      std::cout << "[" << ++callIndex << "] moveSnake: Moving segment " << i
                << " to position: " << prevPositions[i - 1].x << ","
                << prevPositions[i - 1].y << "\n";
      snake.moveSegment(i, prevPositions[i - 1]);
    }
  }

  void checkFoodCollision(Snake &snake) {
    if (!snake.getAliveStatus()) {
      std::cout << "[" << ++callIndex
                << "] checkFoodCollision: Snake is dead, skipping check\n";
      return;
    }

    // Проверяем коллизии с обычной едой
    for (auto &food : mFoodArray) {
      if (snake.getPosition() == food.getPosition()) {
        std::cout << "[" << ++callIndex
                  << "] checkFoodCollision: Snake ate regular food at "
                  << snake.getPosition().x << "," << snake.getPosition().y
                  << "\n";
        snake.grow();
        snake.grow(); // Обычная еда дает +2 к длине
        std::cout << "[" << ++callIndex
                  << "] checkFoodCollision: Snake length after regular food: "
                  << snake.getLength() << "\n";
        food.regeneratePosition();
        // Проверяем, чтобы новая позиция не совпадала с другой едой
        while (isPositionOccupiedByFood(food.getPosition(), food)) {
          std::cout << "[" << ++callIndex
                    << "] checkFoodCollision: Regenerating food position\n";
          food.regeneratePosition();
        }
      }
    }

    // Проверяем коллизию с движущейся едой
    if (snake.getPosition() == mMovingFood.getPosition()) {
      std::cout << "[" << ++callIndex
                << "] checkFoodCollision: Snake ate moving food at "
                << snake.getPosition().x << "," << snake.getPosition().y
                << "\n";
      snake.grow();
      snake.grow();
      snake.grow(); // Движущаяся еда дает +3 к длине
      std::cout << "[" << ++callIndex
                << "] checkFoodCollision: Snake length after moving food: "
                << snake.getLength() << "\n";
      mMovingFood.regeneratePosition();
      while (isPositionOccupiedByFood(mMovingFood.getPosition(), mMovingFood)) {
        std::cout
            << "[" << ++callIndex
            << "] checkFoodCollision: Regenerating moving food position\n";
        mMovingFood.regeneratePosition();
      }
    }
  }

  bool isPositionOccupiedByFood(const sf::Vector2f &pos,
                                SnakeFood &_food) const {
    // Проверяем совпадение с обычной едой
    for (const auto &food : mFoodArray) {
      if (food.getPosition() == pos && &food != &_food) {
        std::cout << "[" << ++callIndex
                  << "] isPositionOccupiedByFood: Position occupied by food\n";
        return true;
      }
    }
    // Проверяем совпадение с движущейся едой
    if (mMovingFood.getPosition() == pos) {
      return true;
    }
    return false;
  }

  void checkCollisions(Snake &snake, const Snake &otherSnake) {
    // Проверка столкновения с границами поля
    sf::Vector2f pos = snake.getPosition();
    std::cout << "[" << ++callIndex
              << "] checkCollisions: Checking snake at position " << pos.x
              << "," << pos.y << "\n";

    if (!worldIsCycled) {
      if (pos.x < 0 || pos.x >= myConst::app_window_size_x || pos.y < 0 ||
          pos.y >= myConst::app_window_size_y) {
        std::cout << "[" << ++callIndex
                  << "] checkCollisions: Snake hit wall\n";
        snake.setAliveStatus(false);
        return;
      }
    } else {
      // Циклическое перемещение
      if (pos.x < 0)
        pos.x = myConst::app_window_size_x - myConst::cell_size;
      if (pos.x >= myConst::app_window_size_x)
        pos.x = 0;
      if (pos.y < 0)
        pos.y = myConst::app_window_size_y - myConst::cell_size;
      if (pos.y >= myConst::app_window_size_y)
        pos.y = 0;
      snake.moveSegment(0, pos);
    }

    // Проверка столкновения с собственным телом
    for (size_t i = 1; i < snake.getLength(); ++i) {
      if (snake.getPosition() == snake.getSegmentPosition(i)) {
        std::cout << "[" << ++callIndex
                  << "] checkCollisions: Snake hit itself\n";
        snake.setAliveStatus(false);
        return;
      }
    }

    // Проверка столкновения с другой змейкой
    for (size_t i = 0; i < otherSnake.getLength(); ++i) {
      if (snake.getPosition() == otherSnake.getSegmentPosition(i)) {
        std::cout << "[" << ++callIndex
                  << "] checkCollisions: Snake hit other snake\n";
        snake.setAliveStatus(false);
        return;
      }
    }
  }

  sf::Vector2f findClosestFood(const sf::Vector2f &pos) const {
    float minDist = std::numeric_limits<float>::max();
    sf::Vector2f closestPos;

    // Проверяем обычную еду
    for (const auto &food : mFoodArray) {
      float dist = calculateDistance(pos, food.getPosition());
      if (dist < minDist) {
        minDist = dist;
        closestPos = food.getPosition();
      }
    }

    // Проверяем движущуюся еду (с учетом приоритета)
    float distToMoving = calculateDistance(pos, mMovingFood.getPosition()) /
                         1.5f; // Приоритет движущейся еды
    if (distToMoving < minDist) {
      closestPos = mMovingFood.getPosition();
    }

    return closestPos;
  }

  float calculateDistance(const sf::Vector2f &a, const sf::Vector2f &b) const {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
  }

  void initializeFood() {
    mFoodArray.resize(FOOD_COUNT);
    // Генерируем начальные позиции для всей еды
    for (size_t i = 0; i < FOOD_COUNT; ++i) {
      mFoodArray[i].regeneratePosition();
      // Проверяем, чтобы позиции еды не совпадали
      bool needRegenerate;
      do {
        needRegenerate = false;
        // Проверяем коллизии с уже размещенной едой
        for (size_t j = 0; j < i; ++j) {
          if (mFoodArray[i].getPosition() == mFoodArray[j].getPosition()) {
            needRegenerate = true;
            mFoodArray[i].regeneratePosition();
            break;
          }
        }
        // Проверяем коллизию с движущейся едой
        if (mFoodArray[i].getPosition() == mMovingFood.getPosition()) {
          needRegenerate = true;
          mFoodArray[i].regeneratePosition();
        }
      } while (needRegenerate);
    }
  }

  std::vector<std::vector<sf::RectangleShape>> tiles;
  Snake mPlayerSnake;
  BotSnake mBotSnake;
  std::vector<SnakeFood> mFoodArray;
  MovingFood mMovingFood;
  const bool worldIsCycled = false;

public:
  World()
      : mPlayerSnake(Snake({myConst::app_window_size_x / 3.f,
                            myConst::app_window_size_y / 2.f},
                           sf::Color::Green)),
        mBotSnake(BotSnake({2 * myConst::app_window_size_x / 3.f,
                            myConst::app_window_size_y / 2.f},
                           sf::Color::Blue)) {
    // Создаем сетку тайлов
    int rows = myConst::app_window_size_y / myConst::cell_size;
    int cols = myConst::app_window_size_x / myConst::cell_size;

    tiles.resize(rows);
    for (int i = 0; i < rows; i++) {
      tiles[i].resize(cols);
      for (int j = 0; j < cols; j++) {
        tiles[i][j].setSize({myConst::cell_size, myConst::cell_size});
        tiles[i][j].setPosition(
            {j * myConst::cell_size, i * myConst::cell_size});
        tiles[i][j].setFillColor(sf::Color(60, 65, 70));
        tiles[i][j].setOutlineColor(sf::Color(20, 25, 20));
        tiles[i][j].setOutlineThickness(-1);
      }
    }

    srand(time(nullptr));

    // Инициализируем еду
    initializeFood();
  }

  static void resetCallIndex() { callIndex = 0; }

  void update() {
    resetCallIndex(); // Сбрасываем счетчик в начале каждого обновления
    std::cout << "\n[" << ++callIndex
              << "] === Starting new update cycle ===\n";

    // Обновляем позицию движущейся еды
    mMovingFood.move();

    // Временно отключаем движение бота для отладки
    // mBotSnake.makeDecision(mFoodArray, mMovingFood, mPlayerSnake);

    // Двигаем змей
    moveSnake(mPlayerSnake);
    // moveSnake(mBotSnake);  // Временно отключаем движение

    // Сначала проверяем столкновения со стенами и друг с другом
    checkCollisions(mPlayerSnake, mBotSnake);
    // checkCollisions(mBotSnake, mPlayerSnake);  // Отключаем для отладки

    // Только после этого проверяем столкновения с едой
    checkFoodCollision(mPlayerSnake);
    // checkFoodCollision(mBotSnake);  // Отключаем для отладки
  }

  void setPlayerDirection(Direction newDir) {
    if (isValidDirection(newDir, mPlayerSnake)) {
      mPlayerSnake.setDirection(newDir);
    }
  }

  Snake &getPlayerSnake() { return mPlayerSnake; }
  BotSnake &getBotSnake() { return mBotSnake; }
  bool isGameOver() const { return !mPlayerSnake.getAliveStatus(); }
  bool isBotAlive() const { return mBotSnake.getAliveStatus(); }

  void reset() {
    // Пересоздаем змеек на их начальных позициях
    mPlayerSnake = Snake(
        {myConst::app_window_size_x / 3.f, myConst::app_window_size_y / 2.f},
        sf::Color::Green);
    mBotSnake = BotSnake({2 * myConst::app_window_size_x / 3.f,
                          myConst::app_window_size_y / 2.f},
                         sf::Color::Blue);

    // Заново инициализируем еду
    initializeFood();

    std::cout << "[World] Game reset\n";
  }
};

int World::callIndex = 0; // Инициализируем статический счетчик

#endif /* WORLD */
