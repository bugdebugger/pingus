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

#include "engine/input/xinput/xinput_device.hpp"

#include "engine/input/xinput/xinput_driver.hpp"

namespace Input {

#define INVALID_EVENT_TYPE -1

XInputDevice::XInputDevice(XInputDriver* owner_, XDeviceInfo* info) :
  owner(owner_),
  name(info->name),
  absolute(false),
  buttons(),
  axis(),
  num_keys   (0),
  mouse_pos(),
  time_at_last_press(),
  last_press_id(),
  motion_type        (INVALID_EVENT_TYPE),
  button_press_type  (INVALID_EVENT_TYPE),
  button_release_type(INVALID_EVENT_TYPE),
  key_press_type     (INVALID_EVENT_TYPE),
  key_release_type   (INVALID_EVENT_TYPE),
  proximity_in_type  (INVALID_EVENT_TYPE),
  proximity_out_type (INVALID_EVENT_TYPE)
{
  get_info(info);

  if (!register_events(owner->get_syswminfo().info.x11.display, info, name.c_str(), True))
  {
    std::cout << "debug: CL_InputDeviceXInput: Couldn't find device: " << name << std::endl;
  }
}

int
XInputDevice::register_events(Display* dpy,
                              XDeviceInfo* info,
                              const char* dev_name,
                              Bool handle_proximity)
{
  int             number = 0; /* number of events registered */
  XEventClass     event_list[7];
  int             i;
  XDevice         *device;
  Window          root_win;
  unsigned long   screen;
  XInputClassInfo *ip;

  screen   = DefaultScreen(dpy);
  root_win = RootWindow(dpy, screen);

  device = XOpenDevice(dpy, info->id);

  if (!device)
  {
    fprintf(stderr, "unable to open device %s\n", dev_name);
    return 0;
  }

  if (device->num_classes > 0)
  {
    for (ip = device->classes, i=0; i<info->num_classes; ip++, i++)
    {
      switch (ip->input_class)
      {
        case KeyClass:
          DeviceKeyPress  (device, key_press_type,   event_list[number]); number++;
          DeviceKeyRelease(device, key_release_type, event_list[number]); number++;
          break;

        case ButtonClass:
          DeviceButtonPress  (device, button_press_type,   event_list[number]); number++;
          DeviceButtonRelease(device, button_release_type, event_list[number]); number++;
          break;

        case ValuatorClass:
          DeviceMotionNotify(device, motion_type, event_list[number]); number++;
          if (handle_proximity) {
            ProximityIn (device, proximity_in_type,  event_list[number]); number++;
            ProximityOut(device, proximity_out_type, event_list[number]); number++;
          }
          break;

        case FeedbackClass:
          std::cout << "Error: XInputDevice: register_events: unhandled class: FeedbackClass" << std::endl;
          break;

        case ProximityClass:
          std::cout << "Error: XInputDevice: register_events: unhandled class: ProximityClass" << std::endl;
          break;

        case FocusClass:
          std::cout << "Error: XInputDevice: register_events: unhandled class: FocusClass" << std::endl;
          break;

        case OtherClass:
          std::cout << "Error: XInputDevice: register_events: unhandled class: OtherClass" << std::endl;
          break;

        default:
          std::cout << "Error: XInputDevice: register_events: unknown class: " << ip->input_class << std::endl;
          break;
      }
    }

    if (XSelectExtensionEvent(dpy, root_win, event_list, number)) {
      fprintf(stderr, "error selecting extended events\n");
      return 0;
    }
  }

  return number;
}

void
XInputDevice::get_info(XDeviceInfo* info)
{
  printf("\"%s\"\tid=%ld\t[%s]\n", info->name, info->id,
         (info->use == IsXExtensionDevice) ? "XExtensionDevice" :
         ((info->use == IsXPointer) ? "XPointer" : "XKeyboard"));

  XAnyClassPtr any = (XAnyClassPtr)(info->inputclassinfo);
  std::cout << "Info->num_classes: " << info->num_classes << std::endl;
  for (int i = 0; i < info->num_classes; ++i)
  {
    switch (any->c_class)
    {
      case KeyClass:
      {
        XKeyInfoPtr k = (XKeyInfoPtr)any;

        printf("\tNum_keys is %d\n", k->num_keys);
        printf("\tMin_keycode is %d\n", k->min_keycode);
        printf("\tMax_keycode is %d\n", k->max_keycode);

        num_keys = k->num_keys;
      }
      break;

      case ButtonClass:
      {
        XButtonInfoPtr b = (XButtonInfoPtr)any;
        printf("\tNum_buttons is %d\n", b->num_buttons);

        buttons.resize(b->num_buttons, false);
      }
      break;

      case ValuatorClass:
      {
        XValuatorInfoPtr v = (XValuatorInfoPtr)any;
        XAxisInfoPtr a = (XAxisInfoPtr) ((char*)v +
                                         sizeof (XValuatorInfo));

        printf("\tNum_axes is %d\n", v->num_axes);
        printf("\tMode is %s\n", (v->mode == Absolute) ? "Absolute" : "Relative");
        printf("\tMotion_buffer is %ld\n", v->motion_buffer);

        absolute = (v->mode == Absolute);

        for (int j = 0; j < v->num_axes; ++j, ++a)
        {
          printf("\tAxis %d :\n", j);
          printf("\t\tMin_value is %d\n", a->min_value);
          printf("\t\tMax_value is %d\n", a->max_value);
          printf ("\t\tResolution is %d\n", a->resolution);

          axis.push_back(AxisInfo(a->min_value, a->max_value, a->resolution));
        }
      }
      break;

      case FeedbackClass:
        std::cout << "Error: XInputDevice: get_info: unhandled class: FeedbackClass" << std::endl;
        break;

      case ProximityClass:
        std::cout << "Error: XInputDevice: get_info: unhandled class: ProximityClass" << std::endl;
        break;

      case FocusClass:
        std::cout << "Error: XInputDevice: get_info: unhandled class: FocusClass" << std::endl;
        break;

      case OtherClass:
        std::cout << "Error: XInputDevice: get_info: unhandled class: OtherClass" << std::endl;
        break;

      default:
        std::cout << "Error: XInputDevice: get_info: unknown class: " << any->c_class << std::endl;
        break;
    }

    std::cout << "AnyClass ptr: " << any->length << std::endl;
    any = (XAnyClassPtr)((char*)any + any->length);
  }
}

} // namespace Input

/* EOF */
