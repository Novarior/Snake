#include "gamesrc/SnakeWorld.hpp"

int main() {
  setenv("NSInputAnalytics_Enabled", "0", 1);

  sf::RenderWindow mWindow(
      sf::VideoMode({myConst::app_window_size_x, myConst::app_window_size_y}),
      myConst::app_name);

  sf::Clock mClock;
  sf::Clock tickClock; // Отдельные часы для системы тиков
  float deltaTime = 0.0f;
  float tickAccumulator = 0.0f; // Аккумулятор времени для тиков
  mWindow.setFramerateLimit(120);
  mWindow.setVerticalSyncEnabled(true);

  sf::Font mFont(
      std::filesystem::path(ApplicationsFunctions::get_resources_dir())
          .append(myConst::font)
          .string());

  sf::Text mText(mFont, "Hello World", 14);
  mText.setPosition({5, 5});
  mText.setFillColor(sf::Color::White);
  mText.setOutlineColor(sf::Color::Black);
  mText.setOutlineThickness(1);

  // create world
  World mWorld;
  FPS mfps;

  // main loop
  while (mWindow.isOpen()) {
    // update delta time
    deltaTime = mClock.restart().asSeconds();
    tickAccumulator += deltaTime; // Накапливаем время
    mfps.update();

    // Обработка тиков
    while (tickAccumulator >= myConst::tick_interval) {
      // Обновление игровой логики
      mWorld.update();
      tickAccumulator -= myConst::tick_interval;
    }

    while (const std::optional event = mWindow.pollEvent()) {
      if (event->is<sf::Event::KeyPressed>()) {
        switch (event->getIf<sf::Event::KeyPressed>()->code) {
        case sf::Keyboard::Key::Escape:
          mWindow.close();
          break;
        case sf::Keyboard::Key::W:
        case sf::Keyboard::Key::Up:
          mWorld.setPlayerDirection(Direction::UP);
          break;
        case sf::Keyboard::Key::S:
        case sf::Keyboard::Key::Down:
          mWorld.setPlayerDirection(Direction::DOWN);
          break;
        case sf::Keyboard::Key::A:
        case sf::Keyboard::Key::Left:
          mWorld.setPlayerDirection(Direction::LEFT);
          break;
        case sf::Keyboard::Key::D:
        case sf::Keyboard::Key::Right:
          mWorld.setPlayerDirection(Direction::RIGHT);
          break;
        case sf::Keyboard::Key::R:
          mWorld.reset();
          break;
        default:
          break;
        }
      }

      if (event->is<sf::Event::Closed>())
        mWindow.close();
    }

    mText.setString(
        "Debug Info:\n"
        "Delta Time: " +
        std::to_string(deltaTime) +
        "\n"
        "FPS: " +
        std::to_string(mfps.getFPS()) +
        "\n"
        "\nPlayer Snake:\n"
        "Length: " +
        std::to_string(mWorld.getPlayerSnake().getLength()) +
        "\n"
        "Alive: " +
        std::to_string(mWorld.getPlayerSnake().getAliveStatus()) +
        "\n"
        "Position: (" +
        std::to_string(mWorld.getPlayerSnake().getPosition().x) + ", " +
        std::to_string(mWorld.getPlayerSnake().getPosition().y) +
        ")\n"
        "\nBot Snake:\n"
        "Length: " +
        std::to_string(mWorld.getBotSnake().getLength()) +
        "\n"
        "Alive: " +
        std::to_string(mWorld.getBotSnake().getAliveStatus()) +
        "\n"
        "Position: (" +
        std::to_string(mWorld.getBotSnake().getPosition().x) + ", " +
        std::to_string(mWorld.getBotSnake().getPosition().y) + ")");

    mWindow.clear(sf::Color::Black);

    mWindow.draw(mWorld);
    mWindow.draw(mText);

    mWindow.display();
  }

  return 0;
}