#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


///////////////////////////////////////
// SFML-FlexUI Label Component Class //
///////////////////////////////////////


// Static Variables //
const SFUI::Float SFUI::Label::BOTTOM_OFFSET_FACTOR = 0.65f;


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
    computeTextCore();
    computeTextStyling();
    computeTextAlign();
    computeTextColors();
    computeTextLayout();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Label::draw(SFUI::RenderTarget& renderTarget) {
    renderTarget.draw(shadowRects);
    renderTarget.draw(shadowArcs);
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
    SFUI::Vector4f labelPadding = computedLayout.padding;
    GLint newClipping[4] = {
        static_cast<GLint>(labelPosition.x + labelPadding.x),
        static_cast<GLint>(renderTarget.getSize().y - (labelPosition.y + labelPadding.z) - (labelSize.y - labelPadding.z - labelPadding.w)),
        static_cast<GLint>(labelSize.x - (labelPadding.x + labelPadding.y)),
        static_cast<GLint>(labelSize.y - (labelPadding.z + labelPadding.w))
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
SFUI::String SFUI::Label::getText() {
    return textObject.getString();
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::SharedPointer<SFUI::Font> SFUI::Label::getFont() {
    return labelStyle.font;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::Label::getTextSize() {
    return textObject.getCharacterSize();
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::UnsignedInt32 SFUI::Label::getTextStyle() {
    return textObject.getStyle();
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::Label::getLetterSpacing() {
    return textObject.getLetterSpacing();
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::Label::getLineSpacing() {
    return textObject.getLineSpacing();
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::Label::getOutlineThickness() {
    return textObject.getOutlineThickness();
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
    return textObject.getFillColor();
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::Label::getTextOutlineColor() {
    return textObject.getOutlineColor();
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::FloatRect SFUI::Label::getTextBounds() {
    if (textObject.getString().getSize() > 0) return textObject.getGlobalBounds();
    return SFUI::FloatRect({textObject.getPosition().x, textObject.getPosition().y}, {0.0f, 0.0f});
}


/**
 * @brief .
 * 
 * @param .
 * 
 * @return .
 */
SFUI::Vector2f SFUI::Label::getCharacterPosition(SFUI::Size charIndex) {
    sf::Vector2f position = textObject.findCharacterPos(charIndex);
    return {position.x, position.y};
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextCore() {
    textObject.setString(labelStyle.text);
    textObject.setFont(*labelStyle.font);
    if (labelStyle.textSize > 0.0f) textObject.setCharacterSize(labelStyle.textSize);
    else textObject.setCharacterSize(12.0f);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextStyling() {
    // Style //
    SFUI::UnsignedInt32 computedTextStyle = SFUI::Text::Style::Regular;
    if (std::holds_alternative<SFUI::UnsignedInt32>(labelStyle.textStyle)) {
        computedTextStyle = std::get<SFUI::UnsignedInt32>(labelStyle.textStyle);
    }
    else if (std::holds_alternative<SFUI::String>(labelStyle.textStyle)) {
        SFUI::String tempTextStyle = std::get<SFUI::String>(labelStyle.textStyle);
        std::transform(tempTextStyle.begin(), tempTextStyle.end(), tempTextStyle.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
    
        if (tempTextStyle.find("regular") != SFUI::String::npos) computedTextStyle |= SFUI::Text::Style::Regular;
        if (tempTextStyle.find("bold") != SFUI::String::npos) computedTextStyle |= SFUI::Text::Style::Bold;
        if (tempTextStyle.find("italic") != SFUI::String::npos) computedTextStyle |= SFUI::Text::Style::Italic;
        if (tempTextStyle.find("underlined") != SFUI::String::npos) computedTextStyle |= SFUI::Text::Style::Underlined;
        if (tempTextStyle.find("strikethrough") != SFUI::String::npos) computedTextStyle |= SFUI::Text::Style::StrikeThrough;
    }
    textObject.setStyle(computedTextStyle);

    // Letter Spacing //
    if (labelStyle.letterSpacing.has_value() && labelStyle.letterSpacing.value() >= 0.0f)
        textObject.setLetterSpacing(labelStyle.letterSpacing.value());

    // Line Spacing //
    if (labelStyle.lineSpacing.has_value() && labelStyle.lineSpacing.value() >= 0.0f)
        textObject.setLineSpacing(labelStyle.lineSpacing.value());
    
    // Outline Thickness //
    if (labelStyle.textOutlineThickness >= 0.0f) textObject.setOutlineThickness(labelStyle.textOutlineThickness);
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextAlign() {
    // Horizontal Align //
    SFUI::String tempAlignHorizontal = labelStyle.textAlignHorizontal;
    std::transform(tempAlignHorizontal.begin(), tempAlignHorizontal.end(), tempAlignHorizontal.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlignHorizontal == "left" || tempAlignHorizontal == "center" || tempAlignHorizontal == "right")
        computedLabelStyle.textAlignHorizontal = tempAlignHorizontal;
    else
        computedLabelStyle.textAlignHorizontal = "center";
    
    // Vertical Align //
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
SFUI::Void SFUI::Label::computeTextColors() {
    textObject.setFillColor(resolveColorSubProp(labelStyle.textColor));
    textObject.setOutlineColor(resolveColorSubProp(labelStyle.textOutlineColor));
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextLayout() {
    if (labelStyle.text.empty() || !labelStyle.font) return;

    SFUI::Vector2f textPosition;
    
    // Horizontal Text Position //
    if (computedLabelStyle.textAlignHorizontal == "left")
        textPosition.x = computedLayout.position.x + computedLayout.padding.x;
    else if (computedLabelStyle.textAlignHorizontal == "center")
        textPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (textObject.getLocalBounds().size.x / 2.0f);
    else if (computedLabelStyle.textAlignHorizontal == "right")
        textPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding.y - textObject.getLocalBounds().size.x;

    // Vertical Text Position //
    if (computedLabelStyle.textAlignVertical == "top")
        textPosition.y = computedLayout.position.y + computedLayout.padding.z;
    else if (computedLabelStyle.textAlignVertical == "center")
        textPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (textObject.getLocalBounds().size.y / 2.0f);
    else if (computedLabelStyle.textAlignVertical == "bottom")
        textPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding.w - textObject.getLocalBounds().size.y - (textObject.getCharacterSize() * BOTTOM_OFFSET_FACTOR);

    textObject.setPosition(textPosition);
}
