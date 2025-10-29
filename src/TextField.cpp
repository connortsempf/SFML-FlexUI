#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


////////////////////////////////////////////
// SFML-FlexUI Text Field Component Class //
////////////////////////////////////////////


// Static Variables //
const SFUI::Float SFUI::TextField::CARET_LINE_WIDTH_FACTOR = 0.1f;
const SFUI::Float SFUI::TextField::CARET_BOX_WIDTH_FACTOR = 1.0f;
const SFUI::Float SFUI::TextField::CARET_UNDERLINE_WIDTH_FACTOR = 1.0f;
const SFUI::Float SFUI::TextField::CARET_UNDERLINE_HEIGHT_FACTOR = 0.1f;
const SFUI::String SFUI::TextField::CTRL_WHITESPACE_GROUP = " \t";
const SFUI::String SFUI::TextField::CTRL_ALPHANUMERIC_GROUP = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_";
const SFUI::String SFUI::TextField::CTRL_SYMBOL_GROUP = "`~!@#$%^&*()-=+[]{}\\|:;\'\",<.>/?";


/**
 * @brief .
 * 
 * @param .
 */
SFUI::TextField::TextField(const SFUI::String& componentID) :
    Component(componentID),
    textFieldStyle(),
    textFieldBehavior(),
    background(std::make_shared<SFUI::Button>(componentID + "_Background")),
    inputText(std::make_shared<SFUI::Label>(componentID + "_InputText")),
    caret(std::make_shared<SFUI::Container>(componentID + "_Caret"))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::TextField::TextField(const SFUI::String& componentID, const SFUI::PropGroup::TextField& textFieldPropGroup) :
    Component(componentID, textFieldPropGroup.layout, textFieldPropGroup.style),
    textFieldStyle(textFieldPropGroup.textFieldStyle),
    textFieldBehavior(textFieldPropGroup.textFieldBehavior),
    background(std::make_shared<SFUI::Button>(componentID + "_Background")),
    inputText(std::make_shared<SFUI::Label>(componentID + "_InputText")),
    caret(std::make_shared<SFUI::Container>(componentID + "_Caret"))
{}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::TextField::handleEvent(const SFUI::Event& event) {
    background->handleEvent(event);
    inputText->handleEvent(event);
    caret->handleEvent(event);

    // Mouse Button Pressed Event Handling //
    if (const SFUI::Event::MouseButtonPressed* mousePressedEvent = event.getIf<SFUI::Event::MouseButtonPressed>()) {
        sf::Mouse::Button mouseButton = mousePressedEvent->button;
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mousePressedEvent->position.x, mousePressedEvent->position.y);
        SFUI::Bool buttonHovered = isMouseHovered(mousePosition);

        if (isFocused && !buttonHovered) {
            isFocused = false;
            caretVisible = false;
            if (textFieldBehavior.onPressOut) textFieldBehavior.onPressOut(componentID);
            if (textFieldBehavior.onBlur) textFieldBehavior.onBlur(componentID);
        }
    }

    // Key Pressed Event Handling //
    else if (const sf::Event::KeyPressed* keyPressedEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (isFocused) {
            editText(keyPressedEvent);
            if (textFieldBehavior.onKeyPress) textFieldBehavior.onKeyPress(componentID, keyPressedEvent->code);
        }
    }

    // Text Entered Event Handling //
    else if (const sf::Event::TextEntered* textEnteredEvent = event.getIf<sf::Event::TextEntered>()) {
        if (isFocused) insertText(textEnteredEvent->unicode);
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::TextField::update(const SFUI::Vector2u renderTargetSize) {
    this->renderTargetSize = renderTargetSize;
    
    computeAlignDirection();
    computeAlignPrimary();
    computeAlignSecondary();
    computeMargin();
    computeSize();
    computePadding();
    computePosition();
    computeBorderWidth();
    computeCornerRadius();
    computeFillColor();
    computeBorderColor();
    computeChildrenMargin();
    computeChildrenSize();
    computeChildrenPosition();
    updateChildren();

    // Text Field Specific Computation //
    computePlaceholderText();
    computeCaretShape();
    computeCaretBlinkTiming();
    computeCaretLifetime();
    computeCaretFillColor();
    computeCaretLayout();
    computeComposedComponents();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::TextField::draw(SFUI::RenderTarget& renderTarget) {
    background->draw(renderTarget);
    inputText->draw(renderTarget);
    if (isFocused && caretVisible) caret->draw(renderTarget);
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeLineMode() {
    SFUI::String tempLineMode = textFieldStyle.lineMode;
    std::transform(tempLineMode.begin(), tempLineMode.end(), tempLineMode.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempLineMode == "single" || tempLineMode == "multi")
        computedTextFieldStyle.lineMode = tempLineMode;
    else
        computedTextFieldStyle.lineMode = "single";
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeTextInset() {
    computedTextFieldStyle.textInset = resolveUniQuadSubProp(computedLayout.size, textFieldStyle.textInset);
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computePlaceholderText() {
    computedTextFieldStyle.placeholderText = textFieldStyle.placeholderText;
    computedTextFieldStyle.placeholderTextColor = resolveColorSubProp(textFieldStyle.placeholderTextColor);
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeCaretShape() {
    SFUI::String tempShape = textFieldStyle.caretShape;
    std::transform(tempShape.begin(), tempShape.end(), tempShape.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempShape == "line" || tempShape == "box" || tempShape == "underline")
        computedTextFieldStyle.caretShape = tempShape;
    else
        computedTextFieldStyle.caretShape = "line";
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeCaretBlinkTiming() {
    computedTextFieldStyle.caretOnTime = textFieldStyle.caretBlinkTime;
    computedTextFieldStyle.caretOffTime = textFieldStyle.caretBlinkTime / std::clamp(textFieldStyle.caretBlinkRatio, 0.0f, 2.0f);
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeCaretLifetime() {
    if (isFocused) {
        if (caretClock.getElapsedTime().asMilliseconds() < computedTextFieldStyle.caretOnTime) {
            caretVisible = true;
        }
        else if ((caretClock.getElapsedTime().asMilliseconds() > computedTextFieldStyle.caretOnTime &&
            caretClock.getElapsedTime().asMilliseconds() < (computedTextFieldStyle.caretOnTime + computedTextFieldStyle.caretOffTime))) {
                caretVisible = false;
        }
        else if (caretClock.getElapsedTime().asMilliseconds() > (computedTextFieldStyle.caretOnTime + computedTextFieldStyle.caretOffTime)) {
            caretClock.restart();
        }
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeCaretFillColor() {
    computedTextFieldStyle.caretFillColor = resolveColorSubProp(textFieldStyle.caretFillColor);
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeCaretLayout() {
    SFUI::Vector2f computedSize = {0.0f, 0.0f};

    if (computedTextFieldStyle.caretShape == "line")
        computedSize = {inputText->getTextSize() * CARET_LINE_WIDTH_FACTOR, inputText->getTextSize()};
    else if (computedTextFieldStyle.caretShape == "box")
        computedSize = {inputText->getTextSize() * CARET_BOX_WIDTH_FACTOR, inputText->getTextSize()};
    else if (computedTextFieldStyle.caretShape == "underline")
        computedSize = {inputText->getTextSize() * CARET_UNDERLINE_WIDTH_FACTOR, inputText->getTextSize() * CARET_UNDERLINE_HEIGHT_FACTOR};

    caret->layout.width = computedSize.x;
    caret->layout.height = computedSize.y;

    SFUI::Float caretOffset = 0.0f;
    SFUI::Vector2f textStartingPosition = {inputText->getTextBounds().position.x, inputText->getTextBounds().position.y};
    if (caretColumnIndex > 0 && textFieldStyle.text.size() > 0) {
        SFUI::Text tempText(*textFieldStyle.font, textFieldStyle.text.substr(0, caretColumnIndex), inputText->getTextSize());
        caretOffset = tempText.getGlobalBounds().size.x;
    }
    
    caret->layout.xPosition = textStartingPosition.x + caretOffset;
    caret->layout.yPosition = textStartingPosition.y;
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeComposedComponents() {
    // Background -- Child Button //
    background->layout = SFUI::Prop::Layout::Component{
        .width = computedLayout.size.x,
        .height = computedLayout.size.y,
        .xPosition = computedLayout.position.x,
        .yPosition = computedLayout.position.y
    };
    background->style = SFUI::Prop::Style::Component{
        .borderWidth = style.borderWidth,
        .cornerRadius = style.cornerRadius,
        .fillColor = style.fillColor,
        .borderColor = style.borderColor
    };
    background->buttonStyle = SFUI::Prop::Style::Button{
        .hoveredFillColor = textFieldStyle.hoveredFillColor,
        .hoveredBorderColor = textFieldStyle.hoveredBorderColor,
        .pressedFillColor = textFieldStyle.pressedFillColor,
        .pressedBorderColor = textFieldStyle.pressedBorderColor,
        .disabledFillColor = textFieldStyle.disabledFillColor,
        .disabledBorderColor = textFieldStyle.disabledBorderColor,
        .focusWidth = textFieldStyle.focusWidth,
        .focusOffset = textFieldStyle.focusOffset,
        .focusCornerRadius = textFieldStyle.focusCornerRadius,
        .focusFillColor = textFieldStyle.focusFillColor,
        .toolTipPadding = textFieldStyle.toolTipPadding,
        .toolTipCornerRadius = textFieldStyle.toolTipCornerRadius,
        .toolTipText = textFieldStyle.toolTipText,
        .toolTipFont = textFieldStyle.toolTipFont,
        .toolTipTextSize = textFieldStyle.toolTipTextSize,
        .toolTipFillColor = textFieldStyle.toolTipFillColor,
        .toolTipTextColor = textFieldStyle.toolTipTextColor
    };
    background->buttonBehavior = SFUI::Prop::Behavior::Button{
        .onHoverIn = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onHoverIn) textFieldBehavior.onHoverIn(this->componentID);
        },
        .onHoverOut = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onHoverOut) textFieldBehavior.onHoverOut(this->componentID);
        },
        .onLeftPressIn = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onLeftPressIn) textFieldBehavior.onLeftPress(this->componentID);
            if (!isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            isFocused = true;
        },
        .onLeftPress = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onLeftPress) textFieldBehavior.onLeftPress(this->componentID);
            if (!isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            isFocused = true;
        },
        .onRightPressIn = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onRightPressIn) textFieldBehavior.onRightPressIn(this->componentID);
            if (!isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            isFocused = true;
        },
        .onRightPress = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onRightPress) textFieldBehavior.onRightPress(this->componentID);
            if (!isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            isFocused = true;
        },
        .onMiddlePressIn = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onMiddlePressIn) textFieldBehavior.onMiddlePressIn(this->componentID);
            if (!isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            isFocused = true;
        },
        .onMiddlePress = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onMiddlePress) textFieldBehavior.onMiddlePress(this->componentID);
            if (!isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            isFocused = true;
        },
        .onDoublePress = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onDoublePress) textFieldBehavior.onDoublePress(this->componentID);\
            if (!isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            isFocused = true;
        }
    };
    background->update(renderTargetSize);

    // Input Text -- Child Label //
    inputText->layout = SFUI::Prop::Layout::Component{
        .width = computedLayout.size.x,
        .height = computedLayout.size.y,
        .xPosition = computedLayout.position.x,
        .yPosition = computedLayout.position.y
    };
    inputText->labelStyle = SFUI::Prop::Style::Label{
        .text = (textFieldStyle.text.empty() ? computedTextFieldStyle.placeholderText : textFieldStyle.text),
        .font = textFieldStyle.font,
        .textSize = textFieldStyle.textSize,
        .textStyle = textFieldStyle.textStyle,
        .letterSpacing = textFieldStyle.letterSpacing,
        .lineSpacing = textFieldStyle.lineSpacing,
        .textOutlineThickness = textFieldStyle.textOutlineThickness,
        .textAlignHorizontal = textFieldStyle.textAlignHorizontal,
        .textAlignVertical = "top",
        .textColor = (textFieldStyle.text.empty() ? computedTextFieldStyle.placeholderTextColor : textFieldStyle.textColor),
        .textOutlineColor = textFieldStyle.textOutlineColor
    };
    inputText->update(renderTargetSize);

    // Caret -- Child Container //
    caret->style.fillColor = computedTextFieldStyle.caretFillColor;
    caret->update(renderTargetSize);
}


/**
 * @brief .
 *
 * @param .
 */
SFUI::Void SFUI::TextField::insertText(const char32_t newAppendedText) {
    // Make Sure Only Letter Text Unicode Characters //
    if (newAppendedText > 29 && newAppendedText != 127) {
        textFieldStyle.text.insert(caretColumnIndex++, sf::String(newAppendedText));
        if (textFieldBehavior.onTextChange) textFieldBehavior.onTextChange(componentID, textFieldStyle.text);
        caretClock.restart();
    }
}


/**
 * @brief .
 *
 * @param .
 */
SFUI::Void SFUI::TextField::editText(const sf::Event::KeyPressed* keyPressedEvent) {
    if (!isFocused || textFieldStyle.text.empty()) return;

    // Left Arrow //
    if (keyPressedEvent->code == sf::Keyboard::Key::Left && caretColumnIndex > 0) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt pos = caretColumnIndex;
            while (pos > 0 && CTRL_WHITESPACE_GROUP.find(textFieldStyle.text[pos - 1]) != sf::String::InvalidPos)
                --pos;
            SFUI::UnsignedInt group = (pos > 0) ? getCharacterGroup(textFieldStyle.text[pos - 1]) : 0;
            while (pos > 0 && getCharacterGroup(textFieldStyle.text[pos - 1]) == group)
                --pos;
            caretColumnIndex = static_cast<SFUI::UnsignedInt>(pos);
        }   else {
            --caretColumnIndex;
        }
        caretClock.restart();
    }

    // Right Arrow //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Right && caretColumnIndex < textFieldStyle.text.size()) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt pos = caretColumnIndex;
            while (pos < textFieldStyle.text.size() && CTRL_WHITESPACE_GROUP.find(textFieldStyle.text[pos]) != sf::String::InvalidPos)
                ++pos;
            SFUI::UnsignedInt group = (pos < textFieldStyle.text.size()) ? getCharacterGroup(textFieldStyle.text[pos]) : 0;
            while (pos < textFieldStyle.text.size() && getCharacterGroup(textFieldStyle.text[pos]) == group)
                ++pos;
            caretColumnIndex = static_cast<SFUI::UnsignedInt>(pos);
        }   else {
            ++caretColumnIndex;
        }
        caretClock.restart();
    }

    // Backspace //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Backspace && caretColumnIndex > 0) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt oldCaret = caretColumnIndex;
            SFUI::UnsignedInt pos = caretColumnIndex;
            while (pos > 0 && CTRL_WHITESPACE_GROUP.find(textFieldStyle.text[pos - 1]) != sf::String::InvalidPos)
                --pos;
            SFUI::UnsignedInt group = (pos > 0) ? getCharacterGroup(textFieldStyle.text[pos - 1]) : 0;
            while (pos > 0 && getCharacterGroup(textFieldStyle.text[pos - 1]) == group)
                --pos;
            textFieldStyle.text.erase(pos, oldCaret - pos);
            caretColumnIndex = static_cast<SFUI::UnsignedInt>(pos);
        }   else {
            textFieldStyle.text.erase(caretColumnIndex - 1, 1);
            --caretColumnIndex;
        }
        caretClock.restart();
    }

    // Delete //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Delete && caretColumnIndex < textFieldStyle.text.size()) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt oldCaret = caretColumnIndex;
            SFUI::UnsignedInt pos = caretColumnIndex;
            while (pos < textFieldStyle.text.size() && CTRL_WHITESPACE_GROUP.find(textFieldStyle.text[pos]) != sf::String::InvalidPos)
                ++pos;
            SFUI::UnsignedInt group = (pos < textFieldStyle.text.size()) ? getCharacterGroup(textFieldStyle.text[pos]) : 0;
            while (pos < textFieldStyle.text.size() && getCharacterGroup(textFieldStyle.text[pos]) == group)
                ++pos;
            textFieldStyle.text.erase(oldCaret, pos - oldCaret);
        }   else {
            textFieldStyle.text.erase(caretColumnIndex, 1);
        }
        caretClock.restart();
    }

    // Up Arrow //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Up && caretColumnIndex > 0) {
        caretColumnIndex = 0;
        caretClock.restart();
    }

    // Down Arrow //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Down && caretColumnIndex < textFieldStyle.text.size()) {
        caretColumnIndex = textFieldStyle.text.length();
        caretClock.restart();
    }
}


/**
 * @brief .
 * 
 * @param .
 * 
 * @return .
 */
SFUI::UnsignedInt SFUI::TextField::getCharacterGroup(const char32_t character) {
    // if (CTRL_ALPHANUMERIC_GROUP.find(character) != sf::String::InvalidPos) return 1;
    // if (CTRL_SYMBOL_GROUP.find(character) != sf::String::InvalidPos) return 2;
    return 0;
}
