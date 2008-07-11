//  Pingus - A free Lemmings clone
//  Copyright (C) 2005-2008 Ingo Ruhnke <grumbel@gmx.de>
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

#include "display/framebuffer.hpp"
#include "sprite_description.hpp"
#include "sprite_impl.hpp"

SpriteImpl::SpriteImpl()
{
}

SpriteImpl::SpriteImpl(const SpriteDescription& desc, ResourceModifierNS::ResourceModifier mod)
  : optimized(false),
    finished(false),
    frame(0),
    tick_count(0)
{
  surface = Surface(desc.filename);
  if (mod != ResourceModifierNS::ROT0)
    surface = surface.mod(mod);

  if (!surface)
    {
      std::cout << "Error: Sprite: couldn't load '" << desc.filename << "'" << std::endl;
      surface = Surface(Pathname("images/core/misc/404.png", Pathname::DATA_PATH));
      if (!surface) assert(!"Surface Couldn't find 404");
    }

  frame_pos = desc.frame_pos;

  array = desc.array;

  frame_size.width  = (desc.frame_size.width  == -1) ? surface.get_width()/array.width   : desc.frame_size.width;
  frame_size.height = (desc.frame_size.height == -1) ? surface.get_height()/array.height : desc.frame_size.height;

  frame_delay  = desc.speed;

  loop = desc.loop;
  loop_last_cycle = false;

  offset = calc_origin(desc.origin, frame_size) - desc.offset;
    
}

SpriteImpl::SpriteImpl(const Surface& surface_)
  : surface(surface_),
    optimized(false),
    offset(0,0),
    frame_pos(0,0),
    frame_size(surface.get_width(), surface.get_height()),
    frame_delay(0),
    array(1,1),
    loop(true),
    loop_last_cycle(false),
    finished(false),
    frame(0),
    tick_count(0)
{
}

SpriteImpl::~SpriteImpl()
{
}

void
SpriteImpl::optimize()
{
  surface = surface.optimize();
  optimized = true;
}

void
SpriteImpl::update(float delta)
{
  if (finished)
    return;

  int total_time = frame_delay * (array.width * array.height);
  tick_count += int(delta * 1000.0f);
  if (tick_count >= total_time)
    {
      if (loop)
        {
          loop_last_cycle = true;
          tick_count = tick_count % total_time;
          frame = tick_count / frame_delay;
        }
      else
        {
          finished = true;
        }
    }
  else
    {
      loop_last_cycle = false;
      frame = tick_count / frame_delay;
    }
}

void 
SpriteImpl::render(float x, float y, Framebuffer& fb)
{
  if (!optimized)
    optimize();
  
  fb.draw_surface(surface.get_surface(), 
                  Vector2i(static_cast<int>(x - offset.x), static_cast<int>(y - offset.y)),
                  Rect(frame_pos + Vector2i(frame_size.width  * (frame%array.width),
                                            frame_size.height * (frame/array.width)),
                       frame_size));
}

void
SpriteImpl::restart()
{
  finished = false;
  loop_last_cycle = false;
  frame = 0;
  tick_count = 0;
}

void
SpriteImpl::finish()
{
  finished = true;
}

/* EOF */
