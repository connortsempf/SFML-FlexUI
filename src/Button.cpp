#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


////////////////////////////////////////
// SFML-FlexUI Button Component Class //
////////////////////////////////////////


// Static Variables //
const SFUI::Time SFUI::Button::DOUBLE_PRESS_GAP_MS = sf::milliseconds(300);
const SFUI::Time SFUI::Button::TOOL_TIP_THRESHOLD_MS = sf::milliseconds(1000);


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID) :
    Component(componentID),
    buttonStyle(),
    buttonBehavior(),
    focus(componentID + "_Focus"),
    toolTip(componentID + "_ToolTip")
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID, const SFUI::PropGroup::Button& buttonPropGroup) :
    Component(componentID, buttonPropGroup.layout, buttonPropGroup.style),
    buttonStyle(buttonPropGroup.buttonStyle),
    buttonBehavior(buttonPropGroup.buttonBehavior),
    focus(componentID + "Focus"),
    toolTip(componentID + "_ToolTip")
{}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Button::handleEvent(const SFUI::Event& event) {
    if (isDisabled) return;

    // Mouse Moved Event Handling //
    if (const SFUI::Event::MouseMoved* mouseMovedEvent = event.getIf<SFUI::Event::MouseMoved>()) {
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseMovedEvent->position.x, mouseMovedEvent->position.y);
        SFUI::Bool buttonHovered = isMouseHovered(mousePosition);
        
        if (buttonHovered) {
            if (!isHovered) {
                toolTipClock.restart();
                isHovered = true;
                if (buttonBehavior.onHoverIn) buttonBehavior.onHoverIn(componentID);
                // std::cout << componentID << " Hovered In\n";
            }   else if (!isShowingToolTip) {
                previousHoverPosition = {static_cast<SFUI::Float>(mousePosition.x), static_cast<SFUI::Float>(mousePosition.y)};
            }
        }
        else if (!buttonHovered) {
            if (isHovered) {
                isHovered = false;
                if (isShowingToolTip) {
                    isShowingToolTip = false;
                    toolTipClock.stop();
                }
                if (buttonBehavior.onHoverOut) buttonBehavior.onHoverOut(componentID);
            }   else {
                if (isShowingToolTip) {
                    isShowingToolTip = false;
                    toolTipClock.restart();
                    toolTipClock.stop();
                }
            }
        }
    }

    // Mouse Button Pressed Event Handling //
    if (const SFUI::Event::MouseButtonPressed* mousePressedEvent = event.getIf<SFUI::Event::MouseButtonPressed>()) {
        sf::Mouse::Button mouseButton = mousePressedEvent->button;
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mousePressedEvent->position.x, mousePressedEvent->position.y);
        SFUI::Bool buttonHovered = isMouseHovered(mousePosition);

        if (mouseButton == sf::Mouse::Button::Left) {
            if (isShowingToolTip) {
                isShowingToolTip = false;
                toolTipClock.restart();
                toolTipClock.stop();
            }
            if (isFocused) {
                isFocused = false;
                if (buttonBehavior.onBlur) buttonBehavior.onBlur(componentID);
            }
        }
        if (buttonHovered) {
            if (isShowingToolTip) {
                isShowingToolTip = false;
                toolTipClock.restart();
                toolTipClock.stop();
            }
            if (mouseButton == sf::Mouse::Button::Left) {
                toolTipClock.stop();
                if (!isLeftPressed) {
                    isLeftPressed = true;
                    if (buttonBehavior.onLeftPressIn) buttonBehavior.onLeftPressIn(componentID);
                }
            }   else if (mouseButton == sf::Mouse::Button::Right) {
                if (!isRightPressed) {
                    isRightPressed = true;
                    if (buttonBehavior.onRightPressIn) buttonBehavior.onRightPressIn(componentID);
                }
            }   else if (mouseButton == sf::Mouse::Button::Middle) {
                if (!isMiddlePressed) {
                    isMiddlePressed = true;
                    if (buttonBehavior.onMiddlePressIn) buttonBehavior.onMiddlePressIn(componentID);
                }
            }
        }
    }

    // Mouse Button Released Event Handling //
    if (const SFUI::Event::MouseButtonReleased* mouseReleasedEvent = event.getIf<SFUI::Event::MouseButtonReleased>()) {
        sf::Mouse::Button mouseButton = mouseReleasedEvent->button;
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseReleasedEvent->position.x, mouseReleasedEvent->position.y);
        SFUI::Bool buttonHovered = isMouseHovered(mousePosition);

        if (buttonHovered) {
            SFUI::Time elapsed = doublePressClock.getElapsedTime();
            if (elapsed < DOUBLE_PRESS_GAP_MS && std::abs(mousePosition.x - previousPressPosition.x) < 4 && std::abs(mousePosition.y - previousPressPosition.y) < 4) {
                if (buttonBehavior.onDoublePress) buttonBehavior.onDoublePress(componentID);
                doublePressClock.restart();
                previousPressTime = SFUI::Time::Zero;
            }
            else {
                if (mouseButton == sf::Mouse::Button::Left && isLeftPressed && buttonBehavior.onLeftPress) buttonBehavior.onLeftPress(componentID);
                if (mouseButton == sf::Mouse::Button::Right && isRightPressed && buttonBehavior.onRightPress) buttonBehavior.onRightPress(componentID);
                if (mouseButton == sf::Mouse::Button::Middle && isMiddlePressed && buttonBehavior.onMiddlePress) buttonBehavior.onMiddlePress(componentID);
                doublePressClock.restart();
                previousPressPosition = mousePosition;
            }
        }
        if (isLeftPressed) isLeftPressed = false;
        if (isRightPressed) isRightPressed = false;
        if (isMiddlePressed) isMiddlePressed = false;
    }

    // Key Pressed Event Handling //
    if (const SFUI::Event::KeyPressed* keyPressedEvent = event.getIf<SFUI::Event::KeyPressed>()) {
        if (isFocused && buttonBehavior.onKeyPress)
            buttonBehavior.onKeyPress(componentID, keyPressedEvent->code);
        if (isShowingToolTip) {
            isShowingToolTip = false;
            toolTipClock.restart();
            toolTipClock.stop();
            if (buttonBehavior.onBlur) buttonBehavior.onBlur(componentID);
        }
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Button::update(const SFUI::Vector2u renderTargetSize) {
    this->renderTargetSize = renderTargetSize;

    // Button Specific Computation (Needs to be Before Graphics Computation) //
    computeDynamicColors();

    computeAlignDirection();
    computeAlignPrimary();
    computeAlignSecondary();
    computeMargin();
    computeSize();
    computePadding();
    computePosition();
    computeBorderWidth();
    computeCornerRadius();
    computeGraphics();
    computeChildrenMargin();
    computeChildrenSize();
    computeChildrenPosition();
    updateChildren();
    
    // Button Specific Computation //
    computeFocusWidth();
    computeFocusOffset();
    computeFocusCornerRadius();
    computeFocusFillColor();
    computeFocus();
    computeToolTipPadding();
    computeToolTipCornerRadius();
    computeToolTipTextSize();
    computeToolTipFillColor();
    computeToolTipTextColor();
    computeToolTip();
    computeToolTipLifetime();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Button::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(shadowRects);
    renderTarget.draw(shadowArcs);
    renderTarget.draw(backgroundRects);
    renderTarget.draw(backgroundArcs);
    renderTarget.draw(borderRects);
    renderTarget.draw(borderArcs);
    if (isFocused) focus.draw(renderTarget);
    if (isShowingToolTip && !buttonStyle.toolTipText.empty()) toolTip.draw(renderTarget);
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Button::getHoveredFillColor() {
    return computedButtonStyle.hoveredFillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Button::getHoveredBorderColor() {
    return computedButtonStyle.hoveredBorderColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Button::getPressedFillColor() {
    return computedButtonStyle.pressedFillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Button::getPressedBorderColor() {
    return computedButtonStyle.pressedBorderColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Button::getDisabledFillColor() {
    return computedButtonStyle.disabledFillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Button::getDisabledBorderColor() {
    return computedButtonStyle.disabledBorderColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::Button::getFocusWidth() {
    return computedButtonStyle.focusWidth;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::Button::getFocusOffset() {
    return computedButtonStyle.focusOffset;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector4f SFUI::Button::getFocusCornerRadius() {
    return computedButtonStyle.focusCornerRadius;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Button::getFocusFillColor() {
    return computedButtonStyle.focusFillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector4f SFUI::Button::getToolTipPadding() {
    return computedButtonStyle.toolTipPadding;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector4f SFUI::Button::getToolTipCornerRadius() {
    return computedButtonStyle.toolTipCornerRadius;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::String SFUI::Button::getToolTipText() {
    return buttonStyle.toolTipText;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::SharedPointer<SFUI::Font> SFUI::Button::getToolTipFont() {
    return buttonStyle.toolTipFont;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::Button::getToolTipTextSize() {
    return computedButtonStyle.toolTipTextSize;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Button::getToolTipFillColor() {
    return computedButtonStyle.toolTipFillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Button::getToolTipTextColor() {
    return computedButtonStyle.toolTipTextColor;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeDynamicColors() {
    if (buttonStyle.hoveredFillColor.has_value())
        computedButtonStyle.hoveredFillColor = resolveColorSubProp(buttonStyle.hoveredFillColor.value());
    if (buttonStyle.hoveredBorderColor.has_value())
        computedButtonStyle.hoveredBorderColor = resolveColorSubProp(buttonStyle.hoveredBorderColor.value());
    if (buttonStyle.pressedFillColor.has_value())
        computedButtonStyle.pressedFillColor = resolveColorSubProp(buttonStyle.pressedFillColor.value());
    if (buttonStyle.pressedBorderColor.has_value())
        computedButtonStyle.pressedBorderColor = resolveColorSubProp(buttonStyle.pressedBorderColor.value());
    if (buttonStyle.disabledFillColor.has_value())
        computedButtonStyle.disabledFillColor = resolveColorSubProp(buttonStyle.disabledFillColor.value());
    if (buttonStyle.disabledBorderColor.has_value())
        computedButtonStyle.disabledBorderColor = resolveColorSubProp(buttonStyle.disabledBorderColor.value());

    // Mutate Based Container with Dynamic Fill Color //
    if (isDisabled) {
        if (buttonStyle.disabledFillColor.has_value())
            computedStyle.fillColor = computedButtonStyle.disabledFillColor;
        if (buttonStyle.disabledBorderColor.has_value())
            computedStyle.borderColor = computedButtonStyle.disabledBorderColor;
    }
    else if ((isLeftPressed || isRightPressed || isMiddlePressed)) {
        if (buttonStyle.pressedFillColor.has_value())
            computedStyle.fillColor = computedButtonStyle.pressedFillColor;
        if (buttonStyle.pressedBorderColor.has_value())
            computedStyle.borderColor = computedButtonStyle.pressedBorderColor;
    }
    else if (isHovered) {
        if (buttonStyle.hoveredFillColor.has_value())
            computedStyle.fillColor = computedButtonStyle.hoveredFillColor;
        if (buttonStyle.hoveredBorderColor.has_value())
            computedStyle.borderColor = computedButtonStyle.hoveredBorderColor;
    }
    else {
        computedStyle.fillColor = resolveColorSubProp(style.fillColor);
        computedStyle.borderColor = resolveColorSubProp(style.borderColor);
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeFocusWidth() {
    SFUI::Float computedFocusWidth = 0.0f;
    
    // If the Focus Width Style Variant Holds an Explicit Width Float //
    if (std::holds_alternative<SFUI::Float>(buttonStyle.focusWidth)) {
        computedFocusWidth = std::get<SFUI::Float>(buttonStyle.focusWidth);
    }
    // If the Focus Width Style Variant Holds a String Percentage Input //
    else if (std::holds_alternative<SFUI::String>(buttonStyle.focusWidth)) {
        SFUI::String focusWidthString = std::get<SFUI::String>(buttonStyle.focusWidth);
        if (focusWidthString.size() > 1 && focusWidthString.back() == '%') {
            focusWidthString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempFocusWidth = std::stod(focusWidthString, &index);
                if (index == focusWidthString.size()) {
                    SFUI::Float relativeFocusWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedFocusWidth = relativeFocusWidthFactor * std::clamp(static_cast<SFUI::Float>(tempFocusWidth) / 100.0f, 0.0f, 0.5f);
                }   else {
                    computedFocusWidth = 0.0f;
                }
            }   catch (...) {
                computedFocusWidth = 0.0f;
            }
        }   else {
            computedFocusWidth = 0.0f;
        }
    }

    // Update the Computed Focus Width Style //
    computedButtonStyle.focusWidth = computedFocusWidth;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeFocusOffset() {
    SFUI::Float computedFocusOffset = 0.0f;
    
    // If the Focus Offset Style Variant Holds an Explicit Offset Float //
    if (std::holds_alternative<SFUI::Float>(buttonStyle.focusOffset)) {
        computedFocusOffset = std::get<SFUI::Float>(buttonStyle.focusOffset);
    }
    // If the Focus Offset Style Variant Holds a String Percentage Input //
    else if (std::holds_alternative<SFUI::String>(buttonStyle.focusOffset)) {
        SFUI::String focusOffsetString = std::get<SFUI::String>(buttonStyle.focusOffset);
        if (focusOffsetString.size() > 1 && focusOffsetString.back() == '%') {
            focusOffsetString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempFocusOffset = std::stod(focusOffsetString, &index);
                if (index == focusOffsetString.size()) {
                    SFUI::Float relativeFocusOffsetFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedFocusOffset = relativeFocusOffsetFactor * std::clamp(static_cast<SFUI::Float>(tempFocusOffset) / 100.0f, 0.0f, 0.5f);
                }   else {
                    computedFocusOffset = 0.0f;
                }
            }   catch (...) {
                computedFocusOffset = 0.0f;
            }
        }   else {
            computedFocusOffset = 0.0f;
        }
    }

    // Update the Computed Focus Offset Style //
    computedButtonStyle.focusOffset = computedFocusOffset;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeFocusCornerRadius() {
    computedButtonStyle.focusCornerRadius = resolveUniQuadSubProp(
        SFUI::Vector2f(computedLayout.size.x + computedButtonStyle.focusOffset * 2.0f, computedLayout.size.y + computedButtonStyle.focusOffset * 2.0f),
        buttonStyle.focusCornerRadius
    );
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeFocusFillColor() {
    computedButtonStyle.focusFillColor = resolveColorSubProp(buttonStyle.focusFillColor);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeFocus() {
    focus.layout.width = computedLayout.size.x + (computedButtonStyle.focusOffset * 2.0f) + (computedButtonStyle.focusWidth * 2.0f);
    focus.layout.height = computedLayout.size.y + (computedButtonStyle.focusOffset * 2.0f) + (computedButtonStyle.focusWidth * 2.0f);
    focus.style.borderWidth = computedButtonStyle.focusWidth;
    focus.style.cornerRadius = SFUI::SubProp::Vector4dim{
        computedButtonStyle.focusCornerRadius.x,
        computedButtonStyle.focusCornerRadius.y,
        computedButtonStyle.focusCornerRadius.z,
        computedButtonStyle.focusCornerRadius.w
    };
    focus.style.fillColor = SFUI::Color(0, 0, 0, 0);
    focus.style.borderColor = computedButtonStyle.focusFillColor;
    focus.update(renderTargetSize);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeToolTipPadding() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;
    computedButtonStyle.toolTipPadding = resolveUniQuadSubProp(computedLayout.size, buttonStyle.toolTipPadding);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeToolTipCornerRadius() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;
    computedButtonStyle.toolTipCornerRadius = resolveUniQuadSubProp(
        SFUI::Vector2f(toolTip.getSize().x, toolTip.getSize().y),
        buttonStyle.toolTipCornerRadius
    );
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeToolTipTextSize() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;

    if (buttonStyle.toolTipTextSize > 0.0f)
        computedButtonStyle.toolTipTextSize = buttonStyle.toolTipTextSize;
    else
        computedButtonStyle.toolTipTextSize = 12.0f;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeToolTipFillColor() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;

    computedButtonStyle.toolTipFillColor = resolveColorSubProp(buttonStyle.toolTipFillColor);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeToolTipTextColor() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;

    computedButtonStyle.toolTipTextColor = resolveColorSubProp(buttonStyle.toolTipTextColor);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeToolTip() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;
    SFUI::Text tempText = SFUI::Text(*(buttonStyle.toolTipFont), buttonStyle.toolTipText, computedButtonStyle.toolTipTextSize);
    toolTip.layout.width = tempText.getGlobalBounds().size.x + (computedButtonStyle.toolTipPadding.x + computedButtonStyle.toolTipPadding.y);
    toolTip.layout.height = tempText.getGlobalBounds().size.y + (computedButtonStyle.toolTipPadding.z + computedButtonStyle.toolTipPadding.w);
    toolTip.layout.padding = SFUI::SubProp::Vector4dim{
        computedButtonStyle.toolTipPadding.x,
        computedButtonStyle.toolTipPadding.y,
        computedButtonStyle.toolTipPadding.z,
        computedButtonStyle.toolTipPadding.w
    };;
    toolTip.style.cornerRadius = SFUI::SubProp::Vector4dim{
        computedButtonStyle.toolTipCornerRadius.x,
        computedButtonStyle.toolTipCornerRadius.y,
        computedButtonStyle.toolTipCornerRadius.z,
        computedButtonStyle.toolTipCornerRadius.w
    };
    toolTip.labelStyle.text = buttonStyle.toolTipText;
    toolTip.labelStyle.font = buttonStyle.toolTipFont;
    toolTip.labelStyle.textSize = computedButtonStyle.toolTipTextSize;
    toolTip.style.fillColor = computedButtonStyle.toolTipFillColor;
    toolTip.labelStyle.textColor = computedButtonStyle.toolTipTextColor;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeToolTipLifetime() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;

    if ((isHovered || isFocused) && !isShowingToolTip) {
        sf::Time toolTipElapsed = toolTipClock.getElapsedTime();
        if (toolTipElapsed > TOOL_TIP_THRESHOLD_MS) {
            SFUI::Float xPosition = previousHoverPosition.x - toolTip.getSize().x;
            SFUI::Float yPosition = previousHoverPosition.y - toolTip.getSize().y;
            if (xPosition + toolTip.getSize().x > renderTargetSize.x) xPosition = xPosition - (xPosition + toolTip.getSize().x - renderTargetSize.x);
            if (yPosition + toolTip.getSize().y > renderTargetSize.y) yPosition = yPosition - (yPosition + toolTip.getSize().y - renderTargetSize.y);
            if (xPosition < 0.0f) xPosition = 0.0f;
            if (yPosition < 0.0f) yPosition = 0.0f;
            toolTip.layout.xPosition = xPosition;
            toolTip.layout.yPosition = yPosition;
            isShowingToolTip = true;
        }
        toolTip.update(renderTargetSize);
    }
}
