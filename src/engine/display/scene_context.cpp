//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "engine/display/scene_context.hpp"

#include "engine/display/framebuffer.hpp"

class SceneContextImpl
{
public:
  DrawingContext color;

  Rect cliprect;
  bool use_cliprect;

  SceneContextImpl() :
    color(),
    cliprect(),
    use_cliprect(false)
  {
  }

  SceneContextImpl(const Rect& rect) :
    color(rect),
    cliprect(),
    use_cliprect(false)
  {
  }
};

SceneContext::SceneContext() :
  impl(new SceneContextImpl())
{
}

SceneContext::SceneContext(const Rect& rect) :
  impl(new SceneContextImpl(rect))
{
}

SceneContext::~SceneContext()
{
}

DrawingContext&
SceneContext::color()
{
  return impl->color;
}

/** Translate the drawing context */
void
SceneContext::translate(int x, int y)
{
  impl->color.translate(x, y);
}

void
SceneContext::push_modelview()
{
  impl->color.push_modelview();
}

void
SceneContext::pop_modelview()
{
  impl->color.pop_modelview();
}

void
SceneContext::reset_modelview()
{
  impl->color.reset_modelview();
}

void
SceneContext::set_rect(const Rect& rect)
{
  impl->color.set_rect(rect);
}

void
SceneContext::set_cliprect(const Rect& rect)
{
  impl->cliprect = rect;
  impl->use_cliprect = true;
}

void
SceneContext::render(Framebuffer& fb, const Rect& rect)
{
  // Render all buffers
  // FIXME: Render all to pbuffer for later combining of them
  if (impl->use_cliprect)
  {
    fb.push_cliprect(impl->cliprect);
    impl->color.render(fb, rect);
    fb.pop_cliprect();
  }
  else
  {
    impl->color.render(fb, rect);
  }
}

void
SceneContext::clear()
{
  impl->color.clear();
}

SceneContextDrawingRequest::SceneContextDrawingRequest(SceneContext* sc_, const Vector2i& pos_, float z_)
  : DrawingRequest(pos_, z_),
    sc(sc_)
{
}

SceneContextDrawingRequest::~SceneContextDrawingRequest()
{
  //delete sc;
}

void
SceneContextDrawingRequest::render(Framebuffer& fb, const Rect& rect)
{
  sc->render(fb, rect);
}

/* EOF */
