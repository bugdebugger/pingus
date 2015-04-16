//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "engine/input/xinput/xinput_driver.hpp"

#include "engine/input/xinput/xinput_device.hpp"
#include "pingus/globals.hpp"

namespace Input {

XInputDriver::XInputDriver() :
  sys(),
  devices()
{
  SDL_VERSION(&sys.version); // this is important!
  if (!SDL_GetWMInfo(&sys))
    std::cout << "Fatal Error: Couldn't get SDL_SysWMinfo" << std::endl;

  setup_xinput();
}

XInputDriver::~XInputDriver()
{
}

Bool
XInputDriver::xinput_is_present()
{
  XExtensionVersion* version = XGetExtensionVersion(sys.info.x11.display, INAME);

  if (version && (version != (XExtensionVersion*)NoSuchExtension))
  {
    Bool present = version->present;
    XFree(version);
    return present;
  }
  else
  {
    return False;
  }
}

void
XInputDriver::setup_xinput()
{
  if (!xinput_is_present())
  {
    std::cout << "debug: XInput extentsion not found" << std::endl;
  }
  else
  {
    int num_devices;
    XDeviceInfo* info = XListInputDevices(sys.info.x11.display, &num_devices);
    for(int i = 0; i < num_devices; ++i)
    {
      log_debug("XInputDriver: Device name='%1'", info[i].name);
      // FIXME: Xinput isn't necesarrily a mouse, could be anything
      //if (info[i].use == IsXExtensionDevice)
      // {
      devices.push_back(new XInputDevice(this, &info[i]));
      // }
    }
    XFreeDeviceList(info);
  }
}

void
XInputDriver::update(float delta)
{
}

Button*
XInputDriver::create_button(const FileReader& reader, Control* parent)
{
  return 0;
}

Axis*
XInputDriver::create_axis(const FileReader& reader, Control* parent)
{
  return 0;
}

Scroller*
XInputDriver::create_scroller(const FileReader& reader, Control* parent)
{
  return 0;
}

Pointer*
XInputDriver::create_pointer (const FileReader& reader, Control* parent)
{
  return 0;
}

} // namespace Input

/* EOF */
