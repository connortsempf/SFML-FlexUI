/**
 * @file TextField.cpp
 * @brief Implements the SFUI TextField component.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.0
 *
 * This file contains the function definitions and internal logic for the
 * SFUI TextField component. It handles:
 *   - Construction and destruction
 *   - Event handling (keyboard input, focus)
 *   - Rendering and style updates
 *
 * It works in conjunction with TextField.hpp to provide full functionality
 * of the TextField component.
 */


#include "Components/TextField.hpp"


/**
 * @brief Factor controlling the vertical offset when centering text.
 */
const SFUI::Float SFUI::TextField::CENTER_TEXT_OFFSET_FACTOR = 1.3f;


/**
 * @brief Factor controlling the vertical offset for the caret in line mode.
 */

const SFUI::Float SFUI::TextField::CARET_LINE_VERTICAL_OFFSET_FACTOR = 0.15f;


/**
 * @brief Factor controlling the vertical offset for the caret in box mode.
 */
const SFUI::Float SFUI::TextField::CARET_BOX_VERTICAL_OFFSET_FACTOR = 0.15f;


/**
 * @brief Factor controlling the vertical offset for the caret in underline mode.
 */
const SFUI::Float SFUI::TextField::CARET_UNDERLINE_VERTICAL_OFFSET_FACTOR = 1.0f;


/**
 * @brief Factor controlling the caret width in line mode.
 */
const SFUI::Float SFUI::TextField::CARET_LINE_WIDTH_FACTOR = 0.1f;


/**
 * @brief Factor controlling the caret width in box mode.
 */
const SFUI::Float SFUI::TextField::CARET_BOX_WIDTH_FACTOR = 0.55f;


/**
 * @brief Factor controlling the caret width in underline mode.
 */
const SFUI::Float SFUI::TextField::CARET_UNDERLINE_WIDTH_FACTOR = 0.55f;


/**
 * @brief Factor controlling the height of the underline caret.
 */
const SFUI::Float SFUI::TextField::CARET_UNDERLINE_HEIGHT_FACTOR = 0.1f;


/**
 * @brief Character group recognized as whitespace for CTRL-mod actions.
 */
const SFUI::String SFUI::TextField::CTRL_WHITESPACE_GROUP = " \t";


/**
 * @brief Character group recognized as alphanumeric for CTRL-mod actions.
 */
const SFUI::String SFUI::TextField::CTRL_ALPHANUMERIC_GROUP = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_";


/**
 * @brief Character group recognized as symbols for CTRL-mod actions.
 */
const SFUI::String SFUI::TextField::CTRL_SYMBOL_GROUP = "`~!@#$%^&*()-=+[]{}\\|:;\'\",<.>/?";


/**
 * @brief The constructor for the TextField component.
 *
 * @param componentID The unique identifier for the TextField component.
 */
SFUI::TextField::TextField(SFUI::String componentID) :
    Component(componentID),
    background(componentID + "_Background"),
    inputText(componentID + "_InputText"),
    caret(componentID + "_Caret")
{
    this->baseLayout = this->layout;
    this->baseStyle = this->style;
    this->baseState = this->state;
}


/**
 * @brief Handle a user input event.
 *
 * @param event The event to handle.
 */
