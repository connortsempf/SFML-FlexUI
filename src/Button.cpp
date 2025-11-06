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
    buttonState(),
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
    buttonState(buttonPropGroup.buttonState),
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
    if (buttonState.isDisabled) return;

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
            if (buttonState.isFocused) {
                buttonState.isFocused = false;
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
        if (buttonState.isFocused && buttonBehavior.onKeyPress)
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

    computeAlignment();
    computeLayoutBox();
    computeStyles();
    computeShadows();
    computeGraphics();
    computeChildrenLayoutBox();
    updateChildren();
    
    // Button Specific Computation //
    computeFocusWidth();
    computeFocusOffset();
    computeToolTipLifetime();
    computeComposedComponents();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Button::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    drawTarget.draw(shadowRects);
    drawTarget.draw(shadowArcs);
    drawTarget.draw(backgroundRects);
    drawTarget.draw(backgroundArcs);
    drawTarget.draw(borderRects);
    drawTarget.draw(borderArcs);
    if (buttonState.isFocused) focus.draw(drawTarget, window);
    if (isShowingToolTip && !buttonStyle.toolTipText.empty()) toolTip.draw(drawTarget, window);
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::SharedPointer<SFUI::Font> SFUI::Button::getToolTipFont() {
    return toolTip.getFont();
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
    if (buttonState.isDisabled) {
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
SFUI::Void SFUI::Button::computeToolTipLifetime() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;

    if ((isHovered || buttonState.isFocused) && !isShowingToolTip) {
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
    }
}


// /**
//  * @brief .
//  */
// SFUI::Void SFUI::Button::computeSystemCursor() {
//     SFUI::String tempSystemCursor = buttonStyle.systemCursor;
//     std::transform(tempSystemCursor.begin(), tempSystemCursor.end(), tempSystemCursor.begin(), [](unsigned char c) {
//         return std::tolower(c);
//     });

//     if (tempSystemCursor == "arrow") { systemCursorHelper = SFUI::Cursor::Type::Arrow; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "arrow-wait") { systemCursorHelper = SFUI::Cursor::Type::ArrowWait; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "wait") { systemCursorHelper = SFUI::Cursor::Type::Wait; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "text") { systemCursorHelper = SFUI::Cursor::Type::Text; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "hand") { systemCursorHelper = SFUI::Cursor::Type::Hand; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-horizontal") { systemCursorHelper = SFUI::Cursor::Type::SizeHorizontal; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-verical") { systemCursorHelper = SFUI::Cursor::Type::SizeVertical; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-top-left-bottom-right") { systemCursorHelper = SFUI::Cursor::Type::SizeTopLeftBottomRight; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-bottom-left-top-right") { systemCursorHelper = SFUI::Cursor::Type::SizeBottomLeftTopRight; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-left") { systemCursorHelper = SFUI::Cursor::Type::SizeLeft; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-right") { systemCursorHelper = SFUI::Cursor::Type::SizeRight; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-top") { systemCursorHelper = SFUI::Cursor::Type::SizeTop; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-bottom") { systemCursorHelper = SFUI::Cursor::Type::SizeBottom; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-top-left") { systemCursorHelper = SFUI::Cursor::Type::SizeTopLeft; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-bottom-right") { systemCursorHelper = SFUI::Cursor::Type::SizeBottomRight; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-bottom-left") { systemCursorHelper = SFUI::Cursor::Type::SizeBottomLeft; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-top-right") { systemCursorHelper = SFUI::Cursor::Type::SizeTopRight; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "size-all") { systemCursorHelper = SFUI::Cursor::Type::SizeAll; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "cross") { systemCursorHelper = SFUI::Cursor::Type::Cross; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "help") { systemCursorHelper = SFUI::Cursor::Type::Help; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else if (tempSystemCursor == "not-allowed") { systemCursorHelper = SFUI::Cursor::Type::NotAllowed; computedButtonStyle.systemCursor = tempSystemCursor; }
//     else { systemCursorHelper = SFUI::Cursor::Type::Arrow; computedButtonStyle.systemCursor = "arrow"; }
// }


// /**
//  * @brief .
//  */
// SFUI::Void SFUI::Button::computeCursor() {
//     // If Custom Cusror Provided -- Default to this Over System Cursor if Both Given //
//     if (buttonStyle.customCursorPath.has_value()) {
//         if (buttonStyle.customCursorPath.value() != computedButtonStyle.customCursorPath) {
//             computedButtonStyle.customCursorPath = buttonStyle.customCursorPath.value();
//             if (!cursorImage.loadFromFile(computedButtonStyle.customCursorPath))
//                 hoverCursorLoadError = true;
//         }
//         else {
//             if (!hoverCursorLoadError) {
//                 if (isDisabled || !isHovered) {
//                     if (auto newCursor = SFUI::Cursor::createFromSystem(SFUI::Cursor::Type::Arrow))
//                         cursor = std::move(*newCursor);
//                 }
//                 else if (!isDisabled && (isHovered || isLeftPressed || isRightPressed || isMiddlePressed)) {
//                     if (auto newCursor = SFUI::Cursor::createFromPixels(cursorImage.getPixelsPtr(), cursorImage.getSize(), {0, 0}))
//                         cursor = std::move(*newCursor);
//                 }
//             }
//         }
//     }
//     // If No Custom Cursor Provided -- Use System Provided Defaults //
//     if (!buttonStyle.customCursorPath.has_value() || hoverCursorLoadError) {
//         if (isDisabled || !isHovered) {
//             if (auto newCursor = SFUI::Cursor::createFromSystem(SFUI::Cursor::Type::Arrow))
//                 cursor = std::move(*newCursor);
//         }
//         else if (!isDisabled && (isHovered || isLeftPressed || isRightPressed || isMiddlePressed)) {
//             if (auto newCursor = SFUI::Cursor::createFromSystem(systemCursorHelper))
//                 cursor = std::move(*newCursor);
//         }
//     }
// }


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeComposedComponents() {
    // Focus //
    focus.layout.width = computedLayout.size.x + (computedButtonStyle.focusOffset * 2.0f) + (computedButtonStyle.focusWidth * 2.0f);
    focus.layout.height = computedLayout.size.y + (computedButtonStyle.focusOffset * 2.0f) + (computedButtonStyle.focusWidth * 2.0f);
    focus.layout.xPosition = computedLayout.position.x - computedButtonStyle.focusOffset - computedButtonStyle.focusWidth;
    focus.layout.yPosition = computedLayout.position.y - computedButtonStyle.focusOffset - computedButtonStyle.focusWidth;
    focus.style.borderWidth = computedButtonStyle.focusWidth;
    focus.style.cornerRadius = buttonStyle.focusCornerRadius;
    focus.style.fillColor = SFUI::Color(0, 0, 0, 0);
    focus.style.borderColor = buttonStyle.focusFillColor;
    focus.update(renderTargetSize);

    // Tool Tip //
    SFUI::Vector4f toolTipPadding = resolveUniQuadSubProp(computedLayout.size, buttonStyle.toolTipPadding);
    toolTip.layout.width = toolTip.getTextBounds().size.x + (toolTipPadding.x + toolTipPadding.y);
    toolTip.layout.height = toolTip.getTextBounds().size.y + (toolTipPadding.z + toolTipPadding.w);
    toolTip.labelStyle.textAlignHorizontal = "center";
    toolTip.labelStyle.textAlignVertical = "center";
    toolTip.style.cornerRadius = buttonStyle.toolTipCornerRadius;
    toolTip.labelStyle.text = buttonStyle.toolTipText;
    toolTip.labelStyle.font = buttonStyle.toolTipFont;
    toolTip.labelStyle.textSize = buttonStyle.toolTipTextSize;
    toolTip.style.fillColor = buttonStyle.toolTipFillColor;
    toolTip.labelStyle.textColor = buttonStyle.toolTipTextColor;
    toolTip.update(renderTargetSize);
}
