/**
 * @file Toggle.cpp
 * @brief Implements the SFUI Toggle component.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.1
 *
 * This file contains the function definitions and internal logic for the
 * SFUI Toggle component. It handles:
 *   - Construction and destruction
 *   - Event handling (toggle on/off)
 *   - Rendering and style updates
 *
 * It works in conjunction with Toggle.hpp to provide full functionality
 * of the Toggle component.
 */


#include "Components/Toggle.hpp"


/**
 * @brief The constructor for the Toggle component.
 *
 * @param componentID The unique identifier for the Toggle component.
 */
SFUI::Toggle::Toggle(SFUI::String componentID) :
    Component(componentID),
    toggle(componentID + "_InnerToggle")
{}


/**
 * @brief Handle a user input event.
 *
 * @param event The event to handle.
 */
SFUI::Void SFUI::Toggle::handleEvent(const SFUI::Event& event) {
    toggle.handleEvent(event);
}


/**
 * @brief Handle the pre updaate updates for the component.
 */
SFUI::Void SFUI::Toggle::preUpdate() {
    this->baseLayout = this->layout;
    this->baseStyle = this->style;
    this->baseState = this->state;
    toggle.preUpdate();
}


/**
 * @brief Recalculate the Toggle component's properties.
 *
 * @param renderTargetSize The size of the render target.
 */
SFUI::Void SFUI::Toggle::update(const SFUI::Vector2u renderTargetSize) {
    this->renderTargetSize = renderTargetSize;
    computeAlignment();
    computeLayoutBox();
    computeStyles();
    computeShadows();
    computeChildrenLayoutBox();
    updateChildren();
    computeDynamicColors();
    computeToggle();
}


/**
 * @brief Draw the Toggle component.
 *
 * @param drawTarget The render target to draw to.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::Toggle::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    toggle.draw(drawTarget, window);
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
SFUI::Void SFUI::Toggle::drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    toggle.drawOverlay(drawTarget, window);
}


/**
 * @brief Compute dynamic colors based on the Toggle's state.
 */
SFUI::Void SFUI::Toggle::computeDynamicColors() {
    if (state.isOn) {
        toggle.style.fillColor = style.onFillColor;
        toggle.style.borderColor = style.onBorderColor;
        if (style.hoveredOnFillColor.has_value())
            toggle.style.hoveredFillColor = style.hoveredOnFillColor.value();
        if (style.hoveredOnBorderColor.has_value())
            toggle.style.hoveredBorderColor = style.hoveredOnBorderColor.value();
        if (style.pressedOnFillColor.has_value())
            toggle.style.pressedFillColor = style.pressedOnFillColor.value();
        if (style.pressedOnBorderColor.has_value())
            toggle.style.pressedBorderColor = style.pressedOnBorderColor.value();
    }
    else if (!state.isOn) {
        toggle.style.fillColor = style.offFillColor;
        toggle.style.borderColor = style.offBorderColor;
        if (style.hoveredOffFillColor.has_value())
            toggle.style.hoveredFillColor = style.hoveredOffFillColor.value();
        if (style.hoveredOffBorderColor.has_value())
            toggle.style.hoveredBorderColor = style.hoveredOffBorderColor.value();
        if (style.pressedOffFillColor.has_value())
            toggle.style.pressedFillColor = style.pressedOffFillColor.value();
        if (style.pressedOffBorderColor.has_value())
            toggle.style.pressedBorderColor = style.pressedOffBorderColor.value();
    }
    if (style.disabledFillColor.has_value())
        toggle.style.disabledFillColor = style.disabledFillColor.value();
    if (style.disabledBorderColor.has_value())
        toggle.style.disabledBorderColor = style.disabledBorderColor.value();
}


/**
 * @brief Compute the Toggle component's internal Button properties.
 */