SFUI::Void SFUI::TextField::handleEvent(const SFUI::Event& event) {
    background.handleEvent(event);
    inputText.handleEvent(event);
    caret.handleEvent(event);

    // Mouse Button Pressed Event Handling //
    if (const SFUI::Event::MouseButtonPressed* mousePressedEvent = event.getIf<SFUI::Event::MouseButtonPressed>()) {
        sf::Mouse::Button mouseButton = mousePressedEvent->button;
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mousePressedEvent->position.x, mousePressedEvent->position.y);
        SFUI::Bool textFieldHovered = isMouseHovered(mousePosition);

        if (state.isFocused && !textFieldHovered) {
            state.isFocused = false;
            caretVisible = false;
            if (behavior.onPressOut) behavior.onPressOut(componentID);
            if (behavior.onBlur) behavior.onBlur(componentID);
        }
    }

    // Key Pressed Event Handling //
    else if (const sf::Event::KeyPressed* keyPressedEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (state.isFocused) {
            if (keyPressedEvent->code == sf::Keyboard::Key::Enter) {
                if (computedTextFieldStyle.lineMode == "single") {
                    if (behavior.onSubmit) behavior.onSubmit(componentID, style.text);
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
        if (state.isFocused) insertText(textEnteredEvent->unicode);
    }
}


/**
 * @brief Handle the pre updaate updates for the component.
 */
SFUI::Void SFUI::TextField::preUpdate() {
    this->baseLayout = this->layout;
    this->baseStyle = this->style;
    this->baseState = this->state;
    background.preUpdate();
    inputText.preUpdate();
    caret.preUpdate();
}


/**
 * @brief Recalculate the component's properties.
 *
 * @param renderTargetSize The size of the render target.
 */
SFUI::Void SFUI::TextField::update(const SFUI::Vector2u renderTargetSize) {
    this->renderTargetSize = renderTargetSize;
    computeAlignment();
    computeLayoutBox();
    computeStyles();
    computeChildrenLayoutBox();
    updateChildren();
    computeLineMode();
    computeBackground();
    computeInputText();
    if (state.isFocused) {
        computeCaret();
        computeDynamicTextOffset();
    }
}


/**
 * @brief Draw the component.
 *
 * @param drawTarget The target to draw to.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::TextField::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    background.draw(drawTarget, window);
    inputText.draw(drawTarget, window);
    if (!state.isDisabled && state.isFocused && caretVisible) caret.draw(drawTarget, window);
}


/**
 * @brief Draw the component or inner components on an overlay layer on top of the main UI tree to the render target.
 *
 * This is relevant for components that are actively animating and do not want their drawn geometry subject to
 * clipping by their parents' bounds. It is also useful for inner components like tooltips, context menus, modals,
 * and other special UI components. This meant to have a seperate second draw pass after the initial UI tree draw()
 * function calls to the components.
 *
 * @param drawTarget Target to draw on.
 * @param window Window reference.
 */
SFUI::Void SFUI::TextField::drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    background.drawOverlay(drawTarget, window);
}


/**
 * @brief Compute the line mode property.
 */
SFUI::Void SFUI::TextField::computeLineMode() {
    SFUI::String tempLineMode = style.lineMode;
    std::transform(tempLineMode.begin(), tempLineMode.end(), tempLineMode.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempLineMode == "single" || tempLineMode == "multi")
        computedTextFieldStyle.lineMode = tempLineMode;
    else
        computedTextFieldStyle.lineMode = "single";
}


/**
 * @brief Compute the background properties.
 */
SFUI::Void SFUI::TextField::computeBackground() {
    // Background Layout //
    background.layout.width = computedLayout.size.x;
    background.layout.height = computedLayout.size.y;
    background.layout.xPosition = computedLayout.position.x;
    background.layout.yPosition = computedLayout.position.y;

    // Background Style //
    background.style.borderWidth = style.borderWidth;
    background.style.cornerRadius = style.cornerRadius;
    background.style.fillColor = style.fillColor;
    background.style.borderColor = style.borderColor;
    background.style.disabledFillColor = style.disabledFillColor;
    background.style.disabledBorderColor = style.disabledBorderColor;
    background.style.toolTipPadding = style.toolTipPadding;
    background.style.toolTipCornerRadius = style.toolTipCornerRadius;
    background.style.toolTipText = style.toolTipText;
    background.style.toolTipFont = style.toolTipFont;
    background.style.toolTipTextSize = style.toolTipTextSize;
    background.style.toolTipFillColor = style.toolTipFillColor;
    background.style.toolTipTextColor = style.toolTipTextColor;

    // Background State //
    background.state.isDisabled = state.isDisabled;

    // Background Behavior //
    background.behavior.onEnable = [this](const SFUI::String& componentID) {
        if (behavior.onEnable) behavior.onEnable(this->componentID);
    };
    background.behavior.onDisable = [this](const SFUI::String& componentID) {
        if (behavior.onDisable) behavior.onDisable(this->componentID);
    };
    background.behavior.onHoverIn = [this](const SFUI::String& componentID) {
        if (behavior.onHoverIn) behavior.onHoverIn(this->componentID);
    };
    background.behavior.onHoverOut = [this](const SFUI::String& componentID) {
        if (behavior.onHoverOut) behavior.onHoverOut(this->componentID);
    };
    background.behavior.onLeftPressIn = [this](const SFUI::String& componentID) {
        if (behavior.onLeftPressIn) behavior.onLeftPress(this->componentID);
        if (!state.isFocused && behavior.onFocus)
            behavior.onFocus(componentID);
        state.isFocused = true;
    };
    background.behavior.onLeftPress = [this](const SFUI::String& componentID) {
        if (behavior.onLeftPress) behavior.onLeftPress(this->componentID);
        if (!state.isFocused && behavior.onFocus)
            behavior.onFocus(componentID);
        state.isFocused = true;
    };
    background.behavior.onRightPressIn = [this](const SFUI::String& componentID) {
        if (behavior.onRightPressIn) behavior.onRightPressIn(this->componentID);
        if (!state.isFocused && behavior.onFocus)
            behavior.onFocus(componentID);
        state.isFocused = true;
    };
    background.behavior.onRightPress = [this](const SFUI::String& componentID) {
        if (behavior.onRightPress) behavior.onRightPress(this->componentID);
        if (!state.isFocused && behavior.onFocus)
            behavior.onFocus(componentID);
        state.isFocused = true;
    };
    background.behavior.onMiddlePressIn = [this](const SFUI::String& componentID) {
        if (behavior.onMiddlePressIn) behavior.onMiddlePressIn(this->componentID);
        if (!state.isFocused && behavior.onFocus)
            behavior.onFocus(componentID);
        state.isFocused = true;
    };
    background.behavior.onMiddlePress = [this](const SFUI::String& componentID) {
        if (behavior.onMiddlePress) behavior.onMiddlePress(this->componentID);
        if (!state.isFocused && behavior.onFocus)
            behavior.onFocus(componentID);
        state.isFocused = true;
    };
    background.behavior.onDoublePress = [this](const SFUI::String& componentID) {
        if (behavior.onDoublePress) behavior.onDoublePress(this->componentID);
        if (!state.isFocused && behavior.onFocus)
            behavior.onFocus(componentID);
        state.isFocused = true;
    };

    // Update //
    background.update(renderTargetSize);
}


/**
 * @brief Compute the input text properties.
 */
SFUI::Void SFUI::TextField::computeInputText() {
    // Text Inset //
    SFUI::Vector4f computedTextInset = resolveUniQuadSubProp(computedLayout.size, style.textInset);
    if (computedTextFieldStyle.lineMode == "single") {
        SFUI::Float singleLineVerticalInset = (computedLayout.size.y - (style.textSize * CENTER_TEXT_OFFSET_FACTOR)) / 2.0f;
        computedTextInset.z = computedTextInset.w = singleLineVerticalInset;
    }
    if ((computedLayout.size.y - (computedTextInset.z + computedTextInset.w)) < style.textSize)
        computedTextInset.z = computedTextInset.w = style.textSize / 2.0f;
    if ((computedLayout.size.x - (computedTextInset.x + computedTextInset.y)) < style.textSize)
        computedTextInset.x = computedTextInset.y = style.textSize / 2.0f;

    // Input Text Layout //
    inputText.layout.width = computedLayout.size.x - (computedTextInset.x + computedTextInset.y);
    inputText.layout.height = computedLayout.size.y - (computedTextInset.z + computedTextInset.w);
    inputText.layout.xPosition = computedLayout.position.x + computedTextInset.x;
    inputText.layout.yPosition = computedLayout.position.y + computedTextInset.z;

    // Input Text Styles //
    inputText.style.text = (style.text.empty() ? style.placeholderText : style.text);
    inputText.style.font = style.font;
    inputText.style.textSize = style.textSize;
    inputText.style.textStyle = style.textStyle;
    inputText.style.letterSpacing = style.letterSpacing;
    inputText.style.lineSpacing = style.lineSpacing;
    inputText.style.textOutlineThickness = style.textOutlineThickness;
    inputText.style.textAlignHorizontal = style.textAlignHorizontal;
    inputText.style.textAlignVertical = "top";
    inputText.style.textOffset = dynamicTextOffset;
    inputText.style.textColor = (style.text.empty() ? style.placeholderTextColor : style.textColor);
    inputText.style.textOutlineColor = style.textOutlineColor;

    // Update //
    inputText.update(renderTargetSize);
}


/**
 * @brief Compute the caret properties.
 */
SFUI::Void SFUI::TextField::computeCaret() {
    // Caret Shape //
    SFUI::String computedCaretShape = style.caretShape;
    std::transform(computedCaretShape.begin(), computedCaretShape.end(), computedCaretShape.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    if (computedCaretShape != "line" && computedCaretShape != "box" && computedCaretShape != "underline")
        computedCaretShape = "line";

    // Caret On/Off State //
    SFUI::Float caretOnTime = style.caretBlinkTime;
    SFUI::Float caretOffTime = style.caretBlinkTime / std::clamp(style.caretBlinkRatio, 0.0f, 2.0f);
    if (state.isFocused) {
        if (caretClock.getElapsedTime().asMilliseconds() < caretOnTime)
            caretVisible = true;
        else if ((caretClock.getElapsedTime().asMilliseconds() > caretOnTime &&
        caretClock.getElapsedTime().asMilliseconds() < (caretOnTime + caretOffTime)))
            caretVisible = false;
        else if (caretClock.getElapsedTime().asMilliseconds() > (caretOnTime + caretOffTime))
            caretClock.restart();
    }

    // Caret Size //
    SFUI::Vector2f computedSize = {0.0f, 0.0f};
    if (computedCaretShape == "line")
        computedSize = {style.textSize * CARET_LINE_WIDTH_FACTOR, style.textSize};
    else if (computedCaretShape == "box")
        computedSize = {style.textSize * CARET_BOX_WIDTH_FACTOR, style.textSize};
    else if (computedCaretShape == "underline")
        computedSize = {style.textSize * CARET_UNDERLINE_WIDTH_FACTOR, style.textSize * CARET_UNDERLINE_HEIGHT_FACTOR};
    caret.layout.width = computedSize.x;
    caret.layout.height = computedSize.y;

    // Caret Position //
    SFUI::Vector2f computedPositionOffset = {0.0f, 0.0f};
    if (computedCaretShape == "line")
        computedPositionOffset = {0.0f, (style.textSize * CARET_LINE_VERTICAL_OFFSET_FACTOR)};
    else if (computedCaretShape == "box")
        computedPositionOffset = {0.0f, (style.textSize * CARET_BOX_VERTICAL_OFFSET_FACTOR)};
    else if (computedCaretShape == "underline")
        computedPositionOffset = {0.0f, (style.textSize * CARET_UNDERLINE_VERTICAL_OFFSET_FACTOR)};
    SFUI::Vector2f caretPosition = inputText.getCharacterPosition(caretIndex);
    caret.layout.xPosition = caretPosition.x + computedPositionOffset.x;
    caret.layout.yPosition = caretPosition.y  + computedPositionOffset.y;
    caret.style.fillColor = style.caretFillColor;

    // Update //
    caret.update(renderTargetSize);
}


/**
 * @brief Compute the dynamic text offset to ensure caret visibility.
 */
SFUI::Void SFUI::TextField::computeDynamicTextOffset() {
    SFUI::Vector2i tempCaretPosition = caret.getPosition();
    SFUI::Vector2f caretPosition = {static_cast<SFUI::Float>(tempCaretPosition.x), static_cast<SFUI::Float>(tempCaretPosition.y)};
    SFUI::Vector2f caretSize = caret.getSize();
    SFUI::Vector2i tempTextBoxPosition = inputText.getPosition();
    SFUI::Vector2f textBoxPosition = {static_cast<SFUI::Float>(tempTextBoxPosition.x), static_cast<SFUI::Float>(tempTextBoxPosition.y)};
    SFUI::Vector2f textBoxSize = inputText.getSize();
    SFUI::Bool dynamicUpdateNeeded = false;

    if (caretPosition.x < textBoxPosition.x) {
        dynamicUpdateNeeded = true;
        dynamicTextOffset.x += textBoxPosition.x - caretPosition.x;
    }
    else if (caretPosition.x + caretSize.x > textBoxPosition.x + textBoxSize.x) {
        dynamicUpdateNeeded = true;
        dynamicTextOffset.x -= (caretPosition.x + caretSize.x) - (textBoxPosition.x + textBoxSize.x);
    }
    if (caretPosition.y < textBoxPosition.y) {
        dynamicUpdateNeeded = true;
        dynamicTextOffset.y += textBoxPosition.y - caretPosition.y;
    }
    else if (caretPosition.y + caretSize.y > textBoxPosition.y + textBoxSize.y) {
        dynamicUpdateNeeded = true;
        dynamicTextOffset.y -= (caretPosition.y + caretSize.y) - (textBoxPosition.y + textBoxSize.y);
    }

    if (dynamicUpdateNeeded) {
        inputText.style.textOffset = dynamicTextOffset;
        inputText.update(renderTargetSize);
        computeCaret();
    }
}


/**
 * @brief Handle inserting new text into the text field.
 *
 * @param newAppendedText The new text to insert.
 */
SFUI::Void SFUI::TextField::insertText(const char32_t newAppendedText) {
    if (newAppendedText <= 29 || newAppendedText == 127) return;

    style.text.insert(caretIndex++, sf::String(newAppendedText));
    if (behavior.onTextChange) behavior.onTextChange(componentID, style.text);
    caretClock.restart();
}


/**
 * @brief Handle editing the text in the text field.
 *
 * @param keyPressedEvent The key pressed event to handle.
 */
SFUI::Void SFUI::TextField::editText(const sf::Event::KeyPressed* keyPressedEvent) {
    if (!state.isFocused) return;

    // Enter //
    if (keyPressedEvent->code == sf::Keyboard::Key::Enter && computedTextFieldStyle.lineMode == "multi") {
        style.text.insert(caretIndex++, "\n");
    }

    // Left Arrow //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Left && caretIndex > 0) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt pos = caretIndex;
            while (pos > 0 && CTRL_WHITESPACE_GROUP.find(style.text[pos - 1]) != sf::String::InvalidPos)
                --pos;
            SFUI::UnsignedInt group = (pos > 0) ? getCharacterGroup(style.text[pos - 1]) : 0;
            while (pos > 0 && getCharacterGroup(style.text[pos - 1]) == group)
                --pos;
            caretIndex = static_cast<SFUI::UnsignedInt>(pos);
        }   else {
            --caretIndex;
        }
        caretClock.restart();
    }

    // Right Arrow //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Right && caretIndex < style.text.size()) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt pos = caretIndex;
            while (pos < style.text.size() && CTRL_WHITESPACE_GROUP.find(style.text[pos]) != sf::String::InvalidPos)
                ++pos;
            SFUI::UnsignedInt group = (pos < style.text.size()) ? getCharacterGroup(style.text[pos]) : 0;
            while (pos < style.text.size() && getCharacterGroup(style.text[pos]) == group)
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
            while (pos > 0 && CTRL_WHITESPACE_GROUP.find(style.text[pos - 1]) != sf::String::InvalidPos)
                --pos;
            SFUI::UnsignedInt group = (pos > 0) ? getCharacterGroup(style.text[pos - 1]) : 0;
            while (pos > 0 && getCharacterGroup(style.text[pos - 1]) == group)
                --pos;
            style.text.erase(pos, oldCaret - pos);
            caretIndex = static_cast<SFUI::UnsignedInt>(pos);
        }   else {
            style.text.erase(caretIndex - 1, 1);
            --caretIndex;
        }
        if (behavior.onTextChange) behavior.onTextChange(componentID, style.text);
        caretClock.restart();
    }

    // Delete //
    else if (keyPressedEvent->code == sf::Keyboard::Key::Delete && caretIndex < style.text.size()) {
        if (keyPressedEvent->control) {
            SFUI::UnsignedInt oldCaret = caretIndex;
            SFUI::UnsignedInt pos = caretIndex;
            while (pos < style.text.size() && CTRL_WHITESPACE_GROUP.find(style.text[pos]) != sf::String::InvalidPos)
                ++pos;
            SFUI::UnsignedInt group = (pos < style.text.size()) ? getCharacterGroup(style.text[pos]) : 0;
            while (pos < style.text.size() && getCharacterGroup(style.text[pos]) == group)
                ++pos;
            style.text.erase(oldCaret, pos - oldCaret);
        }   else {
            style.text.erase(caretIndex, 1);
        }
        if (behavior.onTextChange) behavior.onTextChange(componentID, style.text);
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
                if (style.text[i] == '\n') {
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
                if (style.text[i] == '\n' || i == 0) {
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
    else if (keyPressedEvent->code == sf::Keyboard::Key::Down && caretIndex < style.text.size()) {
        if (computedTextFieldStyle.lineMode == "single") {
            caretIndex = style.text.length();
        }
        else if (computedTextFieldStyle.lineMode == "multi") {
            SFUI::UnsignedInt aboveRowCharsPassed = 0;
            SFUI::UnsignedInt belowRowCharsPassed = 0;
            SFUI::UnsignedInt aboveRowNewlineIndex = 0;
            SFUI::UnsignedInt belowRowNewlineIndex = 0;
            SFUI::UnsignedInt textLength = style.text.size();
            SFUI::Bool caretMoveNeeded = true;

            for (SFUI::Int i = caretIndex - 1; i >= 0; --i) {
                if (style.text[i] == '\n') break;
                ++aboveRowCharsPassed;
            }

            SFUI::Bool aboveRowNewlineMet = false;
            for (SFUI::Int i = caretIndex - 1; i < textLength; ++i) {
                if (!aboveRowNewlineMet) {
                    if (style.text[i] == '\n') {
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
                    if (style.text[i] == '\n') {
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
 * @brief Get the character group for a given character.
 *
 * @param character The character to evaluate.
 *
 * @return The character group identifier (1 for Alphanumeric, 2 for Symbol, 0 otherwise for white space).
 */
SFUI::UnsignedInt SFUI::TextField::getCharacterGroup(const char32_t character) {
    if (CTRL_ALPHANUMERIC_GROUP.find(character) != sf::String::InvalidPos) return 1;
    if (CTRL_SYMBOL_GROUP.find(character) != sf::String::InvalidPos) return 2;
    return 0;
}
