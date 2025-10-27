#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>
#include <sfml-flexui-extended.hpp>


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
{
    background->addChild(inputText);
    background->addChild(caret);
}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::TextField::TextField(const SFUI::String& componentID, const SFUI::PropGroup::TextField& textFieldPropGroup) :
    Component(componentID, textFieldPropGroup.layout, textFieldPropGroup.style),
    backgroundStyle(textFieldPropGroup.backgroundStyle),
    inputTextStyle(textFieldPropGroup.inputTextStyle),
    caretStyle(textFieldPropGroup.caretStyle),
    textFieldStyle(textFieldPropGroup.textFieldStyle),
    backgroundBehavior(textFieldPropGroup.backgroundBehavior),
    textFieldBehavior(textFieldPropGroup.textFieldBehavior),
    background(std::make_shared<SFUI::Button>(componentID + "_Background")),
    inputText(std::make_shared<SFUI::Label>(componentID + "_InputText")),
    caret(std::make_shared<SFUI::Container>(componentID + "_Caret"))
{
    background->addChild(inputText);
    background->addChild(caret);
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeComposedProps() {
    // background->layout = layout;
    background->layout = SFUI::Prop::Layout::Component{
        .width = computedLayout.size.x * 0.9f,
        .height = computedLayout.size.y * 0.9f,
        .xPosition = computedLayout.position.x,
        .yPosition = computedLayout.position.y,
    };
    background->style = style;
    background->buttonStyle = backgroundStyle;
    background->buttonBehavior = backgroundBehavior;
    inputText->layout = SFUI::Prop::Layout::Component{
        .width = "100%",
        .height = "100%",
    };
    inputText->labelStyle = inputTextStyle;
    inputText->labelStyle.text = "TextField";
    caret->style = caretStyle;
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
            caret->style.fillColor = caretStyle.fillColor;
        }
        else if ((caretClock.getElapsedTime().asMilliseconds() > computedTextFieldStyle.caretOnTime &&
            caretClock.getElapsedTime().asMilliseconds() < (computedTextFieldStyle.caretOnTime + computedTextFieldStyle.caretOffTime))) {
                caret->style.fillColor = SFUI::Color(0, 0, 0, 0);
        }
        else if (caretClock.getElapsedTime().asMilliseconds() > (computedTextFieldStyle.caretOnTime + computedTextFieldStyle.caretOffTime)) {
            caretClock.restart();
        }
    }
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
    caret->layout.xPosition = this->computedLayout.position.x + 10.0f;
    caret->layout.yPosition = this->computedLayout.position.y + 10.0f;
}


/**
 * @brief .
 */
