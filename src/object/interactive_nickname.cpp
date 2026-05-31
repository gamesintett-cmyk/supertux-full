#include "object/interactive_nickname.hpp"

#include "supertux/resources.hpp"
#include "video/drawing_context.hpp"

InteractiveNickname::InteractiveNickname(const Vector& pos, const std::string& text) :
  m_pos(pos),
  m_text(text),
  m_time_left(30.0f)
{
  m_pos.x -= static_cast<float>(m_text.size()) * 8.0f;
}

void
InteractiveNickname::update(float dt_sec)
{
  m_time_left -= dt_sec;

  if (m_time_left <= 0.0f)
  {
    remove_me();
  }
}

void
InteractiveNickname::draw(DrawingContext& context)
{
  context.color().draw_text(Resources::normal_font, m_text, m_pos, ALIGN_LEFT, LAYER_OBJECTS + 10, Color::WHITE);
}