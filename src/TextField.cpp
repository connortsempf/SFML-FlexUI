#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


////////////////////////////////////////////
// SFML-FlexUI Text Field Component Class //
////////////////////////////////////////////


// Static Variables //
const SFUI::Float SFUI::TextField::CENTER_TEXT_OFFSET_FACTOR = 1.33f;
const SFUI::Float SFUI::TextField::CARET_LINE_VERTICAL_OFFSET_FACTOR = 0.15f;
const SFUI::Float SFUI::TextField::CARET_BOX_VERTICAL_OFFSET_FACTOR = 0.15f;
const SFUI::Float SFUI::TextField::CARET_UNDERLINE_VERTICAL_OFFSET_FACTOR = 1.0f;
const SFUI::Float SFUI::TextField::CARET_LINE_WIDTH_FACTOR = 0.1f;
const SFUI::Float SFUI::TextField::CARET_BOX_WIDTH_FACTOR = 0.55f;
const SFUI::Float SFUI::TextField::CARET_UNDERLINE_WIDTH_FACTOR = 0.55f;
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
    textFieldState(),
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
    textFieldState(textFieldPropGroup.textFieldState),
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

        if (textFieldState.isFocused && !buttonHovered) {
            textFieldState.isFocused = false;
            caretVisible = false;
            if (textFieldBehavior.onPressOut) textFieldBehavior.onPressOut(componentID);
            if (textFieldBehavior.onBlur) textFieldBehavior.onBlur(componentID);
        }
    }

    // Key Pressed Event Handling //
    else if (const sf::Event::KeyPressed* keyPressedEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (textFieldState.isFocused) {
            if (keyPressedEvent->code == sf::Keyboard::Key::Enter) {
                if (computedTextFieldStyle.lineMode == "single") {
                    if (textFieldBehavior.onSubmit) textFieldBehavior.onSubmit(componentID, textFieldStyle.text);
                }
                else if (computedTextFieldStyle.lineMode == "multi") {
                    editText(keyPressedEvent);
                }
            }
            else editText(keyPressedEvent);
        }
    }

    // Text Entered Event Handling //
    else if (const sf::Event::TextEntered* textEnteredEvent = event.getIf<sf::Event::TextEntered>()) {
        if (textFieldState.isFocused) insertText(textEnteredEvent->unicode);
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
    computeLineMode();
    computePlaceholderText();
    computeComposedBackground();
    computeComposedInputText();
    computeTextInset();
    computeCaretShape();
    computeCaretBlinkTiming();
    computeCaretLifetime();
    computeComposedCaret();
    computeDynamicTextOffset();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::TextField::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    background->draw(drawTarget, window);
    inputText->draw(drawTarget, window);
    if (!textFieldState.isDisabled && textFieldState.isFocused && caretVisible) caret->draw(drawTarget, window);
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
SFUI::Void SFUI::TextField::computePlaceholderText() {
    computedTextFieldStyle.placeholderText = textFieldStyle.placeholderText;
    computedTextFieldStyle.placeholderTextColor = resolveColorSubProp(textFieldStyle.placeholderTextColor);
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeComposedBackground() {
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
    background->buttonState = SFUI::Prop::State::Button{
        .isDisabled = textFieldState.isDisabled,
        .isFocused = textFieldState.isFocused
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
        .onEnable = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onEnable) textFieldBehavior.onEnable(this->componentID);
        },
        .onDisable = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onDisable) textFieldBehavior.onDisable(this->componentID);
        },
        .onFocus = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onFocus) textFieldBehavior.onFocus(this->componentID);
        },
        .onBlur = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onBlur) textFieldBehavior.onBlur(this->componentID);
        },
        .onHoverIn = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onHoverIn) textFieldBehavior.onHoverIn(this->componentID);
        },
        .onHoverOut = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onHoverOut) textFieldBehavior.onHoverOut(this->componentID);
        },
        .onLeftPressIn = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onLeftPressIn) textFieldBehavior.onLeftPress(this->componentID);
            if (!textFieldState.isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            textFieldState.isFocused = true;
        },
        .onLeftPress = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onLeftPress) textFieldBehavior.onLeftPress(this->componentID);
            if (!textFieldState.isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            textFieldState.isFocused = true;
        },
        .onRightPressIn = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onRightPressIn) textFieldBehavior.onRightPressIn(this->componentID);
            if (!textFieldState.isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            textFieldState.isFocused = true;
        },
        .onRightPress = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onRightPress) textFieldBehavior.onRightPress(this->componentID);
            if (!textFieldState.isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            textFieldState.isFocused = true;
        },
        .onMiddlePressIn = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onMiddlePressIn) textFieldBehavior.onMiddlePressIn(this->componentID);
            if (!textFieldState.isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            textFieldState.isFocused = true;
        },
        .onMiddlePress = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onMiddlePress) textFieldBehavior.onMiddlePress(this->componentID);
            if (!textFieldState.isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            textFieldState.isFocused = true;
        },
        .onDoublePress = [this](const SFUI::String& componentID) {
            if (textFieldBehavior.onDoublePress) textFieldBehavior.onDoublePress(this->componentID);
            if (!textFieldState.isFocused && textFieldBehavior.onFocus)
                textFieldBehavior.onFocus(componentID);
            textFieldState.isFocused = true;
        }
    };
    background->update(renderTargetSize);
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeComposedInputText() {
    inputText->layout = SFUI::Prop::Layout::Component{
        .width = computedLayout.size.x - (computedTextFieldStyle.textInset.x + computedTextFieldStyle.textInset.y),
        .height = computedLayout.size.y - (computedTextFieldStyle.textInset.z + computedTextFieldStyle.textInset.w),
        .xPosition = computedLayout.position.x + computedTextFieldStyle.textInset.x,
        .yPosition = computedLayout.position.y + computedTextFieldStyle.textInset.z
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
        .textOffset = dynamicTextOffset,
        .textColor = (textFieldStyle.text.empty() ? computedTextFieldStyle.placeholderTextColor : textFieldStyle.textColor),
        .textOutlineColor = textFieldStyle.textOutlineColor
    };
    inputText->update(renderTargetSize);
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeTextInset() {
    computedTextFieldStyle.textInset = resolveUniQuadSubProp(computedLayout.size, textFieldStyle.textInset);
    if (computedTextFieldStyle.lineMode == "single") {
        SFUI::Float singleLineVerticalInset = (computedLayout.size.y - (inputText->getTextSize() * CENTER_TEXT_OFFSET_FACTOR)) / 2.0f;
        computedTextFieldStyle.textInset.z = computedTextFieldStyle.textInset.w = singleLineVerticalInset;
    }
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
    if (textFieldState.isFocused) {
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
SFUI::Void SFUI::TextField::computeComposedCaret() {
    SFUI::Vector2f computedSize = {0.0f, 0.0f};
    SFUI::Vector2f computedPositionOffset = {0.0f, 0.0f};

    if (computedTextFieldStyle.caretShape == "line") {
        computedSize = {inputText->getTextSize() * CARET_LINE_WIDTH_FACTOR, inputText->getTextSize()};
        computedPositionOffset = {0.0f, (inputText->getTextSize() * CARET_LINE_VERTICAL_OFFSET_FACTOR)};
    }
    else if (computedTextFieldStyle.caretShape == "box") {
        computedSize = {inputText->getTextSize() * CARET_BOX_WIDTH_FACTOR, inputText->getTextSize()};
        computedPositionOffset = {0.0f, (inputText->getTextSize() * CARET_BOX_VERTICAL_OFFSET_FACTOR)};
    }
    else if (computedTextFieldStyle.caretShape == "underline") {
        computedSize = {inputText->getTextSize() * CARET_UNDERLINE_WIDTH_FACTOR, inputText->getTextSize() * CARET_UNDERLINE_HEIGHT_FACTOR};
        computedPositionOffset = {0.0f, (inputText->getTextSize() * CARET_UNDERLINE_VERTICAL_OFFSET_FACTOR)};
    }

    caret->layout.width = computedSize.x;
    caret->layout.height = computedSize.y;

    SFUI::Vector2f caretPosition = inputText->getCharacterPosition(caretIndex);
    caret->layout.xPosition = caretPosition.x + computedPositionOffset.x;
    caret->layout.yPosition = caretPosition.y  + computedPositionOffset.y;
    caret->style.fillColor = textFieldStyle.caretFillColor;
    caret->update(renderTargetSize);
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeDynamicTextOffset() {
    SFUI::Vector2i tempCaretPosition = caret->getPosition();
    SFUI::Vector2f caretPosition = {static_cast<SFUI::Float>(tempCaretPosition.x), static_cast<SFUI::Float>(tempCaretPosition.y)};
    SFUI::Vector2f caretSize = caret->getSize();
    SFUI::Vector2i tempTextBoxPosition = inputText->getPosition();
    SFUI::Vector2f textBoxPosition = {static_cast<SFUI::Float>(tempTextBoxPosition.x), static_cast<SFUI::Float>(tempTextBoxPosition.y)};
    SFUI::Vector2f textBoxSize = inputText->getSize();

    // Horizontal Offset //
    if (caretPosition.x < textBoxPosition.x)
        dynamicTextOffset.x += textBoxPosition.x - caretPosition.x;
    else if (caretPosition.x + caretSize.x > textBoxPosition.x + textBoxSize.x)
        dynamicTextOffset.x -= (caretPosition.x + caretSize.x) - (textBoxPosition.x + textBoxSize.x);

    // Vertical Offset //
    if (caretPosition.y < textBoxPosition.y)
        dynamicTextOffset.y += textBoxPosition.y - caretPosition.y;
    else if (caretPosition.y + caretSize.y > textBoxPosition.y + textBoxSize.y)
        dynamicTextOffset.y -= (caretPosition.y + caretSize.y) - (textBoxPosition.y + textBoxSize.y);

    inputText->labelStyle.textOffset = dynamicTextOffset;
    inputText->update(renderTargetSize);

    computeComposedCaret();
    caret->update(renderTargetSize);
}


/**
 * @brief .
 *
 * @param .
 */
SFUI::Void SFUI::TextField::insertText(const char32_t newAppendedText) {
    if (newAppendedText <= 29 || newAppendedText == 127) return;
    
    textFieldStyle.text.insert(caretIndex++, sf::String(newAppendedText));
    if (textFieldBehavior.onTextChange) textFieldBehavior.onTextChange(componentID, textFieldStyle.text);
    caretClock.restart();
}


/**
 * @brief .
 *
 * @param .
 */
SFUI::Void SFUI::TextField::editText(const sf::Event::KeyPressed* keyPressedEvent) {
    if (!textFieldState.isFocused) return;

    // Enter //
    if (keyPressedEvent->code == sf::Keyboard::Key::Enter && computedTextFieldStyle.lineMode == "multi") {
        textFieldStyle.text.insert(caretIndex++, "\n");
    }

    // Left Arrow //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Left && caretIndex > 0) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt pos = caretIndex;
            while (pos > 0 && CTRL_WHITESPACE_GROUP.find(textFieldStyle.text[pos - 1]) != sf::String::InvalidPos)
                --pos;
            SFUI::UnsignedInt group = (pos > 0) ? getCharacterGroup(textFieldStyle.text[pos - 1]) : 0;
            while (pos > 0 && getCharacterGroup(textFieldStyle.text[pos - 1]) == group)
                --pos;
            caretIndex = static_cast<SFUI::UnsignedInt>(pos);
        }   else {
            --caretIndex;
        }
        caretClock.restart();
    }

    // Right Arrow //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Right && caretIndex < textFieldStyle.text.size()) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt pos = caretIndex;
            while (pos < textFieldStyle.text.size() && CTRL_WHITESPACE_GROUP.find(textFieldStyle.text[pos]) != sf::String::InvalidPos)
                ++pos;
            SFUI::UnsignedInt group = (pos < textFieldStyle.text.size()) ? getCharacterGroup(textFieldStyle.text[pos]) : 0;
            while (pos < textFieldStyle.text.size() && getCharacterGroup(textFieldStyle.text[pos]) == group)
                ++pos;
            caretIndex = static_cast<SFUI::UnsignedInt>(pos);
        }   else {
            ++caretIndex;
        }
        caretClock.restart();
    }

    // Backspace //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Backspace && caretIndex > 0) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt oldCaret = caretIndex;
            SFUI::UnsignedInt pos = caretIndex;
            while (pos > 0 && CTRL_WHITESPACE_GROUP.find(textFieldStyle.text[pos - 1]) != sf::String::InvalidPos)
                --pos;
            SFUI::UnsignedInt group = (pos > 0) ? getCharacterGroup(textFieldStyle.text[pos - 1]) : 0;
            while (pos > 0 && getCharacterGroup(textFieldStyle.text[pos - 1]) == group)
                --pos;
            textFieldStyle.text.erase(pos, oldCaret - pos);
            caretIndex = static_cast<SFUI::UnsignedInt>(pos);
        }   else {
            textFieldStyle.text.erase(caretIndex - 1, 1);
            --caretIndex;
        }
        caretClock.restart();
    }

    // Delete //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Delete && caretIndex < textFieldStyle.text.size()) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt oldCaret = caretIndex;
            SFUI::UnsignedInt pos = caretIndex;
            while (pos < textFieldStyle.text.size() && CTRL_WHITESPACE_GROUP.find(textFieldStyle.text[pos]) != sf::String::InvalidPos)
                ++pos;
            SFUI::UnsignedInt group = (pos < textFieldStyle.text.size()) ? getCharacterGroup(textFieldStyle.text[pos]) : 0;
            while (pos < textFieldStyle.text.size() && getCharacterGroup(textFieldStyle.text[pos]) == group)
                ++pos;
            textFieldStyle.text.erase(oldCaret, pos - oldCaret);
        }   else {
            textFieldStyle.text.erase(caretIndex, 1);
        }
        caretClock.restart();
    }

    // Up Arrow //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Up && caretIndex > 0) {
        if (computedTextFieldStyle.lineMode == "single") {
            caretIndex = 0;
        }
        else if (computedTextFieldStyle.lineMode == "multi") {
            SFUI::UnsignedInt belowRowCharsPassed = 0;
            SFUI::UnsignedInt aboveRowCharsPassed = 0;
            SFUI::UnsignedInt belowRowNewlineIndex = 0;
            SFUI::UnsignedInt aboveRowNewlineIndex = 0;
            SFUI::Bool caretMoveNeeded = true;

            for (SFUI::Int i = caretIndex - 1; i >= 0; --i) {
                if (textFieldStyle.text[i] == '\n') {
                    belowRowNewlineIndex = i;
                    break;
                }
                else if (i == 0) {
                    caretIndex = 0;
                    caretMoveNeeded = false;
                    break;
                }
                ++belowRowCharsPassed;
            }
            
            for (SFUI::Int i = belowRowNewlineIndex - 1; i >= 0; --i) {
                if (textFieldStyle.text[i] == '\n' || i == 0) {
                    aboveRowNewlineIndex = i;
                    if (i == 0) ++aboveRowCharsPassed;
                    break;
                }
                ++aboveRowCharsPassed;
            }

            if (caretMoveNeeded) {
                if (belowRowCharsPassed <= aboveRowCharsPassed)
                    caretIndex = aboveRowNewlineIndex + belowRowCharsPassed;
                else if (belowRowCharsPassed > aboveRowCharsPassed)
                    caretIndex = aboveRowNewlineIndex + aboveRowCharsPassed;
            }
        }
        caretClock.restart();
    }

    // Down Arrow //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Down && caretIndex < textFieldStyle.text.size()) {
        if (computedTextFieldStyle.lineMode == "single") {
            caretIndex = textFieldStyle.text.length();
        }
        else if (computedTextFieldStyle.lineMode == "multi") {
            SFUI::UnsignedInt aboveRowCharsPassed = 0;
            SFUI::UnsignedInt belowRowCharsPassed = 0;
            SFUI::UnsignedInt aboveRowNewlineIndex = 0;
            SFUI::UnsignedInt belowRowNewlineIndex = 0;
            SFUI::UnsignedInt textLength = textFieldStyle.text.size();
            SFUI::Bool caretMoveNeeded = true;

            for (SFUI::Int i = caretIndex - 1; i >= 0; --i) {
                if (textFieldStyle.text[i] == '\n') break;
                ++aboveRowCharsPassed;
            }

            SFUI::Bool aboveRowNewlineMet = false;
            for (SFUI::Int i = caretIndex - 1; i < textLength; ++i) {
                if (!aboveRowNewlineMet) {
                    if (textFieldStyle.text[i] == '\n') {
                        aboveRowNewlineIndex = i;
                        aboveRowNewlineMet = true;
                    }
                    else if (i == textLength - 1) {
                        caretIndex = textLength;
                        caretMoveNeeded = false;
                        break;
                    }
                }
                else if (aboveRowNewlineMet) {
                    if (textFieldStyle.text[i] == '\n') {
                        belowRowNewlineIndex = i;
                        break;
                    }
                    ++belowRowCharsPassed;
                }
            }

            if (caretMoveNeeded) {
                if (aboveRowCharsPassed > belowRowCharsPassed)
                    caretIndex = aboveRowNewlineIndex + belowRowCharsPassed + 1;
                else if (aboveRowCharsPassed <= belowRowCharsPassed)
                    caretIndex = aboveRowNewlineIndex + aboveRowCharsPassed + 1;
            }
        }
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
    if (CTRL_ALPHANUMERIC_GROUP.find(character) != sf::String::InvalidPos) return 1;
    if (CTRL_SYMBOL_GROUP.find(character) != sf::String::InvalidPos) return 2;
    return 0;
}
