//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pingus/pingu_action.hpp"

#include "pingus/collision_map.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_enums.hpp"
#include "pingus/action_name.hpp"
#include "pingus/world.hpp"
#include "pingus/worldobj.hpp"

PinguAction::PinguAction (Pingu* p)
  : pingu (p)
{
}

PinguAction::~PinguAction ()
{
}

// Checks if the pingu action needs to catch another pingu (needed for
// example by the blocker)
bool
PinguAction::need_catch ()
{
  return false;
}

Vector3f
PinguAction::get_center_pos() const
{
  return pingu->get_pos() + Vector3f(0, -16);
}

// Wrapper around the colmap, to get the pixels infront of the pingu,
// from is current position
int
PinguAction::rel_getpixel (int x, int y)
{
  // FIXME: Inline me
  return WorldObj::get_world()->get_colmap()->getpixel(static_cast<int>(pingu->get_x() + static_cast<float>((x * pingu->direction))),
                                                       static_cast<int>(pingu->get_y() - static_cast<float>(y)));
}

char
PinguAction::get_persistent_char ()
{
  assert(!"This is not a persitent action!");
  return '-';
}

bool
PinguAction::head_collision_on_walk (int x, int y)
{
  int pixel = rel_getpixel(x, y + pingu_height);

  if (pixel != Groundtype::GP_NOTHING && !(pixel & Groundtype::GP_BRIDGE))
    return true;

  return false;
}

std::string
PinguAction::get_name () const
{
  return ActionName::to_screenname(get_type());
}

ActionType
PinguAction::get_activation_mode(ActionName::Enum action_name)
{
  switch(action_name)
  {
    case ActionName::CLIMBER:
      return WALL_TRIGGERED;

    case ActionName::FLOATER:
      return FALL_TRIGGERED;

    default:
      return INSTANT;
  }
}

/* EOF */
