/*
    Copyright 2021 XITRIX

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#pragma once

#include <borealis/core/application.hpp>
#include <borealis/core/bind.hpp>
#include <borealis/core/box.hpp>
#include <borealis/views/cells/cell_detail.hpp>
#include <borealis/views/recycler.hpp>

namespace brls
{

class BooleanCell : public DetailCell
{
  public:
    BooleanCell();

    void init(std::string title, bool isOn, std::function<void(bool)> callback);

    void setOn(bool on, bool animated = true);
    bool isOn()
    {
        return state;
    }

    Event<bool>* getEvent()
    {
        return &event;
    }

    static View* create();

  private:
    bool state;
    float baseDetailTextSize;

    Animatable scale = 1;
#ifdef __SWITCH2__
    // Eased slide parameter of the Switch 2 pill toggle (0 = off, 1 = on).
    Animatable switchP = 0;
#endif
    Event<bool> event;

    void updateUI();
    void scaleTick();

#ifdef __SWITCH2__
    void draw(NVGcontext* vg, float x, float y, float width, float height, Style style, FrameContext* ctx) override;
#endif
};

} // namespace brls
