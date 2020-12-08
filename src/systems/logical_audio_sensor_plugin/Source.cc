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

#include "Source.hh"

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>

namespace ignition
{
namespace gazebo
{
// Inline bracket to help doxygen filtering.
inline namespace IGNITION_GAZEBO_VERSION_NAMESPACE {
namespace systems
{
namespace logical_audio
{

  const std::unordered_map<std::string, AttenuationFunction>
    Source::kAttFuncMap {
      {"linear", AttenuationFunction::Linear}};

  const std::unordered_map<std::string, AttenuationShape>
    Source::kAttShapeMap {
      {"sphere", AttenuationShape::Sphere}};

  //////////////////////////////////////////////////
  Source::Source(const unsigned int _id,
                 const ignition::math::Vector3d &_position,
                 const AttenuationFunction _attenuationFunc,
                 const AttenuationShape _attenuationShape,
                 const float _innerRadius,
                 const float _falloffDistance,
                 const float _volumeLevel,
                 const bool _playing,
                 const unsigned int _playDuration) :
    id(_id),
    position(_position),
    attenuationFunc(_attenuationFunc),
    attenuationShape(_attenuationShape),
    innerRadius(_innerRadius),
    falloffDistance(_falloffDistance),
    volumeLevel(_volumeLevel),
    playing(_playing),
    playDuration(_playDuration)
  {
  }

  //////////////////////////////////////////////////
  Source::Source(const unsigned int _id,
                 const ignition::math::Vector3d &_position,
                 const std::string &_attenuationFunc,
                 const std::string &_attenuationShape,
                 const float _innerRadius,
                 const float _falloffDistance,
                 const float _volumeLevel,
                 const bool _playing,
                 const unsigned int _playDuration) :
    id(_id),
    position(_position),
    innerRadius(_innerRadius),
    falloffDistance(_falloffDistance),
    volumeLevel(_volumeLevel),
    playing(_playing),
    playDuration(_playDuration)
  {
    this->SetAttenuationShape(_attenuationShape);
    this->SetAttenuationFunction(_attenuationFunc);
  }

  //////////////////////////////////////////////////
  void Source::SetAttenuationFunction(const std::string &_attenuationFunc)
  {
    std::string caseInsensitiveFunc;
    std::transform(_attenuationFunc.begin(), _attenuationFunc.end(),
        caseInsensitiveFunc.begin(), ::tolower);

    auto iter = this->kAttFuncMap.find(caseInsensitiveFunc);
    if (iter != this->kAttFuncMap.end())
      this->attenuationFunc = iter->second;
    else
      this->attenuationFunc = AttenuationFunction::Undefined;
  }

  //////////////////////////////////////////////////
  void Source::SetAttenuationShape(const std::string &_attenuationShape)
  {
    std::string caseInsensitiveShape;
    std::transform(_attenuationShape.begin(), _attenuationShape.end(),
        caseInsensitiveShape.begin(), ::tolower);

    auto iter = this->kAttShapeMap.find(caseInsensitiveShape);
    if (iter != this->kAttShapeMap.end())
      this->attenuationShape = iter->second;
    else
      this->attenuationShape = AttenuationShape::Undefined;
  }
}
}
}
}
}
