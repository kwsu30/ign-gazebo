/*
 * Copyright (C) 2018 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#include "ServerPrivate.hh"

#include <sdf/Root.hh>
#include <sdf/World.hh>

#include <ignition/common/Console.hh>

#include <ignition/gazebo/SystemPluginPtr.hh>

#include "SimulationRunner.hh"

using namespace ignition;
using namespace gazebo;

//////////////////////////////////////////////////
ServerPrivate::ServerPrivate()
{
  // Add the signal handler
  this->sigHandler.AddCallback(
      std::bind(&ServerPrivate::OnSignal, this, std::placeholders::_1));
}

//////////////////////////////////////////////////
ServerPrivate::~ServerPrivate()
{
  this->Stop();
  if (this->runThread.joinable())
  {
    this->running = false;
    this->runThread.join();
  }
}

//////////////////////////////////////////////////
void ServerPrivate::OnSignal(int _sig)
{
  igndbg << "Server received signal[" << _sig  << "]\n";
  this->Stop();
}

/////////////////////////////////////////////////
void ServerPrivate::Stop()
{
  for (std::unique_ptr<SimulationRunner> &runner : this->simRunners)
  {
    runner->Stop();
  }
}

/////////////////////////////////////////////////
bool ServerPrivate::Run(const uint64_t _iterations,
    std::optional<std::condition_variable *> _cond)
{
  this->runMutex.lock();
  this->running = true;
  if (_cond)
    _cond.value()->notify_all();
  this->runMutex.unlock();

  bool result = true;

  // Minor performance tweak. In many situations there will only be one
  // simulation runner, and we can avoid using the thread pool.
  if (this->simRunners.size() == 1)
  {
    result = this->simRunners[0]->Run(_iterations);
  }
  else
  {
    for (std::unique_ptr<SimulationRunner> &runner : this->simRunners)
    {
      this->workerPool.AddWork([&runner, &_iterations] ()
        {
          runner->Run(_iterations);
        });
    }

    // Wait for the runner to complete.
    result = this->workerPool.WaitForResults();
  }

  this->running = false;
  return result;
}

//////////////////////////////////////////////////
void ServerPrivate::CreateEntities(const sdf::Root &_root)
{
  // Create a simulation runner for each world.
  for (uint64_t worldIndex = 0; worldIndex < _root.WorldCount(); ++worldIndex)
  {
    auto world = _root.WorldByIndex(worldIndex);
    auto element = world->Element();

    std::vector<SystemPluginPtr> systems;

    if (element->HasElement("plugin"))
    {
      sdf::ElementPtr pluginElem = element->GetElement("plugin");
      while (pluginElem)
      {
        auto system = this->systemManager.LoadPlugin(pluginElem);
        if (system)
        {
          systems.push_back(system.value());
        }
        pluginElem = pluginElem->GetNextElement("plugin");
      }
    }

    this->simRunners.push_back(std::make_unique<SimulationRunner>(
          _root.WorldByIndex(worldIndex), systems));
  }
}