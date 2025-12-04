/**
 * @file Button.cpp
 * @brief Implements the SFUI Button component.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.0
 *
 * This file contains the function definitions and internal logic for the
 * SFUI Button component. It handles:
 *   - Construction and destruction
 *   - Event handling (click, hover)
 *   - Rendering and style updates
 *
 * It works in conjunction with Button.hpp to provide full functionality
 * of the Button component.
 */


#include "Components/Button.hpp"


/**
 * @brief Time threshold to detect a double-press event.
 */
const SFUI::Time SFUI::Button::DOUBLE_PRESS_GAP_MS = sf::milliseconds(300);


/**
 * @brief Hovering time threshold before showing the tool-tip.
 */
const SFUI::Time SFUI::Button::TOOL_TIP_THRESHOLD_MS = sf::milliseconds(1000);


/**
 * @brief The constructor for Button.
 *
 * @param componentID The unique identifier for the button component.
 */
SFUI::Button::Button(SFUI::String componentID) :
    Component(componentID),
    focus(componentID + "_Focus"),
    toolTip(componentID + "_ToolTip")
{}


/**
 * @brief Handle events for the Button component.
 *
 * @param event The event to handle.
 */
SFUI::Void SFUI::Button::handleEvent(const SFUI::Event& event) {
    if (state.isDisabled) return;

    // Mouse Moved Event Handling //
    if (const SFUI::Event::MouseMoved* mouseMovedEvent = event.getIf<SFUI::Event::MouseMoved>()) {
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseMovedEvent->position.x, mouseMovedEvent->position.y);
        SFUI::Bool buttonHovered = isMouseHovered(mousePosition);

        if (buttonHovered) {
            if (!isHovered) {
                toolTipClock.restart();
                isHovered = true;
                if (behavior.onHoverIn) behavior.onHoverIn(componentID);
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
                if (behavior.onHoverOut) behavior.onHoverOut(componentID);
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
            if (state.isFocused) {
                state.isFocused = false;
                if (behavior.onBlur) behavior.onBlur(componentID);
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
                    if (behavior.onLeftPressIn) behavior.onLeftPressIn(componentID);
                }
            }   else if (mouseButton == sf::Mouse::Button::Right) {
                if (!isRightPressed) {
                    isRightPressed = true;
                    if (behavior.onRightPressIn) behavior.onRightPressIn(componentID);
                }
            }   else if (mouseButton == sf::Mouse::Button::Middle) {
                if (!isMiddlePressed) {
                    isMiddlePressed = true;
                    if (behavior.onMiddlePressIn) behavior.onMiddlePressIn(componentID);
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
                if (behavior.onDoublePress) behavior.onDoublePress(componentID);
                doublePressClock.restart();
                previousPressTime = SFUI::Time::Zero;
            }
            else {
                if (mouseButton == sf::Mouse::Button::Left && isLeftPressed && behavior.onLeftPress) behavior.onLeftPress(componentID);
                if (mouseButton == sf::Mouse::Button::Right && isRightPressed && behavior.onRightPress) behavior.onRightPress(componentID);
                if (mouseButton == sf::Mouse::Button::Middle && isMiddlePressed && behavior.onMiddlePress) behavior.onMiddlePress(componentID);
                doublePressClock.restart();
                previousPressPosition = mousePosition;
            }
        }
        if (isLeftPressed || isRightPressed || isMiddlePressed) {
            isLeftPressed = isRightPressed = isMiddlePressed = false;
        }
    }

    // Key Pressed Event Handling //
    if (const SFUI::Event::KeyPressed* keyPressedEvent = event.getIf<SFUI::Event::KeyPressed>()) {
        if (state.isFocused && behavior.onKeyPress)
            behavior.onKeyPress(componentID, keyPressedEvent->code);
        if (isShowingToolTip) {
            isShowingToolTip = false;
            toolTipClock.restart();
            toolTipClock.stop();
            if (behavior.onBlur) behavior.onBlur(componentID);
        }
    }
}


/**
 * @brief Handle the pre updaate updates for the component.
 */
SFUI::Void SFUI::Button::preUpdate() {
    this->baseLayout = this->layout;
    this->baseStyle = this->style;
    this->baseState = this->state;
    focus.preUpdate();
    toolTip.preUpdate();
}


/**
 * @brief Recalculate the Button's properties.
 *
 * @param renderTargetSize The size of the render target.
 */
SFUI::Void SFUI::Button::update(const SFUI::Vector2u renderTargetSize) {
    this->renderTargetSize = renderTargetSize;
    computeDynamicColors();
    computeAlignment();
    computeLayoutBox();
    computeStyles();
    computeShadows();
    computeGraphics();
    computeChildrenLayoutBox();
    updateChildren();
    computeFocus();
    computeToolTip();
}


/**
 * @brief Draw the Button component.
 *
 * @param drawTarget The render target to draw on.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::Button::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    drawTarget.draw(shadowRects);
    drawTarget.draw(shadowArcs);
    drawTarget.draw(backgroundRects);
    drawTarget.draw(backgroundArcs);
    drawTarget.draw(borderRects);
    drawTarget.draw(borderArcs);
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
SFUI::Void SFUI::Button::drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    if (state.isFocused) focus.draw(drawTarget, window);
    if (!style.toolTipText.empty() && isShowingToolTip) toolTip.draw(drawTarget, window);
}


/**
 * @brief Compute dynamic colors based on button state.
 */
