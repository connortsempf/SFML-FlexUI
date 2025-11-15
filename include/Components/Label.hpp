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
            Label(Label&&) = default;
            Label& operator=(Label&&) = default;
            Label(SFUI::String componentID);
            template<typename... Children>
            Label(SFUI::String componentID, SFUI::PropGroup::Label labelPropGroup, Children&&... children) :
                Component(std::move(componentID), std::move(labelPropGroup.layout), std::move(labelPropGroup.style)),
                labelStyle(std::move(labelPropGroup.labelStyle)),
                textObject(*(labelStyle.font), labelStyle.text, labelStyle.textSize) {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);

            }
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
            SFUI::Prop::Style::Label dirtyLabelStyle;

        private:
            SFUI::Void computeTextCore();
            SFUI::Void computeTextStyles();
            SFUI::Void computeTextLayout();
    };
}
