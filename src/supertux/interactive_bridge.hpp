// SuperTux Interactive Bridge MVP
// Reads commands written by bridge/supertux-bridge.js and spawns objects in the active sector.
#pragma once

class Sector;

namespace InteractiveBridge
{
  void process_pending_commands(Sector& sector);
}
