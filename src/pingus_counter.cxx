//  $Id: pingus_counter.cxx,v 1.18 2003/10/21 21:37:06 grumbel Exp $
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

#include <stdio.h>
#include <ClanLib/Display/display.h>
#include <ClanLib/Display/font.h>
#include "gettext.h"
#include "pingus_resource.hxx"
#include "pingus_counter.hxx"
#include "world.hxx"
#include "pingu_holder.hxx"
#include "true_server.hxx"
#include "plf.hxx"
#include "globals.hxx"
#include "fonts.hxx"

namespace Pingus {

PingusCounter::PingusCounter(Server* s)
  : server(s),
    background (PingusResource::load_surface("Buttons/info","core"))
{
  font = Fonts::pingus_small_fix_num;
}

void
PingusCounter::draw(GraphicContext& gc)
{
  char str[128];

  if (!fast_mode)
    background.draw (CL_Display::get_width ()/2 - background.get_width()/2, 0);

  World* world = server->get_world();

  snprintf(str, 128, _("Released:%3d/%-3d   Out:%3d   Saved:%3d/%-3d"),
	   world->get_pingus()->get_number_of_released(),
	   world->get_pingus()->get_number_of_allowed(),
	   world->get_pingus()->get_number_of_alive(),
	   world->get_pingus()->get_number_of_exited(),
	   server->get_plf()->get_number_to_save());

  CL_Font myfont = font;
  myfont.set_alignment(origin_top_center);
  myfont.draw(CL_Display::get_width ()/2,3, str);

  UNUSED_ARG(gc);
}

} // namespace Pingus

/* EOF */
