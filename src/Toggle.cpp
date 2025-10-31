#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


////////////////////////////////////////
// SFML-FlexUI Toggle Component Class //
////////////////////////////////////////


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Toggle::Toggle(const SFUI::String& componentID) :
    Component(componentID),
    toggleStyle(),
    toggleState(),
    toggleBehavior(),
    toggle(componentID + "_InnerToggle")
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Toggle::Toggle(const SFUI::String& componentID, const SFUI::PropGroup::Toggle& togglePropGroup) :
    Component(componentID, togglePropGroup.layout, togglePropGroup.style),
    toggleStyle(togglePropGroup.toggleStyle),
    toggleState(togglePropGroup.toggleState),
    toggleBehavior(togglePropGroup.toggleBehavior),
    toggle(componentID + "_InnerToggle")
{}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Toggle::handleEvent(const SFUI::Event& event) {
    toggle.handleEvent(event);
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Toggle::update(const SFUI::Vector2u renderTargetSize) {
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
    computeChildrenMargin();
    computeChildrenSize();
    computeChildrenPosition();
    updateChildren();
    
    // Toggle Specific Computation //
    computeDynamicColors();
    computeComposedComponents();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Toggle::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    toggle.draw(drawTarget, window);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Toggle::computeDynamicColors() {
    // Resting Color Styles //
    computedToggleStyle.offFillColor = resolveColorSubProp(toggleStyle.offFillColor);
    computedToggleStyle.offBorderColor = resolveColorSubProp(toggleStyle.offBorderColor);
    computedToggleStyle.onFillColor = resolveColorSubProp(toggleStyle.onFillColor);
    computedToggleStyle.onBorderColor = resolveColorSubProp(toggleStyle.onBorderColor);

    // Hovered Color Styles //
    if (toggleStyle.hoveredOffFillColor.has_value())
        computedToggleStyle.hoveredOffFillColor = resolveColorSubProp(toggleStyle.hoveredOffFillColor.value());
    if (toggleStyle.hoveredOffBorderColor.has_value())
        computedToggleStyle.hoveredOffBorderColor = resolveColorSubProp(toggleStyle.hoveredOffBorderColor.value());
    if (toggleStyle.hoveredOnFillColor.has_value())
        computedToggleStyle.hoveredOnFillColor = resolveColorSubProp(toggleStyle.hoveredOnFillColor.value());
    if (toggleStyle.hoveredOnBorderColor.has_value())
        computedToggleStyle.hoveredOnBorderColor = resolveColorSubProp(toggleStyle.hoveredOnBorderColor.value());
    
    // Pressed Color Styles //
    if (toggleStyle.pressedOffFillColor.has_value())
        computedToggleStyle.pressedOffFillColor = resolveColorSubProp(toggleStyle.pressedOffFillColor.value());
    if (toggleStyle.pressedOffBorderColor.has_value())
        computedToggleStyle.pressedOffBorderColor = resolveColorSubProp(toggleStyle.pressedOffBorderColor.value());
    if (toggleStyle.pressedOnFillColor.has_value())
        computedToggleStyle.pressedOnFillColor = resolveColorSubProp(toggleStyle.pressedOnFillColor.value());
    if (toggleStyle.pressedOnBorderColor.has_value())
        computedToggleStyle.pressedOnBorderColor = resolveColorSubProp(toggleStyle.pressedOnBorderColor.value());
}


/**
 * @brief .
 */
SFUI::Void SFUI::Toggle::computeComposedComponents() {
    toggle.layout = SFUI::Prop::Layout::Component{
        .width = computedLayout.size.x,
        .height = computedLayout.size.y,
        .xPosition = computedLayout.position.x,
        .yPosition = computedLayout.position.y
    };
    toggle.style = this->style;
    toggle.style.fillColor = toggleState.isOn ? computedToggleStyle.onFillColor : computedToggleStyle.offFillColor;
    toggle.style.borderColor = toggleState.isOn ? computedToggleStyle.onBorderColor : computedToggleStyle.offBorderColor;
    toggle.buttonStyle = SFUI::Prop::Style::Button{
        .hoveredFillColor = toggleState.isOn ? computedToggleStyle.hoveredOnFillColor : computedToggleStyle.hoveredOffFillColor,
        .hoveredBorderColor = toggleState.isOn ? computedToggleStyle.hoveredOnBorderColor : computedToggleStyle.hoveredOffBorderColor,
        .pressedFillColor = toggleState.isOn ? computedToggleStyle.pressedOnFillColor : computedToggleStyle.pressedOffFillColor,
        .pressedBorderColor = toggleState.isOn ? computedToggleStyle.pressedOnBorderColor : computedToggleStyle.pressedOffBorderColor,
        .disabledFillColor = toggleStyle.disabledFillColor,
        .disabledBorderColor = toggleStyle.disabledBorderColor,
        .focusWidth = toggleStyle.focusWidth,
        .focusOffset = toggleStyle.focusOffset,
        .focusCornerRadius = toggleStyle.focusCornerRadius,
        .focusFillColor = toggleStyle.focusFillColor,
        .toolTipPadding = toggleStyle.toolTipPadding,
        .toolTipCornerRadius = toggleStyle.toolTipCornerRadius,
        .toolTipText = toggleStyle.toolTipText,
        .toolTipFont = toggleStyle.toolTipFont,
        .toolTipTextSize = toggleStyle.toolTipTextSize,
        .toolTipFillColor = toggleStyle.toolTipFillColor,
        .toolTipTextColor = toggleStyle.toolTipTextColor
    };
    toggle.buttonState = SFUI::Prop::State::Button{
        .isDisabled = toggleState.isDisabled,
        .isFocused = toggleState.isFocused
    };
    toggle.buttonBehavior = SFUI::Prop::Behavior::Button{
        .onEnable = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onEnable) toggleBehavior.onEnable(this->componentID);
        },
        .onDisable = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onDisable) toggleBehavior.onDisable(this->componentID);
        },
        .onFocus = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onFocus) toggleBehavior.onFocus(this->componentID);
        },
        .onBlur = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onBlur) toggleBehavior.onBlur(this->componentID);
        },
        .onHoverIn = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onHoverIn) toggleBehavior.onHoverIn(this->componentID);
        },
        .onHoverOut = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onHoverOut) toggleBehavior.onHoverOut(this->componentID);
        },
        .onLeftPressIn = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onLeftPressIn) toggleBehavior.onLeftPress(this->componentID);
        },
        .onLeftPress = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onLeftPress) toggleBehavior.onLeftPress(this->componentID);
            toggleState.isOn = !toggleState.isOn;
            if (toggleBehavior.onToggledState) toggleBehavior.onToggledState(this->componentID, toggleState.isOn);
        },
        .onRightPressIn = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onRightPressIn) toggleBehavior.onRightPressIn(this->componentID);
        },
        .onRightPress = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onRightPress) toggleBehavior.onRightPress(this->componentID);
        },
        .onMiddlePressIn = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onMiddlePressIn) toggleBehavior.onMiddlePressIn(this->componentID);
        },
        .onMiddlePress = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onMiddlePress) toggleBehavior.onMiddlePress(this->componentID);
        },
        .onDoublePress = [this](const SFUI::String& componentID) {
            if (toggleBehavior.onDoublePress) toggleBehavior.onDoublePress(this->componentID);
        },
        .onKeyPress = [this](const SFUI::String& componentID, sf::Keyboard::Key pressedKey) {
            if (toggleBehavior.onKeyPress) toggleBehavior.onKeyPress(this->componentID, pressedKey);
        }
    };
    toggle.update(renderTargetSize);
}
