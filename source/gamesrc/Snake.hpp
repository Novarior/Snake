#ifndef SNAKE
#define SNAKE

#include "../libs/helper.hpp"
#include "../libs/myConst.hpp"
#include <iostream>

class Snake : public sf::Drawable {
private:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    for (const auto &segment : segments) {
      target.draw(segment, states);
    }
  }

  std::vector<sf::RectangleShape> segments;
  Direction direction = Direction::IDLE;
  bool isAlive = true;
  sf::Color baseColor = sf::Color::Green; // Базовый цвет змейки

  void updateColors() {
    if (segments.empty()) {
      std::cout << "[Snake:" << segments.size()
                << "] updateColors: Empty snake!\n";
      return;
    }

    std::cout << "[Snake:" << segments.size()
              << "] updateColors: Updating colors for " << segments.size()
              << " segments\n";

    // Задаем цвет головы
    segments[0].setFillColor(baseColor);

    // Обновляем цвета всех остальных сегментов
    for (size_t i = 1; i < segments.size(); ++i) {
      float ratio = static_cast<float>(i) / (segments.size() - 1);
      sf::Color segmentColor(
          static_cast<std::uint8_t>(baseColor.r +
                                    (255 - baseColor.r) * ratio), // Red
          static_cast<std::uint8_t>(baseColor.g * (1 - ratio)),   // Green
          static_cast<std::uint8_t>(baseColor.b +
                                    (255 - baseColor.b) * ratio) // Blue
      );
      segments[i].setFillColor(segmentColor);
    }
  }

public:
  Snake(sf::Vector2f _position = {0, 0}, sf::Color color = sf::Color::Green)
      : baseColor(color) {
    // Инициализация головы змейки
    sf::RectangleShape head;
    head.setPosition(_position);
    head.setSize({myConst::cell_size, myConst::cell_size});
    head.setFillColor(baseColor);
    head.setOutlineThickness(-1.0f);
    head.setOutlineColor(sf::Color::White);
    segments.push_back(head);

    // Добавляем еще 3 сегмента
    for (int i = 1; i < 4; ++i) {
      sf::RectangleShape segment;
      segment.setSize({myConst::cell_size, myConst::cell_size});
      segment.setOutlineThickness(-1.0f);
      segment.setOutlineColor(sf::Color::White);
      segment.setPosition(_position - sf::Vector2f(myConst::cell_size * i, 0));
      segments.push_back(segment);
    }

    // Инициализируем цвета
    updateColors();
  }

  void grow() {
    if (segments.empty()) {
      std::cout << "[Snake:" << segments.size()
                << "] grow: Cannot grow empty snake!\n";
      return;
    }

    std::cout << "[Snake:" << segments.size() << "] grow: Growing snake\n";

    // Создаем новый сегмент
    sf::RectangleShape newSegment;
    newSegment.setSize({myConst::cell_size, myConst::cell_size});

    // Позиционируем новый сегмент за последним существующим сегментом
    sf::Vector2f lastPos = segments.back().getPosition();
    std::cout << "[Snake:" << segments.size() << "] grow: Adding segment at "
              << lastPos.x << "," << lastPos.y << "\n";

    newSegment.setPosition(lastPos);

    // Устанавливаем базовые параметры сегмента
    newSegment.setOutlineThickness(-1.0f);
    newSegment.setOutlineColor(sf::Color::White);

    segments.push_back(newSegment);

    // Обновляем цвета всех сегментов
    updateColors();
    std::cout << "[Snake:" << segments.size() << "] grow: New length is "
              << segments.size() << "\n";
  }

  size_t getLength() const { return segments.size(); }
  bool getAliveStatus() const { return isAlive; }
  void setAliveStatus(bool status) { isAlive = status; }
  sf::Vector2f getPosition() const { return segments[0].getPosition(); }
  sf::FloatRect getHeadBounds() const { return segments[0].getGlobalBounds(); }
  Direction getCurrentDirection() const { return direction; }
  void setDirection(Direction newDir) { direction = newDir; }

  sf::Vector2f getSegmentPosition(size_t index) const {
    if (index < segments.size()) {
      return segments[index].getPosition();
    }
    return sf::Vector2f(0, 0);
  }

  void moveSegment(size_t index, sf::Vector2f newPosition) {
    if (index < segments.size()) {
      segments[index].setPosition(newPosition);
    }
  }
};

#endif /* SNAKE */
