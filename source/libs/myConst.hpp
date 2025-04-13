#ifndef MYCONST
#define MYCONST

/// constant file
namespace myConst {
// application name
const char app_name[] = "SnakeGame";
const char app_version[] = "0.1.0";
const unsigned app_window_size_x = 1200;
const unsigned app_window_size_y = 800;
const float cell_size = 10; // Размер одной клетки в пикселях

const char font[] = "fonts/Muli-Regular.ttf";

// Интервал между тиками в секундах (8 тиков в секунду вместо 20)
constexpr float tick_interval = 1.0f / 8.0f;

}; // namespace myConst
enum class Direction { IDLE, UP, DOWN, LEFT, RIGHT };
#endif /* MYCONST */