SFUI::Void SFUI::TextField::computeComposedComponents() {
    background->update(renderTargetSize);
    inputText->update(renderTargetSize);
    caret->update(renderTargetSize);
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::TextField::handleEvent(const SFUI::Event& event) {
    background->handleEvent(event);
    inputText->handleEvent(event);
    caret->handleEvent(event);

    // // Mouse Moved Event Handling //
    // if (const SFUI::Event::MouseMoved* mouseMovedEvent = event.getIf<SFUI::Event::MouseMoved>()) {
    //     const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseMovedEvent->position.x, mouseMovedEvent->position.y);
    //     SFUI::Bool buttonHovered = isMouseHovered(mousePosition);
        
    //     if (buttonHovered) {
    //         if (!isHovered) {
    //             isHovered = true;
    //             if (textFieldBehavior.onHoverIn) textFieldBehavior.onHoverIn(componentID);
    //         }
    //     }   else if (!buttonHovered) {
    //         if (isHovered) {
    //             isHovered = false;
    //             if (textFieldBehavior.onHoverOut) textFieldBehavior.onHoverOut(componentID);
    //         }
    //     }
    // }

    // // Mouse Button Pressed Event Handling //
    // if (const SFUI::Event::MouseButtonPressed* mousePressedEvent = event.getIf<SFUI::Event::MouseButtonPressed>()) {
    //     sf::Mouse::Button mouseButton = mousePressedEvent->button;
    //     const SFUI::Vector2i mousePosition = SFUI::Vector2i(mousePressedEvent->position.x, mousePressedEvent->position.y);
    //     SFUI::Bool buttonHovered = isMouseHovered(mousePosition);

    //     if (mouseButton == sf::Mouse::Button::Left) {
    //         if (isFocused) {
    //             isFocused = false;
    //             if (textFieldBehavior.onBlur) textFieldBehavior.onBlur(componentID);
    //         }
    //     }
    //     if (buttonHovered) {
    //         if (mouseButton == sf::Mouse::Button::Left) {
    //             if (!isLeftPressed) {
    //                 isLeftPressed = true;
    //                 if (textFieldBehavior.onLeftPressIn) textFieldBehavior.onLeftPressIn(componentID);
    //             }
    //         }   else if (mouseButton == sf::Mouse::Button::Right) {
    //             if (!isRightPressed) {
    //                 isRightPressed = true;
    //                 if (textFieldBehavior.onRightPressIn) textFieldBehavior.onRightPressIn(componentID);
    //             }
    //         }   else if (mouseButton == sf::Mouse::Button::Middle) {
    //             if (!isMiddlePressed) {
    //                 isMiddlePressed = true;
    //                 if (textFieldBehavior.onMiddlePressIn) textFieldBehavior.onMiddlePressIn(componentID);
    //             }
    //         }
    //     }
    // }

    // // Mouse Button Released Event Handling //
    // if (const SFUI::Event::MouseButtonReleased* mouseReleasedEvent = event.getIf<SFUI::Event::MouseButtonReleased>()) {
    //     sf::Mouse::Button mouseButton = mouseReleasedEvent->button;
    //     const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseReleasedEvent->position.x, mouseReleasedEvent->position.y);
    //     SFUI::Bool buttonHovered = isMouseHovered(mousePosition);

    //     if (buttonHovered) {
    //         SFUI::Time elapsed = doublePressClock.getElapsedTime();
    //         if (elapsed < DOUBLE_PRESS_GAP_MS && std::abs(mousePosition.x - previousPressPosition.x) < 4 && std::abs(mousePosition.y - previousPressPosition.y) < 4) {
    //             if (textFieldBehavior.onDoublePress) textFieldBehavior.onDoublePress(componentID);
    //             doublePressClock.restart();
    //             previousPressTime = SFUI::Time::Zero;
    //         }
    //         else {
    //             if (mouseButton == sf::Mouse::Button::Left && isLeftPressed && textFieldBehavior.onLeftPress) textFieldBehavior.onLeftPress(componentID);
    //             if (mouseButton == sf::Mouse::Button::Right && isRightPressed && textFieldBehavior.onRightPress) textFieldBehavior.onRightPress(componentID);
    //             if (mouseButton == sf::Mouse::Button::Middle && isMiddlePressed && textFieldBehavior.onMiddlePress) textFieldBehavior.onMiddlePress(componentID);
    //             doublePressClock.restart();
    //             previousPressPosition = mousePosition;
    //         }
    //     }
    //     if (isLeftPressed) isLeftPressed = false;
    //     if (isRightPressed) isRightPressed = false;
    //     if (isMiddlePressed) isMiddlePressed = false;
    // }

    // // Key Pressed Event Handling //
    // if (const SFUI::Event::KeyPressed* keyPressedEvent = event.getIf<SFUI::Event::KeyPressed>()) {
    //     if (isFocused && textFieldBehavior.onKeyPress) {
    //         textFieldBehavior.onKeyPress(componentID, keyPressedEvent->code);
    //     }
    // }
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
    computeGraphics();
    computeChildrenMargin();
    computeChildrenSize();
    computeChildrenPosition();
    updateChildren();
    
    // Text Field Specific Computation //
    computeComposedProps();
    computePlaceholderText();
    computeCaretShape();
    computeCaretBlinkTiming();
    computeCaretLifetime();
    computeCaretLayout();
    computeComposedComponents();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::TextField::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(backgroundRects);
    renderTarget.draw(backgroundArcs);
    renderTarget.draw(borderRects);
    renderTarget.draw(borderArcs);
    background->draw(renderTarget);
    inputText->draw(renderTarget);
    caret->draw(renderTarget);
}


/**
 * @brief .
 *
 * @param .
 */
SFUI::Void SFUI::TextField::insertText(const char32_t newAppendedText) {
    // if (!isFocused) return;

    // // Make Sure Only Letter Text Unicode Characters //
    // if (newAppendedText > 29 && newAppendedText != 127) {
    //     textFieldStyle.text.insert(caretColumnIndex++, sf::String(newAppendedText));
    //     // if (textFieldBehavior.onTextChange) textFieldBehavior.onTextChange(componentID, text);
    //     caretClock.restart();
    // }
}


/**
 * @brief .
 *
 * @param .
 */
SFUI::Void SFUI::TextField::editText(const sf::Event::KeyPressed* keyPressedEvent) {
    // if (!isFocused || text.empty()) return;

    // Left Arrow //
    // if (keyPressedEvent->code == sf::Keyboard::Key::Left && caretColumnIndex > 0) {
    //     if (keyPressedEvent->control) {
    //         SFUI::UnsignedInt pos = caretColumnIndex;
    //         while (pos > 0 && CTRL_WHITESPACE_GROUP.find(text[pos - 1]) != sf::String::InvalidPos)
    //             --pos;
    //         SFUI::UnsignedInt group = (pos > 0) ? getCharacterGroup(text[pos - 1]) : 0;
    //         while (pos > 0 && getCharacterGroup(text[pos - 1]) == group)
    //             --pos;
    //         caretColumnIndex = static_cast<SFUI::UnsignedInt>(pos);
    //     }   else {
    //         --caretColumnIndex;
    //     }
    //     caretClock.restart();
    // }

    // Right Arrow //
    // else if (keyPressedEvent->code == sf::Keyboard::Key::Right && caretColumnIndex < text.size()) {
    //     if (keyPressedEvent->control) {
    //         SFUI::UnsignedInt pos = caretColumnIndex;
    //         while (pos < text.size() && CTRL_WHITESPACE_GROUP.find(text[pos]) != sf::String::InvalidPos)
    //             ++pos;
    //         SFUI::UnsignedInt group = (pos < text.size()) ? getCharacterGroup(text[pos]) : 0;
    //         while (pos < text.size() && getCharacterGroup(text[pos]) == group)
    //             ++pos;
    //         caretColumnIndex = static_cast<SFUI::UnsignedInt>(pos);
    //     }   else {
    //         ++caretColumnIndex;
    //     }
    //     caretClock.restart();
    // }

    // Backspace //
    // else if (keyPressedEvent->code == sf::Keyboard::Key::Backspace && caretColumnIndex > 0) {
    //     if (keyPressedEvent->control) {
    //         SFUI::UnsignedInt oldCaret = caretColumnIndex;
    //         SFUI::UnsignedInt pos = caretColumnIndex;
    //         while (pos > 0 && CTRL_WHITESPACE_GROUP.find(text[pos - 1]) != sf::String::InvalidPos)
    //             --pos;
    //         SFUI::UnsignedInt group = (pos > 0) ? getCharacterGroup(text[pos - 1]) : 0;
    //         while (pos > 0 && getCharacterGroup(text[pos - 1]) == group)
    //             --pos;
    //         text.erase(pos, oldCaret - pos);
    //         caretColumnIndex = static_cast<SFUI::UnsignedInt>(pos);
    //     }   else {
    //         text.erase(caretColumnIndex - 1, 1);
    //         --caretColumnIndex;
    //     }
    //     caretClock.restart();
    // }

    // Delete //
    // else if (keyPressedEvent->code == sf::Keyboard::Key::Delete && caretColumnIndex < text.size()) {
    //     if (keyPressedEvent->control) {
    //         SFUI::UnsignedInt oldCaret = caretColumnIndex;
    //         SFUI::UnsignedInt pos = caretColumnIndex;
    //         while (pos < text.size() && CTRL_WHITESPACE_GROUP.find(text[pos]) != sf::String::InvalidPos)
    //             ++pos;
    //         SFUI::UnsignedInt group = (pos < text.size()) ? getCharacterGroup(text[pos]) : 0;
    //         while (pos < text.size() && getCharacterGroup(text[pos]) == group)
    //             ++pos;
    //         text.erase(oldCaret, pos - oldCaret);
    //     }   else {
    //         text.erase(caretColumnIndex, 1);
    //     }
    //     caretClock.restart();
    // }

    // Up Arrow //
    // else if (keyPressedEvent->code == sf::Keyboard::Key::Up && caretColumnIndex > 0) {
    //     caretColumnIndex = 0;
    //     caretClock.restart();
    // }

    // Down Arrow //
    // else if (keyPressedEvent->code == sf::Keyboard::Key::Down && caretColumnIndex < text.size()) {
    //     caretColumnIndex = text.length();
    //     caretClock.restart();
    // }
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
    // return 0;
}
