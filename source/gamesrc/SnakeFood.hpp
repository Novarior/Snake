#ifndef SNAKEFOOD
#define SNAKEFOOD

#include "../libs/helper.hpp"
#include "../libs/myConst.hpp"

class SnakeFood : public sf::Drawable {
protected:
  sf::RectangleShape food;
  sf::Vector2f position;

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(food, states);
  }

public:
  SnakeFood() {
    food.setSize(sf::Vector2f(myConst::cell_size, myConst::cell_size));
    food.setFillColor(sf::Color::Red);
    regeneratePosition();
  }

  virtual void regeneratePosition() {
    // Генерируем случайную позицию в пределах игрового поля
    int x = (rand() % (myConst::app_window_size_x /
                       static_cast<int>(myConst::cell_size))) *
            static_cast<int>(myConst::cell_size);
    int y = (rand() % (myConst::app_window_size_y /
                       static_cast<int>(myConst::cell_size))) *
            static_cast<int>(myConst::cell_size);
    position = sf::Vector2f(x, y);
    food.setPosition(position);
  }

  sf::Vector2f getPosition() const { return position; }

  virtual ~SnakeFood() = default;
};

#endif /* SNAKEFOOD */
