#include <sfml-flexui.hpp>


////////////////////////////////////////
// SFML-FlexUI Button Component Class //
////////////////////////////////////////


// Static Variables //
SFUI::Float SFUI::Button::TEXT_VERTICAL_OFFSET_FACTOR = 0.65f;
SFUI::UnsignedInt SFUI::Button::LONG_PRESS_THRESHOLD_MS = 500;


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID) :
    Component(componentID),
    buttonStyle(),
    toolTipTextObject(SFUI::Text(*(buttonStyle.toolTipFont), buttonStyle.toolTipText, buttonStyle.toolTipTextSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID, const SFUI::Prop::Layout& layout) :
    Component(componentID, layout),
    buttonStyle(),
    toolTipTextObject(SFUI::Text(*(buttonStyle.toolTipFont), buttonStyle.toolTipText, buttonStyle.toolTipTextSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID, const SFUI::Prop::Style& style) :
    Component(componentID, style),
    buttonStyle(),
    toolTipTextObject(SFUI::Text(*(buttonStyle.toolTipFont), buttonStyle.toolTipText, buttonStyle.toolTipTextSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID, const SFUI::Prop::ButtonStyle& buttonStyle) :
    Component(componentID),
    buttonStyle(buttonStyle),
    toolTipTextObject(SFUI::Text(*(buttonStyle.toolTipFont), buttonStyle.toolTipText, buttonStyle.toolTipTextSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
 */
SFUI::Button::Button(const SFUI::String& componentID, const SFUI::Prop::Layout& layout, const SFUI::Prop::Style& style, const SFUI::Prop::ButtonStyle& buttonStyle) :
    Component(componentID, layout, style),
    buttonStyle(buttonStyle),
    toolTipTextObject(SFUI::Text(*(buttonStyle.toolTipFont), buttonStyle.toolTipText, buttonStyle.toolTipTextSize))
{}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeAlternateColors() {
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
    computedButtonStyle.focusCornerRadius = resolveCornerRadiusSubPro(
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
SFUI::Void SFUI::Button::computeToolTipPadding() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;
    
    computedButtonStyle.toolTipPadding = buttonStyle.toolTipPadding;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Button::computeToolTipCornerRadius() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;

    computedButtonStyle.toolTipCornerRadius = resolveCornerRadiusSubPro(
        SFUI::Vector2f(
            toolTipTextObject.getLocalBounds().size.x,
            toolTipTextObject.getLocalBounds().size.y
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
SFUI::Void SFUI::Button::computeToolTipText() {
    if (buttonStyle.toolTipText == "" || !buttonStyle.toolTipFont) return;

    toolTipTextObject.setFont(*buttonStyle.toolTipFont);
    toolTipTextObject.setString(buttonStyle.toolTipText);
    toolTipTextObject.setCharacterSize(computedButtonStyle.toolTipTextSize);
    toolTipTextObject.setFillColor(computedButtonStyle.toolTipTextColor);
    
    // Set Tool Tip Position Based on Mouse Position During Input Event Handling //
    // SFUI::Vector2f textPosition;
    // textPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (toolTipTextObject.getLocalBounds().size.x / 2.0f);
    // textPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedButtonStyle.toolTipTextSize * TEXT_VERTICAL_OFFSET_FACTOR);
    // toolTipTextObject.setPosition(textPosition);
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Button::update(const SFUI::Vector2u parentComponentSize) {
    this->parentComponentSize = parentComponentSize;

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

    // Button Specific Computation //
    computeAlternateColors();
    computeFocusWidth();
    computeFocusOffset();
    computeFocusCornerRadius();
    computeFocusFillColor();
    computeToolTipPadding();
    computeToolTipCornerRadius();
    computeToolTipTextSize();
    computeToolTipFillColor();
    computeToolTipTextColor();
    computeToolTipText();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Button::handleEvent(const SFUI::Event& event) {

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
}