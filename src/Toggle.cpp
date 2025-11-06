#include "components/Toggle.hpp"


////////////////////////////
// Toggle Component Class //
////////////////////////////


/**
 * @brief The constructor for the Toggle component.
 * 
 * @param componentID The unique identifier for the Toggle component.
 */
SFUI::Toggle::Toggle(const SFUI::String& componentID) :
    Component(componentID),
    toggleStyle(),
    toggleState(),
    toggleBehavior(),
    toggle(componentID + "_InnerToggle")
{}


/**
 * @brief The constructor for the Toggle component.
 * 
 * @param componentID The unique identifier for the Toggle component.
 * @param togglePropGroup The property group containing layout, style, state, and behavior properties for the Toggle component.
 */
SFUI::Toggle::Toggle(const SFUI::String& componentID, const SFUI::PropGroup::Toggle& togglePropGroup) :
    Component(componentID, togglePropGroup.layout, togglePropGroup.style),
    toggleStyle(togglePropGroup.toggleStyle),
    toggleState(togglePropGroup.toggleState),
    toggleBehavior(togglePropGroup.toggleBehavior),
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
 * @brief Compute dynamic colors based on the Toggle's state.
 */
SFUI::Void SFUI::Toggle::computeDynamicColors() {
    if (toggleStyle.disabledFillColor.has_value())
        toggle.buttonStyle.disabledFillColor = resolveColorSubProp(toggleStyle.disabledFillColor.value());
    if (toggleStyle.disabledBorderColor.has_value())
        toggle.buttonStyle.disabledBorderColor = resolveColorSubProp(toggleStyle.disabledBorderColor.value());
    if (toggleState.isOn) {
        toggle.style.fillColor = resolveColorSubProp(toggleStyle.onFillColor);
        toggle.style.borderColor = resolveColorSubProp(toggleStyle.onBorderColor);
        if (toggleStyle.hoveredOnFillColor.has_value())
            toggle.buttonStyle.hoveredFillColor = resolveColorSubProp(toggleStyle.hoveredOnFillColor.value());
        if (toggleStyle.hoveredOnBorderColor.has_value())
            toggle.buttonStyle.hoveredBorderColor = resolveColorSubProp(toggleStyle.hoveredOnBorderColor.value());
        if (toggleStyle.pressedOnFillColor.has_value())
            toggle.buttonStyle.pressedFillColor = resolveColorSubProp(toggleStyle.pressedOnFillColor.value());
        if (toggleStyle.pressedOnBorderColor.has_value())
            toggle.buttonStyle.pressedBorderColor = resolveColorSubProp(toggleStyle.pressedOnBorderColor.value());
    }
    else if (!toggleState.isOn) {
        toggle.style.fillColor = resolveColorSubProp(toggleStyle.offFillColor);
        toggle.style.borderColor = resolveColorSubProp(toggleStyle.offBorderColor);
        if (toggleStyle.hoveredOffFillColor.has_value())
            toggle.buttonStyle.hoveredFillColor = resolveColorSubProp(toggleStyle.hoveredOffFillColor.value());
        if (toggleStyle.hoveredOffBorderColor.has_value())
            toggle.buttonStyle.hoveredBorderColor = resolveColorSubProp(toggleStyle.hoveredOffBorderColor.value());
        if (toggleStyle.pressedOffFillColor.has_value())
            toggle.buttonStyle.pressedFillColor = resolveColorSubProp(toggleStyle.pressedOffFillColor.value());
        if (toggleStyle.pressedOffBorderColor.has_value())
            toggle.buttonStyle.pressedBorderColor = resolveColorSubProp(toggleStyle.pressedOffBorderColor.value());
    }
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
    toggle.style = this->style;
    toggle.buttonStyle.focusWidth = toggleStyle.focusWidth;
    toggle.buttonStyle.focusOffset = toggleStyle.focusOffset;
    toggle.buttonStyle.focusCornerRadius = toggleStyle.focusCornerRadius;
    toggle.buttonStyle.focusFillColor = toggleStyle.focusFillColor;
    toggle.buttonStyle.toolTipPadding = toggleStyle.toolTipPadding;
    toggle.buttonStyle.toolTipCornerRadius = toggleStyle.toolTipCornerRadius;
    toggle.buttonStyle.toolTipText = toggleStyle.toolTipText;
    toggle.buttonStyle.toolTipFont = toggleStyle.toolTipFont;
    toggle.buttonStyle.toolTipTextSize = toggleStyle.toolTipTextSize;
    toggle.buttonStyle.toolTipFillColor = toggleStyle.toolTipFillColor;
    toggle.buttonStyle.toolTipTextColor = toggleStyle.toolTipTextColor;

    // State //
    toggle.buttonState.isDisabled = toggleState.isDisabled;
    toggle.buttonState.isFocused = toggleState.isFocused;
    
    // Behavior //
    toggle.buttonBehavior.onEnable = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onEnable) toggleBehavior.onEnable(this->componentID);
    };
    toggle.buttonBehavior.onDisable = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onDisable) toggleBehavior.onDisable(this->componentID);
    };
    toggle.buttonBehavior.onFocus = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onFocus) toggleBehavior.onFocus(this->componentID);
    };
    toggle.buttonBehavior.onBlur = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onBlur) toggleBehavior.onBlur(this->componentID);
    };
    toggle.buttonBehavior.onHoverIn = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onHoverIn) toggleBehavior.onHoverIn(this->componentID);
    };
    toggle.buttonBehavior.onHoverOut = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onHoverOut) toggleBehavior.onHoverOut(this->componentID);
    };
    toggle.buttonBehavior.onLeftPressIn = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onLeftPressIn) toggleBehavior.onLeftPress(this->componentID);
    };
    toggle.buttonBehavior.onLeftPress = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onLeftPress) toggleBehavior.onLeftPress(this->componentID);
        toggleState.isOn = !toggleState.isOn;
        if (toggleBehavior.onToggledState) toggleBehavior.onToggledState(this->componentID, toggleState.isOn);
    };
    toggle.buttonBehavior.onRightPressIn = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onRightPressIn) toggleBehavior.onRightPressIn(this->componentID);
    };
    toggle.buttonBehavior.onRightPress = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onRightPress) toggleBehavior.onRightPress(this->componentID);
    };
    toggle.buttonBehavior.onMiddlePressIn = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onMiddlePressIn) toggleBehavior.onMiddlePressIn(this->componentID);
    };
    toggle.buttonBehavior.onMiddlePress = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onMiddlePress) toggleBehavior.onMiddlePress(this->componentID);
    };
    toggle.buttonBehavior.onDoublePress = [this](const SFUI::String& componentID) {
        if (toggleBehavior.onDoublePress) toggleBehavior.onDoublePress(this->componentID);
    };
    toggle.buttonBehavior.onKeyPress = [this](const SFUI::String& componentID, sf::Keyboard::Key pressedKey) {
        if (toggleBehavior.onKeyPress) toggleBehavior.onKeyPress(this->componentID, pressedKey);
    };

    // Update //
    toggle.update(renderTargetSize);
}
