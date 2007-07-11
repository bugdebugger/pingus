//  $Id: bomber.cxx,v 1.37 2003/10/22 11:11:23 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <math.h>
#include "../debug.hpp"
#include "../globals.hpp"
#include "../col_map.hpp"
#include "../display/scene_context.hpp"
#include "../pingu.hpp"
#include "../pingu_enums.hpp"
#include "../ground_map.hpp"
#include "../world.hpp"
#include "../resource.hpp"
#include "../particles/pingu_particle_holder.hpp"
#include "../colliders/pingu_collider.hpp"
#include "../movers/linear_mover.hpp"
#include "bomber.hpp"

namespace Actions {

Bomber::Bomber (Pingu* p)
  : PinguAction(p),
    particle_thrown(false),
    sound_played(false),
    gfx_exploded(false),
    colmap_exploded(false),
    bomber_radius(Resource::load_collision_mask("other/bomber_radius")),
    explo_surf(Resource::load_sprite("pingus/player" + pingu->get_owner_str() + "/explo"))
{
  sprite.load(Direction::LEFT,  "pingus/player" + pingu->get_owner_str() + "/bomber/left");
  sprite.load(Direction::RIGHT, "pingus/player" + pingu->get_owner_str() + "/bomber/right");
}

void
Bomber::on_successfull_apply ()
{
  WorldObj::get_world()->play_sound("ohno", pingu->get_pos ());
}

void
Bomber::draw (SceneContext& gc)
{
  if (sprite[pingu->direction].get_current_frame() >= 13 && !gfx_exploded)
    {
      gc.color().draw (explo_surf, Vector3f(pingu->get_x () - 32, pingu->get_y () - 48));
      gfx_exploded = true;
    }

  gc.color().draw(sprite[pingu->direction], pingu->get_pos ());
}

void
Bomber::update ()
{
  sprite.update ();

  Movers::LinearMover mover(WorldObj::get_world(), pingu->get_pos());

  Vector3f velocity = pingu->get_velocity();

  // Move the Pingu
  mover.update(velocity, Colliders::PinguCollider(pingu_height));

  pingu->set_pos(mover.get_pos());

  // If the Bomber hasn't 'exploded' yet and it has hit Water or Lava
  if (sprite[pingu->direction].get_current_frame() <= 9 && (rel_getpixel(0, -1) == Groundtype::GP_WATER
      || rel_getpixel(0, -1) == Groundtype::GP_LAVA))
    {
      pingu->set_action(Actions::Drown);
      return;
    }

  // If the Bomber hasn't 'exploded' yet and it has hit the ground too quickly
  if (sprite[pingu->direction].get_current_frame () <= 9 && rel_getpixel(0, -1) != Groundtype::GP_NOTHING
      && velocity.y > deadly_velocity)
    {
      pingu->set_action(Actions::Splashed);
      return;
    }

  if (sprite[pingu->direction].get_current_frame () > 9 && !sound_played) {
    WorldObj::get_world()->play_sound("plop", pingu->get_pos ());
    sound_played = true;
  }

  // Throwing particles
  if (sprite[pingu->direction].get_current_frame () > 12 && !particle_thrown)
    {
      particle_thrown = true;
      WorldObj::get_world()->get_pingu_particle_holder()->add_particle(static_cast<int>(pingu->get_x()),
                                                                       static_cast<int>(pingu->get_y()) - 5);
    }


  if (sprite[pingu->direction].get_current_frame () >= 13 && !colmap_exploded)
    {
      colmap_exploded = true;
      WorldObj::get_world()->remove(bomber_radius,
                                                  static_cast<int>(pingu->get_x () - (bomber_radius.get_width()/2)),
                                                  static_cast<int>(pingu->get_y () - 16 - (bomber_radius.get_width()/2)));
    }

  // The pingu explode
  if (sprite[pingu->direction].is_finished ())
    {
      pingu->set_status(PS_DEAD);
    }
}

} // namespace Actions

/* EOF */
