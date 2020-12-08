/*
 * Copyright (C) 2020 Open Source Robotics Foundation
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

#ifndef IGNITION_GAZEBO_SYSTEMS_LOGICAL_AUDIO_SENSOR_PLUGIN_HH_
#define IGNITION_GAZEBO_SYSTEMS_LOGICAL_AUDIO_SENSOR_PLUGIN_HH_

#include <memory>

#include <ignition/gazebo/System.hh>

namespace ignition
{
namespace gazebo
{
// Inline bracket to help doxygen filtering.
inline namespace IGNITION_GAZEBO_VERSION_NAMESPACE {
namespace systems
{
  // Forward Declaration
  class LogicalAudioSensorPluginPrivate;

  /// \brief A plugin for trivial audio detection.
  ///
  /// Multiple audio sources and microphones can be created.
  /// After each simulation step, microphones check if audio
  /// was detected by any sources in the world.
  /// No audio is actually played to an audio device
  /// such as speakers - this plugin is meant to check if audio
  /// could theoretically be heard at a certain location or not.
  ///
  /// This system processes the following sdf parameters:
  ///
  /// <audiosource> A new audiosource in the environment.
  /// <audiosource><id> The source ID, which must be >= 0.
  /// <audiosource><position> The position, expressed as "x y z".
  ///   Each position coordinate must be separated by whitespace.
  /// <audiosource><attenuationfunction> The attenuation function.
  ///   Currently, the only valid value is "linear".
  /// <audiosource><attenuationshape> The attenuation shape.
  ///   Currently, the only valid value is "sphere".
  /// <audiosource><innerradius> The inner radius of the attenuation shape.
  ///   This value must be >= 0.0.
  ///   The volume of the source will be <audiosource><volume> at locations
  ///   that have a distance <= inner radius from the source.
  /// <audiosource><falloffdistance> The falloff distance.
  ///   This value must be greater than the value for
  ///   <audiosource><innerradius>.
  ///   This defines the distance from the audio source where the volume is 0.
  /// <audiosource><volumelevel> The volume level emitted from the source.
  ///   This must be a value between 0.0 and 1.0 (representing 0% to 100%).
  /// <audiosource><playing> Whether the source should play immediately or not.
  ///   Use "true" to initiate audio immediately, and "false" otherwise.
  /// <audiosource><playduration> The duration (in seconds) audio is played
  ///   from the source.
  ///   This value must be >= 0.
  ///   A value of 0 means that the source will play for an infinite
  ///   amount of time.
  ///
  /// <microphone> A new microphone in the environment.
  /// <microphone><id> The microphone ID, which must be >= 0.
  /// <microphone><position> The position, expressed as "x y z".
  ///   Each position coordinate must be separated by whitespace.
  /// <microphone><volumedetectionthreshold> The minimum volume level
  ///   the microphone can detect.
  ///   This must be a value between 0.0 and 1.0 (representing 0% to 100%).
  class IGNITION_GAZEBO_VISIBLE LogicalAudioSensorPlugin :
    public System,
    public ISystemConfigure,
    public ISystemPostUpdate
  {
    /// \brief Constructor
    public: LogicalAudioSensorPlugin();

    /// \brief Destructor
    public: ~LogicalAudioSensorPlugin() override;

    /// Documentation inherited
    public: void Configure(const Entity &_entity,
                           const std::shared_ptr<const sdf::Element> &_sdf,
                           EntityComponentManager &_ecm,
                           EventManager &_eventMgr) override;

    /// Documentation inherited
    public: void PostUpdate(const UpdateInfo &_info,
                            const EntityComponentManager &_ecm) override;

    /// \brief Private data pointer
    private: std::unique_ptr<LogicalAudioSensorPluginPrivate> dataPtr;
  };
}
}
}
}

#endif
