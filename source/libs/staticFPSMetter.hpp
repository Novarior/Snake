#ifndef STATICFPSMETTER
#define STATICFPSMETTER
#ifndef FPS_HPP
#define FPS_HPP

#include <sfml/System.hpp>

class FPS {
public:
  /// @brief Constructor with initialization.
  ///
  FPS() {
    mFrame = 0;
    mFps = 0;
  }

  /// @brief Update the frame count.
  void update() {
    if (mClock.getElapsedTime().asSeconds() >= 1.f) {
      mFps = mFrame;
      mFrame = 0;
      mClock.restart();
    }

    ++mFrame;
  }
  void reset() {
    mFrame = 0;
    mFps = 0;
    mClock.restart();
  }

  /// @brief Get the current FPS count.
  /// @return FPS count.
  const unsigned int getFPS() { return mFps; }

private:
  unsigned int mFrame = 0;
  unsigned int mFps = 0;
  sf::Clock mClock = sf::Clock();
};
#endif // FPS_HPP

#endif /* STATICFPSMETTER */
