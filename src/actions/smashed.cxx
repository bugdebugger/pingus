//  $Id: smashed.cxx,v 1.15 2003/10/18 23:17:27 grumbel Exp $
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

#include "../resource.hxx"
#include "../vector.hxx"
#include "../display/drawing_context.hxx"
#include "../pingu.hxx"
#include "smashed.hxx"

namespace Pingus {
namespace Actions {

Smashed::Smashed (Pingu* p)
  : PinguAction(p),
    sound_played(false)
{
  sprite = Resource::load_sprite("pingus/bomber");
}

void
Smashed::draw (DrawingContext& gc)
{
  gc.draw(sprite, pingu->get_pos ());
}

void
Smashed::update()
{
  sprite.update();
  //  pingu->particle->add_pingu_explo(pingu->x_pos, pingu->y_pos - 16);
  if (sprite.is_finished())
    pingu->set_status(PS_DEAD);
}

} // namespace Actions
} // namespace Pingus

/* EOF */
