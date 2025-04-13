#include "../source/gamesrc/SnakeWorld.hpp"
#include <gtest/gtest.h>

TEST(SnakeTest, InitialPosition) {
  World world;
  auto snake = world.getSnake();
  EXPECT_EQ(snake.getPosition().x, myConst::app_window_size_x / 2);
  EXPECT_EQ(snake.getPosition().y, myConst::app_window_size_y / 2);
}

TEST(SnakeTest, Movement) {
  World world;
  auto snake = world.getSnake();
  auto initialPos = snake.getPosition();
  snake.move(Direction::Right);
  EXPECT_EQ(snake.getPosition().x, initialPos.x + myConst::cell_size);
}