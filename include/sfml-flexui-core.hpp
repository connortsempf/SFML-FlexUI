#pragma once
#include <sfml-flexui-types.hpp>
#include <sfml-flexui-base.hpp>




///////////////////////////////////////////
// SFML-FlexUI Container Component Class //
///////////////////////////////////////////

namespace SFUI {

    class Container : public Component {

        public:
            Container() = default;
            Container(const SFUI::String& componentID);
            Container(const SFUI::String& componentID, const SFUI::PropGroup::Container& containerPropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
    };
}




///////////////////////////////////////
// SFML-FlexUI Label Component Class //
///////////////////////////////////////

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
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
            SFUI::String getText();
            SFUI::SharedPointer<SFUI::Font> getFont();
            SFUI::Float getTextSize();
            SFUI::UnsignedInt32 getTextStyle();
            SFUI::Float getLetterSpacing();
            SFUI::Float getLineSpacing();
            SFUI::Float getOutlineThickness();
            SFUI::String getTextAlignHorizontal();
            SFUI::String getTextAlignVertical();
            SFUI::Vector2f getTextOffset();
            SFUI::Color getTextColor();
            SFUI::Color getTextOutlineColor();
            SFUI::FloatRect getTextBounds();
            SFUI::Vector2f getCharacterPosition(SFUI::Size charIndex);

        private:
            static const SFUI::Float VERTICAL_CENTER_OFFSET_FACTOR;
            static const SFUI::Float BOTTOM_OFFSET_FACTOR;

        private:
            SFUI::Text textObject;
            SFUI::ComputedProp::Style::Label computedLabelStyle;

        private:
            SFUI::Void computeTextCore();
            SFUI::Void computeTextStyling();
            SFUI::Void computeTextAlign();
            SFUI::Void computeTextColors();
            SFUI::Void computeTextLayout();
    };
}




////////////////////////////////////////
// SFML-FlexUI Button Component Class //
////////////////////////////////////////

namespace SFUI {
    
    class Button : public Component {
        
        public:
            SFUI::Prop::Style::Button buttonStyle;
            SFUI::Prop::Behavior::Button buttonBehavior;

        public:
            Button() = default;
            Button(const SFUI::String& componentID);
            Button(const SFUI::String& componentID, const SFUI::PropGroup::Button& buttonPropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
            SFUI::Color getHoveredFillColor();
            SFUI::Color getHoveredBorderColor();
            SFUI::Color getPressedFillColor();
            SFUI::Color getPressedBorderColor();
            SFUI::Color getDisabledFillColor();
            SFUI::Color getDisabledBorderColor();
            SFUI::Float getFocusWidth();
            SFUI::Float getFocusOffset();
            SFUI::Vector4f getFocusCornerRadius();
            SFUI::Color getFocusFillColor();
            SFUI::Vector4f getToolTipPadding();
            SFUI::Vector4f getToolTipCornerRadius();
            SFUI::String getToolTipText();
            SFUI::SharedPointer<SFUI::Font> getToolTipFont();
            SFUI::Float getToolTipTextSize();
            SFUI::Color getToolTipFillColor();
            SFUI::Color getToolTipTextColor();
            
        private:
            static const SFUI::Time DOUBLE_PRESS_GAP_MS;
            static const SFUI::Time TOOL_TIP_THRESHOLD_MS;

        private:
            SFUI::Bool isDisabled = false;
            SFUI::Bool isHovered = false;
            SFUI::Bool isFocused = false;
            SFUI::Bool isLeftPressed = false;
            SFUI::Bool isRightPressed = false;
            SFUI::Bool isMiddlePressed = false;
            SFUI::Bool isShowingToolTip = false;
            SFUI::Vector2f previousHoverPosition;
            SFUI::Vector2i previousPressPosition;
            SFUI::Clock doublePressClock;
            SFUI::Time previousPressTime;
            SFUI::Clock toolTipClock;
            SFUI::Time toolTipTime;
            SFUI::Container focus;
            SFUI::Label toolTip;
            SFUI::ComputedProp::Style::Button computedButtonStyle;

        private:
            SFUI::Void computeDynamicColors();
            SFUI::Void computeFocusWidth();
            SFUI::Void computeFocusOffset();
            SFUI::Void computeToolTipLifetime();
            SFUI::Void computeComposedComponents();
    };
}




/////////////////////////////////////////
// SFML-FlexUI Graphic Component Class //
/////////////////////////////////////////

namespace SFUI {
    
    class Graphic : public Component {
        
        public:
            SFUI::Prop::Style::Graphic graphicStyle;
            SFUI::Prop::Behavior::Graphic graphicBehavior;

