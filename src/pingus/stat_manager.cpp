//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "pingus/stat_manager.hpp"

#include "util/log.hpp"
#include "util/file_reader.hpp"
#include "util/file_writer.hpp"
#include "util/string_util.hpp"
#include "util/system.hpp"

StatManager* StatManager::instance_ = 0;

StatManager*
StatManager::instance()
{
  assert(instance_);
  return instance_;
}

std::string
StatManager::get_resname(const std::string& filename)
{
  std::string::size_type pos;
  std::string str;
  pos = filename.rfind("/");
  pos++;
  str = filename.substr(pos);
  pos = 0;
  while ((pos = str.find('.', pos)) != std::string::npos)
  {
    str.replace(pos, 1, 1, '-');
  }
  return str;
}

StatManager::StatManager(const std::string& arg_filename)
  : statfilename(System::get_userdir() + arg_filename),
    stats()
{
  load(statfilename);

  assert(instance_ == 0);
  instance_ = this;
}

StatManager::~StatManager()
{
  flush();
  instance_ = 0;
}

void
StatManager::load(const std::string& filename)
{
  if (!System::exist(filename.c_str()))
  {
    // Create empty file
    save(filename);
  }

  FileReader reader = FileReader::parse(filename);
  if (reader.get_name() != "pingus-stats")
  {
    std::cerr << "Error: " << filename << ": not a (pingus-stats) file" << std::endl;
    return;
  }
  else
  {
    const std::vector<std::string>& section_names = reader.get_section_names();
    for(std::vector<std::string>::const_iterator i = section_names.begin();
        i != section_names.end(); ++i)
    {
      reader.read_string(i->c_str(), stats[*i]);
    }
  }
}

void
StatManager::flush()
{
  save(statfilename);
}

void
StatManager::save(const std::string& filename)
{
  std::ostringstream out;
  FileWriter writer(out);

  writer.begin_mapping("pingus-stats");

  for (Table::iterator i = stats.begin(); i != stats.end(); ++i)
  {
    if (!i->second.empty())
      writer.write_string(i->first.c_str(), i->second);
  }

  writer.end_mapping();

  System::write_file(filename, out.str());
}

bool
StatManager::get_int(const std::string& name, int& value)
{
  std::string str;
  if (get_string(name, str))
    return StringUtil::from_string(str, value);
  else
    return false;
}

bool
StatManager::get_bool(const std::string& name, bool& value)
{
  std::string str;
  if (get_string(name, str))
    return StringUtil::from_string(str, value);
  else
    return false;
}

bool
StatManager::get_string(const std::string& name, std::string& value)
{
  Table::iterator i = stats.find(name);
  if (i == stats.end())
  {
    return false;
  }
  else
  {
    value = i->second;
    return true;
  }
}

void
StatManager::set_string(const std::string& name, const std::string& value)
{
  stats[name] = value;
  flush();
}

void
StatManager::set_int(const std::string& name, int value)
{
  stats[name] = StringUtil::to_string(value);
  flush();
}

void
StatManager::set_bool(const std::string& name, bool value)
{
  stats[name] = StringUtil::to_string(value);
  flush();
}

/* EOF */
