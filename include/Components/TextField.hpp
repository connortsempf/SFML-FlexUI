#pragma once
#include "Base/Component.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Container.hpp"




////////////////////////////////////////////
// SFML-FlexUI Text Field Component Class //
////////////////////////////////////////////

namespace SFUI {
    
    class TextField : public Component {

        public:
            SFUI::Prop::Style::TextField textFieldStyle;
            SFUI::Prop::State::TextField textFieldState;
            SFUI::Prop::Behavior::TextField textFieldBehavior;

        public:
            TextField() = default;
            TextField(TextField&&) = default;
            TextField& operator=(TextField&&) = default;
            TextField(SFUI::String componentID);
            template<typename... Children>
            TextField(SFUI::String componentID, SFUI::PropGroup::TextField textFieldPropGroup, Children&&... children) :
                Component(std::move(componentID), std::move(textFieldPropGroup.layout), std::move(textFieldPropGroup.style)),
                textFieldStyle(std::move(textFieldPropGroup.textFieldStyle)),
                textFieldState(std::move(textFieldPropGroup.textFieldState)),
                textFieldBehavior(std::move(textFieldPropGroup.textFieldBehavior)),
                background(componentID + "_Background"),
                inputText(componentID + "_InputText"),
                caret(componentID + "_Caret") {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);

            }
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
        
        private:
            static const SFUI::Float CENTER_TEXT_OFFSET_FACTOR;
            static const SFUI::Float CARET_LINE_VERTICAL_OFFSET_FACTOR;
            static const SFUI::Float CARET_BOX_VERTICAL_OFFSET_FACTOR;
            static const SFUI::Float CARET_UNDERLINE_VERTICAL_OFFSET_FACTOR;
            static const SFUI::Float CARET_LINE_WIDTH_FACTOR;
            static const SFUI::Float CARET_BOX_WIDTH_FACTOR;
            static const SFUI::Float CARET_UNDERLINE_WIDTH_FACTOR;
            static const SFUI::Float CARET_UNDERLINE_HEIGHT_FACTOR;
            static const SFUI::String CTRL_WHITESPACE_GROUP;
            static const SFUI::String CTRL_ALPHANUMERIC_GROUP;
            static const SFUI::String CTRL_SYMBOL_GROUP;

        private:
            SFUI::Bool caretVisible = false;
            SFUI::UnsignedInt caretIndex = 0;
            SFUI::Vector2f dynamicTextOffset = {0.0f, 0.0f};
            SFUI::Clock caretClock;
            SFUI::Button background;
            SFUI::Label inputText;
            SFUI::Container caret;
            SFUI::Bool dirtyEvent = false;
            SFUI::Prop::Style::TextField dirtyTextFieldStyle;
            SFUI::Prop::State::TextField dirtyTextFieldState;
            SFUI::ComputedProp::Style::TextField computedTextFieldStyle;

        private:
            SFUI::Void computeLineMode();
            SFUI::Void computeBackground();
            SFUI::Void computeInputText();
            SFUI::Void computeCaret();
            SFUI::Void computeDynamicTextOffset();
            SFUI::Void insertText(const char32_t newAppendedText);
            SFUI::Void editText(const SFUI::Event::KeyPressed* keyPressedEvent);
            SFUI::UnsignedInt getCharacterGroup(const char32_t character);
    };
}