        public:
            Graphic() = default;
            Graphic(const SFUI::String& componentID);
            Graphic(const SFUI::String& componentID, const SFUI::PropGroup::Graphic& graphicPropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
            SFUI::String getGraphicAlign();
            SFUI::Vector2f getOriginalTextureSize();

        private:
            enum class LoadType { PRE_LOAD, SELF_LOAD };
            enum class LoadState { PRE_LOAD_UNLOADED, PRE_LOAD_LOADED, SELF_LOAD_UNLOADED, SELF_LOAD_LOADED, SELF_LOAD_ERROR };

        private:
            SFUI::Texture graphicSource;
            SFUI::Sprite graphic;
            SFUI::Graphic::LoadType loadType = SFUI::Graphic::LoadType::PRE_LOAD;
            SFUI::Graphic::LoadState loadState = SFUI::Graphic::LoadState::PRE_LOAD_UNLOADED;
            SFUI::ComputedProp::Style::Graphic computedGraphicStyle;
        
        private:
            SFUI::Void computeGraphicSource();
            SFUI::Void computeGraphicAlign();
            SFUI::Void computeGraphic();
    };
}




//////////////////////////////////////////////////
// SFML-FlexUI Scroll Container Component Class //
//////////////////////////////////////////////////

namespace SFUI {
    
    class ScrollContainer : public Component {
        
        public:
            SFUI::Prop::Style::ScrollContainer scrollContainerStyle;
            SFUI::Prop::Behavior::ScrollContainer scrollContainerBehavior;

        public:
            ScrollContainer() = default;
            ScrollContainer(const SFUI::String& componentID);
            ScrollContainer(const SFUI::String& componentID, const SFUI::PropGroup::ScrollContainer& scrollContainerPropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& renderTarget);
            SFUI::String getScrollDirection();
            SFUI::Float getScrollSpeedFactor();
            SFUI::Bool getUsingScrollBar();
            SFUI::String getScrollBarAlign();
            SFUI::Float getTrackOnAxisSize();
            SFUI::Float getTrackOffAxisSize();
            SFUI::Vector4f getTrackCornerRadius();
            SFUI::Color getTrackFillColor();
            SFUI::Color getTrackHoveredFillColor();
            SFUI::Color getTrackPressedFillColor();
            SFUI::Vector4f getThumbCornerRadius();
            SFUI::Color getThumbFillColor();
            SFUI::Color getThumbHoveredFillColor();
            SFUI::Color getThumbPressedFillColor();

        private:
            SFUI::Bool isHovered = false;
            SFUI::Bool isTrackHovered = false;
            SFUI::Bool isThumbHovered = false;
            SFUI::Bool isTrackPressed = false;
            SFUI::Bool isThumbPressed = false;
            SFUI::Vector2f scrollOffset = {0.0f, 0.0f};
            SFUI::Vector2f maxScrollOffset = {0.0f, 0.0f};
            SFUI::Vector2f contentSize;
            SFUI::Float dragStartPosition;
            SFUI::ComputedProp::Style::ScrollContainer computedScrollContainerStyle;

        private:
            SFUI::Void computeAlignPrimary();
            SFUI::Void computeScrollDirection();
            SFUI::Void computeScrollSpeedFactor();
            SFUI::Void computeChildrenScrollPosition();
            SFUI::Void computeMaxScrollOffset();
    };
}




////////////////////////////////////////////
// SFML-FlexUI Text Field Component Class //
////////////////////////////////////////////

namespace SFUI {
    
    class TextField : public Component {

        public:
            SFUI::Prop::Style::TextField textFieldStyle;
            SFUI::Prop::Behavior::TextField textFieldBehavior;

        public:
            TextField() = default;
            TextField(const SFUI::String& componentID);
            TextField(const SFUI::String& componentID, const SFUI::PropGroup::TextField& textFieldPropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
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
            SFUI::Bool isFocused = false;
            SFUI::Bool isHovered = false;
            SFUI::Bool caretVisible = false;
            SFUI::UnsignedInt caretColumnIndex = 0;
            SFUI::UnsignedInt caretRowIndex = 0;
            SFUI::Clock caretClock;
            SFUI::SharedPointer<SFUI::Button> background;
            SFUI::SharedPointer<SFUI::Label> inputText;
            SFUI::SharedPointer<SFUI::Container> caret;
            SFUI::ComputedProp::Style::TextField computedTextFieldStyle;

        private:
            SFUI::Void computeLineMode();
            SFUI::Void computeTextInset();
            SFUI::Void computePlaceholderText();
            SFUI::Void computeCaretShape();
            SFUI::Void computeCaretBlinkTiming();
            SFUI::Void computeCaretLifetime();
            SFUI::Void computeCaretFillColor();
            SFUI::Void computeCaretLayout();
            SFUI::Void computeComposedComponents();
            SFUI::Void insertText(const char32_t newAppendedText);
            SFUI::Void editText(const SFUI::Event::KeyPressed* keyPressedEvent);
            SFUI::UnsignedInt getCharacterGroup(const char32_t character);
    };
}
