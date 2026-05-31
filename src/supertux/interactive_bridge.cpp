// SuperTux Interactive Bridge MVP
// Command file: %TEMP%/supertux_interactive_commands.txt on Windows.

#include "supertux/interactive_bridge.hpp"

#include <algorithm>
#include <cctype>
#include <exception>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "math/vector.hpp"
#include "object/floating_text.hpp"
#include "object/player.hpp"
#include "supertux/sector.hpp"

namespace
{
  std::filesystem::path command_file_path()
  {
    return std::filesystem::temp_directory_path() / "supertux_interactive_commands.txt";
  }

  std::string trim(std::string s)
  {
    auto not_space = [](unsigned char c) { return !std::isspace(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
    return s;
  }

  std::map<std::string, std::string> parse_command_line(const std::string& line)
  {
    std::map<std::string, std::string> out;
    std::stringstream ss(line);
    std::string part;
    while (std::getline(ss, part, ';'))
    {
      const auto eq = part.find('=');
      if (eq == std::string::npos) continue;
      std::string key = trim(part.substr(0, eq));
      std::string value = trim(part.substr(eq + 1));
      if (!key.empty()) out[key] = value;
    }
    return out;
  }

  int to_int(const std::string& value, int fallback)
  {
    try {
      return std::stoi(value);
    } catch (...) {
      return fallback;
    }
  }

  std::string safe_nickname(const std::string& nickname)
  {
    if (nickname.empty()) return "viewer";
    if (nickname.size() <= 32) return nickname;
    return nickname.substr(0, 32);
  }

  bool is_allowed_enemy(const std::string& id)
  {
    static const std::vector<std::string> allowed = {
      "snowball", "bouncingsnowball", "captainsnowball", "flyingsnowball",
      "kamikazesnowball", "mriceblock", "mrbomb", "goldbomb", "spiky", "sspiky",
      "jumpy", "snail", "tarantula", "toad", "owl", "plant", "mole",
      "fish-chasing", "fish-harmless", "fish-jumping", "fish-swimming",
      "flame", "livefire", "ghoul", "granito", "granito_big", "granito_giant",
      "crystallo", "rcrystallo", "scrystallo", "willowisp", "zeekling"
    };
    return std::find(allowed.begin(), allowed.end(), id) != allowed.end();
  }

  bool is_allowed_powerup(const std::string& type)
  {
    static const std::vector<std::string> allowed = {
      "egg", "fire", "ice", "air", "earth", "star", "oneup", "flip", "mints", "coffee", "herring"
    };
    return std::find(allowed.begin(), allowed.end(), type) != allowed.end();
  }

  void add_floating_name(Sector& sector, const Vector& pos, const std::string& nickname)
  {
    if (nickname.empty()) return;
    sector.add_object(std::make_unique<FloatingText>(Vector(pos.x, pos.y - 40.0f), safe_nickname(nickname)));
  }

  void spawn_enemy(Sector& sector, const std::string& enemy, int quantity, const std::string& nickname)
  {
    if (!is_allowed_enemy(enemy)) return;
    auto players = sector.get_players();
    if (players.empty() || players[0] == nullptr) return;

    const Vector base = players[0]->get_pos();
    quantity = std::clamp(quantity, 1, 25);

    for (int i = 0; i < quantity; ++i)
    {
      const float offset_x = 120.0f + static_cast<float>((i % 5) * 38);
      const float offset_y = -30.0f - static_cast<float>((i / 5) * 32);
      const Vector pos(base.x + offset_x, base.y + offset_y);

      try {
        sector.add_object(enemy, "", pos.x, pos.y, "auto", "");
        // add_floating_name(sector, pos, nickname);
      } catch (...) {
        // Ignore invalid objects so one bad command does not crash the game.
      }
    }
  }

  void spawn_powerup(Sector& sector, const std::string& type, int quantity, const std::string& nickname)
  {
    if (!is_allowed_powerup(type)) return;
    auto players = sector.get_players();
    if (players.empty() || players[0] == nullptr) return;

    const Vector base = players[0]->get_pos();
    quantity = std::clamp(quantity, 1, 10);

    for (int i = 0; i < quantity; ++i)
    {
      const float offset_x = 80.0f + static_cast<float>((i % 5) * 34);
      const float offset_y = -70.0f - static_cast<float>((i / 5) * 28);
      const Vector pos(base.x + offset_x, base.y + offset_y);
      const std::string data = " (type \"" + type + "\")";

      try {
        sector.add_object("powerup", "", pos.x, pos.y, "auto", data);
        // add_floating_name(sector, pos, nickname);
      } catch (...) {
      }
    }
  }
}

namespace InteractiveBridge
{
  void process_pending_commands(Sector& sector)
  {
    const auto path = command_file_path();
    if (!std::filesystem::exists(path)) return;

    std::ifstream in(path);
    if (!in.good()) return;

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line))
    {
      line = trim(line);
      if (!line.empty()) lines.push_back(line);
      if (lines.size() >= 50) break;
    }
    in.close();

    std::error_code ec;
    std::filesystem::remove(path, ec);

    for (const auto& cmdline : lines)
    {
      const auto cmd = parse_command_line(cmdline);
      const auto action_it = cmd.find("action");
      if (action_it == cmd.end()) continue;

      const std::string action = action_it->second;
      const std::string nickname = cmd.count("nickname") ? cmd.at("nickname") : "";
      const int quantity = cmd.count("quantity") ? to_int(cmd.at("quantity"), 1) : 1;

      if (action == "spawn")
      {
        const std::string enemy = cmd.count("enemy") ? cmd.at("enemy") : "snowball";
        spawn_enemy(sector, enemy, quantity, nickname);
      }
      else if (action == "powerup")
      {
        const std::string type = cmd.count("type") ? cmd.at("type") : "fire";
        spawn_powerup(sector, type, quantity, nickname);
      }
    }
  }
}
