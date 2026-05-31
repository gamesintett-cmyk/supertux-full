#ifndef HEADER_SUPERTUX_OBJECT_INTERACTIVE_NICKNAME_HPP
#define HEADER_SUPERTUX_OBJECT_INTERACTIVE_NICKNAME_HPP

#include "supertux/game_object.hpp"
#include "math/vector.hpp"

#include <string>

class MovingObject;

class InteractiveNickname final : public GameObject
{
public:
  InteractiveNickname(MovingObject* target, const std::string& text);
  InteractiveNickname(const Vector& pos, const std::string& text);
  void update(float dt_sec) override;
  void draw(DrawingContext& context) override;

private:
  MovingObject* m_target;
  Vector m_pos;
  std::string m_text;
  float m_time_left;
};

#endif