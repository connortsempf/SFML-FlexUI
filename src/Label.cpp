#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


///////////////////////////////////////
// SFML-FlexUI Label Component Class //
///////////////////////////////////////


// Static Variables //
const SFUI::Float SFUI::Label::VERTICAL_CENTER_OFFSET_FACTOR = 0.65f;
const SFUI::Float SFUI::Label::VERTICAL_BOTTOM_OFFSET_FACTOR = 1.22f;


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
SFUI::Label::Label(const SFUI::String& componentID, const SFUI::PropGroup::Label& labelPropGroup) :
    Component(componentID, labelPropGroup.layout, labelPropGroup.style),
    labelStyle(labelPropGroup.labelStyle),
    textObject(SFUI::Text(*(labelStyle.font), labelStyle.text, labelStyle.textSize))
{}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Label::handleEvent(const SFUI::Event& event) {}


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
SFUI::Void SFUI::Label::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(backgroundRects);
    renderTarget.draw(backgroundArcs);
    renderTarget.draw(borderRects);
    renderTarget.draw(borderArcs);

    // Save Clipping State for Containing Text within Label's Bounds and Padding //
    GLint parentClipping[4];
    GLboolean scissorWasEnabled = glIsEnabled(GL_SCISSOR_TEST);
    if (scissorWasEnabled) glGetIntegerv(GL_SCISSOR_BOX, parentClipping);
    SFUI::Vector2i labelPosition = computedLayout.position;
    SFUI::Vector2f labelSize = computedLayout.size;
    SFUI::Float labelPadding = computedLayout.padding;
    GLint newClipping[4] = {
        static_cast<GLint>(labelPosition.x + labelPadding),
        static_cast<GLint>(renderTarget.getSize().y - (labelPosition.y + labelPadding) - (labelSize.y - labelPadding * 2.0f)),
        static_cast<GLint>(labelSize.x - (labelPadding * 2.0f)),
        static_cast<GLint>(labelSize.y - (labelPadding * 2.0f))
    };
    if (scissorWasEnabled) {
        GLint newRight = newClipping[0] + newClipping[2];
        GLint newBottom = newClipping[1] + newClipping[3];
        GLint parentRight = parentClipping[0] + parentClipping[2];
        GLint parentBottom = parentClipping[1] + parentClipping[3];
        newClipping[0] = std::max(newClipping[0], parentClipping[0]);
        newClipping[1] = std::max(newClipping[1], parentClipping[1]);
        newClipping[2] = std::min(newRight, parentRight) - newClipping[0];
        newClipping[3] = std::min(newBottom, parentBottom) - newClipping[1];
        newClipping[2] = std::max(0, newClipping[2]);
        newClipping[3] = std::max(0, newClipping[3]);
    };
    glEnable(GL_SCISSOR_TEST);
    glScissor(newClipping[0], newClipping[1], newClipping[2], newClipping[3]);        
    
    // Draw Clipped Text //
    renderTarget.draw(textObject);

    // Restore Previous Clipping //
    if (scissorWasEnabled) {
        glScissor(parentClipping[0], parentClipping[1], parentClipping[2], parentClipping[3]);
    }   else {
        glDisable(GL_SCISSOR_TEST);
    }
}


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
 * 
 * @return .
 */
SFUI::FloatRect SFUI::Label::getTextBounds() {
    return textObject.getGlobalBounds();
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
