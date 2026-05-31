#ifndef HEADER_SUPERTUX_OBJECT_INTERACTIVE_NICKNAME_HPP
#define HEADER_SUPERTUX_OBJECT_INTERACTIVE_NICKNAME_HPP

#include "supertux/game_object.hpp"
#include "math/vector.hpp"

#include <string>

class InteractiveNickname final : public GameObject
{
public:
  InteractiveNickname(const Vector& pos, const std::string& text);

  void update(float dt_sec) override;
  void draw(DrawingContext& context) override;

private:
  Vector m_pos;
  std::string m_text;
  float m_time_left;
};

#endif