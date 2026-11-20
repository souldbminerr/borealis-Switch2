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

#include "borealis/views/cells/cell_bool.hpp"

#include <borealis/core/i18n.hpp>

using namespace brls::literals;

#ifdef __SWITCH2__
// Switch 2 toggle: a 48x26 pill (on #0066EE / off #555555) with a white
// knob, track colour and knob position driven by one eased parameter
// (p in [0,1]), matching the system UI and the Ultrahand S2 reference.
static void s2DrawToggle(NVGcontext* vg, float x, float y, float p, float alpha = 1.0f)
{
    if (p < 0.0f)
        p = 0.0f;
    else if (p > 1.0f)
        p = 1.0f;

    constexpr float TRACK_W = 48.0f;
    constexpr float TRACK_H = 26.0f;
    const float radius = TRACK_H / 2.0f;
    const float knobR  = radius - 3.0f;

    const NVGcolor on  = nvgRGB(0x00, 0x66, 0xEE);
    const NVGcolor off = nvgRGB(0x55, 0x55, 0x55);
    const NVGcolor track = nvgRGBAf(off.r + (on.r - off.r) * p, off.g + (on.g - off.g) * p,
        off.b + (on.b - off.b) * p, alpha);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, TRACK_W, TRACK_H, radius);
    nvgFillColor(vg, track);
    nvgFill(vg);

    const float cx = x + radius + p * (TRACK_W - 2.0f * radius);
    nvgBeginPath(vg);
    nvgCircle(vg, cx, y + radius, knobR);
    nvgFillColor(vg, nvgRGBAf(1.0f, 1.0f, 1.0f, alpha));
    nvgFill(vg);
}
#endif

namespace brls
{

BooleanCell::BooleanCell()
{
    baseDetailTextSize = detail->getFontSize();
    setOn(false, false);
    this->registerClickAction([this](View* view) {
        this->setOn(!state);
        this->event.fire(state);
        return true;
    });
}

void BooleanCell::init(std::string title, bool isOn, std::function<void(bool)> callback)
{
    this->title->setText(title);
    setOn(isOn, false);
    getEvent()->subscribe(callback);
}

void BooleanCell::setOn(bool on, bool animated)
{
    this->state = on;

#ifdef __SWITCH2__
    // Switch 2 style: slide the pill toggle (150 ms, eased) instead of the
    // Switch 1 ON/OFF text pulse. A rapid re-toggle reverses mid-travel.
    if (animated)
    {
        float from = this->switchP;
        this->switchP.stop();
        this->switchP.reset(from);
        this->switchP.addStep(state ? 1.0f : 0.0f, 150, tweeny::easing::quadraticInOut);
        this->switchP.start();
    }
    else
    {
        this->switchP.stop();
        this->switchP.reset(state ? 1.0f : 0.0f);
    }
    updateUI();
#else
    if (animated)
    {
        scale.stop();
        scale.reset(1);
        scale.addStep(0.8f, 100, tweeny::easing::quadraticOut);
        scale.addStep(1, 100, tweeny::easing::quadraticIn);
        scale.setTickCallback([this]
            { this->scaleTick(); });
        scale.setEndCallback([this](bool finished)
            { if (finished) updateUI(); });
        scale.start();
    }
    else
    {
        updateUI();
    }
#endif
}

void BooleanCell::updateUI()
{
#ifdef __SWITCH2__
    // Switch 2 style: the sliding pill replaces the ON/OFF text.
    detail->setText("");
#else
    Theme theme = Application::getTheme();
    detail->setText(state ? "hints/on"_i18n : "hints/off"_i18n);
    detail->setTextColor(state ? theme["brls/list/listItem_value_color"] : theme["brls/text_disabled"]);
#endif
}

#ifdef __SWITCH2__
void BooleanCell::draw(NVGcontext* vg, float x, float y, float width, float height, Style style, FrameContext* ctx)
{
    DetailCell::draw(vg, x, y, width, height, style, ctx);

    // Pill on the right, vertically centred, aligned to the detail label's
    // right edge (where the ON/OFF text used to sit).
    constexpr float TRACK_W = 48.0f;
    constexpr float TRACK_H = 26.0f;
    Rect detailFrame = this->detail->getFrame();
    float px = detailFrame.getMaxX() - TRACK_W;
    float py = y + (height - TRACK_H) / 2.0f;
    s2DrawToggle(vg, px, py, this->switchP);
}
#endif

void BooleanCell::scaleTick()
{
    detail->setFontSize(baseDetailTextSize * scale);
}

View* BooleanCell::create()
{
    return new BooleanCell();
}

} // namespace brls
