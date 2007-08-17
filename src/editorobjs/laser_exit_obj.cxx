//  $Id: laser_exit_obj.cxx,v 1.11 2003/04/19 10:23:18 torangan Exp $
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
#include "laser_exit_obj.hxx"
#include "../editor/editor_view.hxx"
#include "../worldobjsdata/laser_exit_data.hxx"

namespace EditorObjs {

LaserExitObj::LaserExitObj (const WorldObjsData::LaserExitData& data_)
  : data(new WorldObjsData::LaserExitData(data_)),
    frame(0)
{
  data->pos.z = -100;
  sprite = Sprite("Traps/laser_exit", "traps");
  //sprite.set_align_center_bottom ();
  pos_ref = &data->pos;
}

LaserExitObj::~LaserExitObj ()
{
  delete data;
}

EditorObjLst
LaserExitObj::create (const Vector& pos)
{
  WorldObjsData::LaserExitData newdata;
  newdata.pos = pos;
  return EditorObjLst(1, new LaserExitObj(newdata));
}

EditorObj*
LaserExitObj::duplicate ()
{
  return new LaserExitObj(*data);
}

void
LaserExitObj::write_xml (std::ostream& xml)
{
  data->write_xml(xml);
}

void
LaserExitObj::draw (EditorNS::EditorView* view)
{
  view->draw(sprite, data->pos, frame);
}

std::string
LaserExitObj::status_line ()
{
  char str[64];
  snprintf (str, 64, "LaserExitObj: %4.2fx%4.2fx%4.2f", data->pos.x, data->pos.y, data->pos.z);
  return str;
}

} // namespace EditorObjs

/* EOF */