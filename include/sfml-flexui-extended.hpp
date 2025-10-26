#pragma once
#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


////////////////////////////////////////////
// SFML-FlexUI Text Field Component Class //
////////////////////////////////////////////

namespace SFUI {
    
    class TextField : public Component {

        public:
            SFUI::Prop::ButtonStyle backgroundStyle;
            SFUI::Prop::LabelStyle inputTextStyle;
            SFUI::Prop::Style caretStyle;
            SFUI::Prop::TextFieldStyle textFieldStyle;
            SFUI::Prop::ButtonBehavior backgroundBehavior;
            SFUI::Prop::TextFieldBehavior textFieldBehavior;

        public:
            TextField() = default;
            TextField(const SFUI::String& componentID);
            TextField(const SFUI::String& componentID, const SFUI::PropGroup::TextField& textFieldPropGroup);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
        
        private:
            static const SFUI::Float CARET_LINE_WIDTH_FACTOR;
            static const SFUI::Float CARET_BOX_WIDTH_FACTOR;
            static const SFUI::Float CARET_UNDERLINE_WIDTH_FACTOR;
            static const SFUI::Float CARET_UNDERLINE_HEIGHT_FACTOR;
            static const SFUI::String CTRL_WHITESPACE_GROUP;
            static const SFUI::String CTRL_ALPHANUMERIC_GROUP;
            static const SFUI::String CTRL_SYMBOL_GROUP;

        private:
            SFUI::Bool isDisabled;
            SFUI::Bool isPlaceheld;
            SFUI::Bool isFocused;
            SFUI::Bool isHovered;
            SFUI::UnsignedInt caretColumnIndex = 0;
            SFUI::UnsignedInt caretRowIndex = 0;
            SFUI::Clock caretClock;
            SFUI::SharedPointer<SFUI::Button> background;
            SFUI::SharedPointer<SFUI::Label> inputText;
            SFUI::SharedPointer<SFUI::Container> caret;
            SFUI::ComputedProp::TextFieldStyle computedTextFieldStyle;

        private:
            SFUI::Void computeComposedProps();
            SFUI::Void computePlaceholderText();
            SFUI::Void computeCaretShape();
            SFUI::Void computeCaretBlinkTiming();
            SFUI::Void computeCaretLifetime();
            SFUI::Void computeCaretLayout();
            SFUI::Void computeComposedComponents();
            
            SFUI::Void insertText(const char32_t newAppendedText);
            SFUI::Void editText(const SFUI::Event::KeyPressed* keyPressedEvent);
            SFUI::UnsignedInt getCharacterGroup(const char32_t character);
    };
}