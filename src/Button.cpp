#include <sfml-flexui.hpp>


////////////////////////////////////////
// SFML-FlexUI Button Component Class //
////////////////////////////////////////


// Static Variables //
SFUI::Float SFUI::Button::TEXT_VERTICAL_OFFSET_FACTOR = 0.65f;
SFUI::Time SFUI::Button::LONG_PRESS_THRESHOLD_MS = sf::milliseconds(500);
SFUI::Time SFUI::Button::DOUBLE_PRESS_GAP_MS = sf::milliseconds(300);
SFUI::Time SFUI::Button::TOOL_TIP_THRESHOLD_MS = sf::milliseconds(1000);


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID) :
    Component(componentID),
    buttonStyle(),
    buttonBehavior(),
    focus(componentID + "Focus"),
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
 */
SFUI::Void SFUI::Button::computeDynamicFillColor() {
    if (isDisabled && buttonStyle.disabledFillColor.has_value())
        computedStyle.fillColor = resolveColorSubProp(buttonStyle.disabledFillColor.value());
    else if ((isLeftPressed || isRightPressed || isMiddlePressed) && buttonStyle.pressedFillColor.has_value())
        computedStyle.fillColor = resolveColorSubProp(buttonStyle.pressedFillColor.value());
    else if ((isHovered) && buttonStyle.hoveredFillColor.has_value())
        computedStyle.fillColor = resolveColorSubProp(buttonStyle.hoveredFillColor.value());
    else
        computedStyle.fillColor = resolveColorSubProp(style.fillColor);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeDynamicBorderColor() {
    if (isDisabled && buttonStyle.disabledBorderColor.has_value())
        computedStyle.borderColor = resolveColorSubProp(buttonStyle.disabledBorderColor.value());
    else if ((isLeftPressed || isRightPressed || isMiddlePressed) && buttonStyle.pressedBorderColor.has_value())
        computedStyle.borderColor = resolveColorSubProp(buttonStyle.pressedBorderColor.value());
    else if ((isHovered) && buttonStyle.hoveredBorderColor.has_value())
        computedStyle.borderColor = resolveColorSubProp(buttonStyle.hoveredBorderColor.value());
    else
        computedStyle.borderColor = resolveColorSubProp(style.borderColor);
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
    computedButtonStyle.focusCornerRadius = resolveCornerRadiusSubProp(
        SFUI::Vector2f(
            computedLayout.size.x + computedButtonStyle.focusOffset * 2.0f,
            computedLayout.size.y + computedButtonStyle.focusOffset * 2.0f
        ),
        buttonStyle.focusCornerRadius,
        buttonStyle.focusCornerRadiusTopLeft,
        buttonStyle.focusCornerRadiusTopRight,
        buttonStyle.focusCornerRadiusBottomLeft,
        buttonStyle.focusCornerRadiusBottomRight
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
    focus.style.cornerRadiusTopLeft = computedButtonStyle.focusCornerRadius.x;
    focus.style.cornerRadiusTopRight = computedButtonStyle.focusCornerRadius.y;
    focus.style.cornerRadiusBottomLeft = computedButtonStyle.focusCornerRadius.z;
    focus.style.cornerRadiusBottomRight = computedButtonStyle.focusCornerRadius.w;
    focus.style.fillColor = SFUI::Color(0, 0, 0, 0);
    focus.style.borderColor = computedButtonStyle.focusFillColor;
    focus.update(renderTargetSize);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeToolTipPadding() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;
    
    computedButtonStyle.toolTipPadding = buttonStyle.toolTipPadding;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeToolTipCornerRadius() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;

    computedButtonStyle.toolTipCornerRadius = resolveCornerRadiusSubProp(
        SFUI::Vector2f(
            toolTip.getSize().x,
            toolTip.getSize().y
        ),
        buttonStyle.toolTipCornerRadius,
        buttonStyle.toolTipCornerRadiusTopLeft,
        buttonStyle.toolTipCornerRadiusTopRight,
        buttonStyle.toolTipCornerRadiusBottomLeft,
        buttonStyle.toolTipCornerRadiusBottomRight
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
    toolTip.layout.width = tempText.getGlobalBounds().size.x + (computedButtonStyle.toolTipPadding * 2.0f);
    toolTip.layout.height = tempText.getGlobalBounds().size.y + (computedButtonStyle.toolTipPadding * 2.0f);
    toolTip.layout.padding = computedButtonStyle.toolTipPadding;
    toolTip.style.cornerRadiusTopLeft = computedButtonStyle.toolTipCornerRadius.x;
    toolTip.style.cornerRadiusTopRight = computedButtonStyle.toolTipCornerRadius.y;
    toolTip.style.cornerRadiusBottomLeft = computedButtonStyle.toolTipCornerRadius.z;
    toolTip.style.cornerRadiusBottomRight = computedButtonStyle.toolTipCornerRadius.w;
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
            SFUI::Float xPosition = previousHoverPosition.x - (toolTip.getSize().x + (computedButtonStyle.toolTipPadding * 2.0f));
            SFUI::Float yPosition = previousHoverPosition.y - (toolTip.getSize().y + (computedButtonStyle.toolTipPadding * 2.0f));
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


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Button::update(const SFUI::Vector2u renderTargetSize) {
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
    computeGraphics();
    computeChildrenMargin();
    computeChildrenSize();
    computeChildrenPosition();
    updateChildren();

    // Button Specific Computation //
    computeDynamicFillColor();
    computeDynamicBorderColor();
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
SFUI::Void SFUI::Button::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(backgroundRects);
    renderTarget.draw(backgroundArcs);
    renderTarget.draw(borderRects);
    renderTarget.draw(borderArcs);
    if (isFocused) focus.draw(renderTarget);
    if (isShowingToolTip) toolTip.draw(renderTarget);
}


/**
 * @brief .
 * 
 * @param .
 * 
 * @return .
 */
SFUI::Bool SFUI::Button::isMouseHovered(const SFUI::Vector2i& mousePosition) {
    SFUI::Vector2f mousePos(mousePosition.x, mousePosition.y);
    SFUI::FloatRect cornerBounds = borderRects.getBounds();
    SFUI::FloatRect edgeBounds = backgroundRects.getBounds();
    return (cornerBounds.contains(mousePos) || edgeBounds.contains(mousePos));
}
