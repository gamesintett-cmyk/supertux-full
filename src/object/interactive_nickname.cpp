#include "object/interactive_nickname.hpp"

#include "supertux/moving_object.hpp"
#include "supertux/resources.hpp"
#include "video/drawing_context.hpp"

InteractiveNickname::InteractiveNickname(MovingObject* target, const std::string& text) :
  m_target(target),
  m_pos(0.0f, 0.0f),
  m_text(text),
  m_time_left(60.0f)
{
}

void
InteractiveNickname::update(float dt_sec)
{
  m_time_left -= dt_sec;

  if (m_time_left <= 0.0f || m_target == nullptr || !m_target->is_valid())
  {
    remove_me();
    return;
  }

  const Vector target_pos = m_target->get_pos();
  m_pos = Vector(target_pos.x, target_pos.y - 90.0f);
  m_pos.x -= static_cast<float>(m_text.size()) * 8.0f;
}

void
InteractiveNickname::draw(DrawingContext& context)
{
  context.color().draw_text(Resources::normal_font, m_text, m_pos, ALIGN_LEFT, LAYER_OBJECTS + 10, Color::WHITE);
}