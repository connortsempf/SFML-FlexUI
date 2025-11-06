#pragma once
#include "Base/Component.hpp"




///////////////////////////
// Label Component Class //
///////////////////////////

namespace SFUI {

    class Label : public Component {

        public:
            SFUI::Prop::Style::Label labelStyle;

        public:
            Label() = default;
            Label(const SFUI::String& componentID);
            Label(const SFUI::String& componentID, const SFUI::PropGroup::Label& labelPropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
            SFUI::SharedPointer<SFUI::Font> getFont();
            SFUI::Float getTextSize();
            SFUI::FloatRect getTextBounds();
            SFUI::Vector2f getCharacterPosition(SFUI::Size charIndex);

        private:
            static const SFUI::Float VERTICAL_CENTER_OFFSET_FACTOR;
            static const SFUI::Float BOTTOM_OFFSET_FACTOR;

        private:
            SFUI::Text textObject;

        private:
            SFUI::Void computeTextCore();
            SFUI::Void computeTextStyles();
            SFUI::Void computeTextLayout();
    };
}