SFUI::Void SFUI::Button::computeDynamicColors() {
    computedStyle.fillColor = resolveColorSubProp(style.fillColor);
    computedStyle.borderColor = resolveColorSubProp(style.borderColor);
    if (state.isDisabled) {
        if (style.disabledFillColor.has_value())
            computedStyle.fillColor = resolveColorSubProp(style.disabledFillColor.value());
        if (style.disabledBorderColor.has_value())
            computedStyle.borderColor = resolveColorSubProp(style.disabledBorderColor.value());
    }
    else if ((isLeftPressed || isRightPressed || isMiddlePressed) && isHovered) {
        if (style.pressedFillColor.has_value())
            computedStyle.fillColor = resolveColorSubProp(style.pressedFillColor.value());
        if (style.pressedBorderColor.has_value())
            computedStyle.borderColor = resolveColorSubProp(style.pressedBorderColor.value());
    }
    else if (isHovered) {
        if (style.hoveredFillColor.has_value())
            computedStyle.fillColor = resolveColorSubProp(style.hoveredFillColor.value());
        if (style.hoveredBorderColor.has_value())
            computedStyle.borderColor = resolveColorSubProp(style.hoveredBorderColor.value());
    }
}


/**
 * @brief Compute the focus graphics for the Button component.
 */
SFUI::Void SFUI::Button::computeFocus() {
    // Focus Width //
    SFUI::Float computedFocusWidth = 0.0f;
    if (std::holds_alternative<SFUI::Float>(style.focusWidth))
        computedFocusWidth = std::get<SFUI::Float>(style.focusWidth);
    else if (std::holds_alternative<SFUI::String>(style.focusWidth)) {
        SFUI::String focusWidthString = std::get<SFUI::String>(style.focusWidth);
        if (focusWidthString.size() > 1 && focusWidthString.back() == '%') {
            focusWidthString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempFocusWidth = std::stod(focusWidthString, &index);
                if (index == focusWidthString.size()) {
                    SFUI::Float relativeFocusWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedFocusWidth = relativeFocusWidthFactor * std::clamp(static_cast<SFUI::Float>(tempFocusWidth) / 100.0f, 0.0f, 0.5f);
                }
            }   catch (...) {}
        }
    }

    // Focus Offset //
    SFUI::Float computedFocusOffset = 0.0f;
    if (std::holds_alternative<SFUI::Float>(style.focusOffset))
        computedFocusOffset = std::get<SFUI::Float>(style.focusOffset);
    else if (std::holds_alternative<SFUI::String>(style.focusOffset)) {
        SFUI::String focusOffsetString = std::get<SFUI::String>(style.focusOffset);
        if (focusOffsetString.size() > 1 && focusOffsetString.back() == '%') {
            focusOffsetString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempFocusOffset = std::stod(focusOffsetString, &index);
                if (index == focusOffsetString.size()) {
                    SFUI::Float relativeFocusOffsetFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedFocusOffset = relativeFocusOffsetFactor * std::clamp(static_cast<SFUI::Float>(tempFocusOffset) / 100.0f, 0.0f, 0.5f);
                }
            }   catch (...) {}
        }
    }

    // Focus Prop Channeling //
    focus.layout.width = computedLayout.size.x + (computedFocusOffset * 2.0f) + (computedFocusWidth * 2.0f);
    focus.layout.height = computedLayout.size.y + (computedFocusOffset * 2.0f) + (computedFocusWidth * 2.0f);
    focus.layout.xPosition = computedLayout.position.x - computedFocusOffset - computedFocusWidth;
    focus.layout.yPosition = computedLayout.position.y - computedFocusOffset - computedFocusWidth;
    focus.style.borderWidth = computedFocusWidth;
    focus.style.cornerRadius = style.focusCornerRadius;
    focus.style.fillColor = SFUI::Color(0, 0, 0, 0);
    focus.style.borderColor = style.focusFillColor;
    focus.update(renderTargetSize);
}


/**
 * @brief Compute the tool tip for the Button component.
 */
SFUI::Void SFUI::Button::computeToolTip() {
    // Tool Tip Lifetime //
    if (style.toolTipText != "" && style.toolTipFont) {
        if ((isHovered || state.isFocused) && !isShowingToolTip) {
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

    // Tool Tip Prop Channeling //
    SFUI::Vector4f toolTipPadding = resolveUniQuadSubProp(computedLayout.size, style.toolTipPadding);
    toolTip.layout.width = toolTip.getTextBounds().size.x + (toolTipPadding.x + toolTipPadding.y);
    toolTip.layout.height = toolTip.getTextBounds().size.y + (toolTipPadding.z + toolTipPadding.w);
    toolTip.style.textAlignHorizontal = "center";
    toolTip.style.textAlignVertical = "center";
    toolTip.style.cornerRadius = style.toolTipCornerRadius;
    toolTip.style.text = style.toolTipText;
    toolTip.style.font = style.toolTipFont;
    toolTip.style.textSize = style.toolTipTextSize;
    toolTip.style.fillColor = style.toolTipFillColor;
    toolTip.style.textColor = style.toolTipTextColor;
    toolTip.update(renderTargetSize);
}
