#include <sfml-flexui.hpp>


///////////////////////////////////////
// SFML-FlexUI Label Component Class //
///////////////////////////////////////


// Static Variables //
SFUI::Float SFUI::Label::VERTICAL_CENTER_OFFSET_FACTOR = 0.65f;
SFUI::Float SFUI::Label::VERTICAL_BOTTOM_OFFSET_FACTOR = 1.22f;


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Label::Label(const SFUI::String& componentID) :
    Component(componentID),
    labelStyle(),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Label::Label(const SFUI::String& componentID, const SFUI::Prop::Layout& layout) :
    Component(componentID, layout),
    labelStyle(),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Label::Label(const SFUI::String& componentID, const SFUI::Prop::Style& style) :
    Component(componentID, style),
    labelStyle(),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::Label::Label(const SFUI::String& componentID, const SFUI::Prop::LabelStyle& labelStyle) :
    Component(componentID),
    labelStyle(labelStyle),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 * @param .
 */
SFUI::Label::Label(const SFUI::String& componentID, const SFUI::Prop::Layout& layout, const SFUI::Prop::Style& style, const SFUI::Prop::LabelStyle& labelStyle) :
    Component(componentID, layout, style),
    labelStyle(labelStyle),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::Label::getTextSize() {
    return computedLabelStyle.textSize;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::String SFUI::Label::getTextAlignHorizontal() {
    return computedLabelStyle.textAlignHorizontal;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::String SFUI::Label::getTextAlignVertical() {
    return computedLabelStyle.textAlignVertical;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Label::getTextColor() {
    return computedLabelStyle.textColor;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextSize() {
    if (labelStyle.textSize > 0.0f)
        computedLabelStyle.textSize = labelStyle.textSize;
    else
        computedLabelStyle.textSize = 12.0f;
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextAlignHorizontal() {
    SFUI::String tempAlignHorizontal = labelStyle.textAlignHorizontal;
    std::transform(tempAlignHorizontal.begin(), tempAlignHorizontal.end(), tempAlignHorizontal.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlignHorizontal == "left" || tempAlignHorizontal == "center" || tempAlignHorizontal == "right")
        computedLabelStyle.textAlignHorizontal = tempAlignHorizontal;
    else
        computedLabelStyle.textAlignHorizontal = "center";
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextAlignVertical() {
    SFUI::String tempAlignVertical = labelStyle.textAlignVertical;
    std::transform(tempAlignVertical.begin(), tempAlignVertical.end(), tempAlignVertical.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlignVertical == "top" || tempAlignVertical == "center" || tempAlignVertical == "bottom")
        computedLabelStyle.textAlignVertical = tempAlignVertical;
    else
        computedLabelStyle.textAlignVertical = "center";
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextColor() {
    computedLabelStyle.textColor = resolveColorSubProp(labelStyle.textColor);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeText() {
    if (!labelStyle.font) return;

    // Text Object //
    textObject.setFont(*labelStyle.font);
    textObject.setString(labelStyle.text);
    textObject.setCharacterSize(computedLabelStyle.textSize);
    textObject.setFillColor(computedLabelStyle.textColor);
    
    SFUI::Vector2f textPosition;
    // Horizontal Text Position //
    if (computedLabelStyle.textAlignHorizontal == "left")
        textPosition.x = computedLayout.position.x + computedLayout.padding;
    else if (computedLabelStyle.textAlignHorizontal == "center")
        textPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (textObject.getLocalBounds().size.x / 2.0f);
    else if (computedLabelStyle.textAlignHorizontal == "right")
        textPosition.x = computedLayout.position.x + computedLayout.size.x - textObject.getLocalBounds().size.x - computedLayout.padding;
    // Vertical Text Position //
    if (computedLabelStyle.textAlignVertical == "top")
        textPosition.y = computedLayout.position.y + computedLayout.padding;
    else if (computedLabelStyle.textAlignVertical == "center")
        textPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedLabelStyle.textSize * VERTICAL_CENTER_OFFSET_FACTOR);
    else if (computedLabelStyle.textAlignVertical == "bottom")
        textPosition.y = computedLayout.position.y + computedLayout.size.y - (computedLabelStyle.textSize * VERTICAL_BOTTOM_OFFSET_FACTOR) - computedLayout.padding;

    textObject.setPosition(textPosition);
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Label::update(const SFUI::Vector2u renderTargetSize) {
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

    // Label Specific Computation //
    computeTextSize();
    computeTextAlignHorizontal();
    computeTextAlignVertical();
    computeTextColor();
    computeText();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Label::handleEvent(const SFUI::Event& event) {

}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Label::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(backgroundRects);
    renderTarget.draw(backgroundArcs);
    renderTarget.draw(borderRects);
    renderTarget.draw(borderArcs);
    renderTarget.draw(textObject);
}