SFUI::Void SFUI::Toggle::computeToggle() {
    // Layout //
    toggle.layout.width = computedLayout.size.x;
    toggle.layout.height = computedLayout.size.y;
    toggle.layout.xPosition = computedLayout.position.x;
    toggle.layout.yPosition = computedLayout.position.y;

    // Style //
    toggle.style.borderWidth = style.borderWidth;
    toggle.style.cornerRadius = style.cornerRadius;
    toggle.style.shadowOffset = style.shadowOffset;
    toggle.style.shadowRadius = style.shadowRadius;
    toggle.style.shadowFillColor = style.shadowFillColor;
    toggle.style.focusWidth = style.focusWidth;
    toggle.style.focusOffset = style.focusOffset;
    toggle.style.focusCornerRadius = style.focusCornerRadius;
    toggle.style.focusFillColor = style.focusFillColor;
    toggle.style.toolTipPadding = style.toolTipPadding;
    toggle.style.toolTipCornerRadius = style.toolTipCornerRadius;
    toggle.style.toolTipText = style.toolTipText;
    toggle.style.toolTipFont = style.toolTipFont;
    toggle.style.toolTipTextSize = style.toolTipTextSize;
    toggle.style.toolTipFillColor = style.toolTipFillColor;
    toggle.style.toolTipTextColor = style.toolTipTextColor;

    // State //
    toggle.state.isDisabled = state.isDisabled;
    toggle.state.isFocused = state.isFocused;

    // Behavior //
    toggle.behavior.onEnable = [this](const SFUI::String& componentID) {
        if (behavior.onEnable) behavior.onEnable(this->componentID);
    };
    toggle.behavior.onDisable = [this](const SFUI::String& componentID) {
        if (behavior.onDisable) behavior.onDisable(this->componentID);
    };
    toggle.behavior.onFocus = [this](const SFUI::String& componentID) {
        if (behavior.onFocus) behavior.onFocus(this->componentID);
    };
    toggle.behavior.onBlur = [this](const SFUI::String& componentID) {
        if (behavior.onBlur) behavior.onBlur(this->componentID);
    };
    toggle.behavior.onHoverIn = [this](const SFUI::String& componentID) {
        if (behavior.onHoverIn) behavior.onHoverIn(this->componentID);
    };
    toggle.behavior.onHoverOut = [this](const SFUI::String& componentID) {
        if (behavior.onHoverOut) behavior.onHoverOut(this->componentID);
    };
    toggle.behavior.onLeftPressIn = [this](const SFUI::String& componentID) {
        if (behavior.onLeftPressIn) behavior.onLeftPress(this->componentID);
    };
    toggle.behavior.onLeftPress = [this](const SFUI::String& componentID) {
        if (behavior.onLeftPress) behavior.onLeftPress(this->componentID);
        state.isOn = !state.isOn;
        if (behavior.onToggledState) behavior.onToggledState(this->componentID, state.isOn);
    };
    toggle.behavior.onRightPressIn = [this](const SFUI::String& componentID) {
        if (behavior.onRightPressIn) behavior.onRightPressIn(this->componentID);
    };
    toggle.behavior.onRightPress = [this](const SFUI::String& componentID) {
        if (behavior.onRightPress) behavior.onRightPress(this->componentID);
    };
    toggle.behavior.onMiddlePressIn = [this](const SFUI::String& componentID) {
        if (behavior.onMiddlePressIn) behavior.onMiddlePressIn(this->componentID);
    };
    toggle.behavior.onMiddlePress = [this](const SFUI::String& componentID) {
        if (behavior.onMiddlePress) behavior.onMiddlePress(this->componentID);
    };
    toggle.behavior.onDoublePress = [this](const SFUI::String& componentID) {
        if (behavior.onDoublePress) behavior.onDoublePress(this->componentID);
    };
    toggle.behavior.onKeyPress = [this](SFUI::String componentID, sf::Keyboard::Key pressedKey) {
        if (behavior.onKeyPress) behavior.onKeyPress(this->componentID, pressedKey);
    };

    // Update //
    toggle.update(renderTargetSize);
}
