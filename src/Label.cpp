#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


///////////////////////////////////////
// SFML-FlexUI Label Component Class //
///////////////////////////////////////


// Static Variables //
const SFUI::Float SFUI::Label::VERTICAL_CENTER_OFFSET_FACTOR = 0.3f;
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
    computeAlignment();
    computeLayoutBox();
    computeStyles();
    computeColors();
    computeShadows();
    computeGraphics();
    computeChildrenLayoutBox();
    updateChildren();
    computeTextCore();
    computeTextStyles();
    computeTextLayout();
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::Label::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    drawTarget.draw(shadowRects);
    drawTarget.draw(shadowArcs);
    drawTarget.draw(backgroundRects);
    drawTarget.draw(backgroundArcs);
    drawTarget.draw(borderRects);
    drawTarget.draw(borderArcs);

    // Save Clipping State for Containing Text within Label's Bounds and Padding //
    GLint parentClipping[4];
    GLboolean scissorWasEnabled = glIsEnabled(GL_SCISSOR_TEST);
    if (scissorWasEnabled) glGetIntegerv(GL_SCISSOR_BOX, parentClipping);
    SFUI::Vector2i labelPosition = computedLayout.position;
    SFUI::Vector2f labelSize = computedLayout.size;
    SFUI::Vector4f labelPadding = computedLayout.padding;
    GLint newClipping[4] = {
        static_cast<GLint>(labelPosition.x + labelPadding.x),
        static_cast<GLint>(drawTarget.getSize().y - (labelPosition.y + labelPadding.z) - (labelSize.y - labelPadding.z - labelPadding.w)),
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
    drawTarget.draw(textObject);

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
SFUI::FloatRect SFUI::Label::getTextBounds() {
    SFUI::FloatRect textBounds = {{textObject.getPosition().x, textObject.getPosition().y}, {0.0f, 0.0f}};
    if (textObject.getString().getSize() > 0) textBounds = textObject.getGlobalBounds();
    return textBounds;
}


/**
 * @brief .
 * 
 * @param .
 * 
 * @return .
 */
SFUI::Vector2f SFUI::Label::getCharacterPosition(SFUI::Size charIndex) {
    SFUI::Vector2f characterPosition = {0.0f, 0.0f};
    if (textObject.getString().getSize() > 0) {
        sf::Vector2f position = textObject.findCharacterPos(charIndex);
        characterPosition = {position.x, position.y};
    }
    return characterPosition;
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
SFUI::Void SFUI::Label::computeTextStyles() {
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

    // Colors //
    textObject.setFillColor(resolveColorSubProp(labelStyle.textColor));
    textObject.setOutlineColor(resolveColorSubProp(labelStyle.textOutlineColor));
}


/**
 * @brief .
 */
SFUI::Void SFUI::Label::computeTextLayout() {
    // Horizontal Align //
    SFUI::String computedHorizontalAlign = labelStyle.textAlignHorizontal;
    std::transform(computedHorizontalAlign.begin(), computedHorizontalAlign.end(), computedHorizontalAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    if (computedHorizontalAlign != "left" && computedHorizontalAlign != "center" && computedHorizontalAlign == "right")
        computedHorizontalAlign = "center";

    // Vertical Align //
    SFUI::String computedVerticalAlign = labelStyle.textAlignVertical;
    std::transform(computedVerticalAlign.begin(), computedVerticalAlign.end(), computedVerticalAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    if (computedVerticalAlign != "top" && computedVerticalAlign != "center" && computedVerticalAlign == "bottom")
        computedVerticalAlign = "center";

    // Text Position //
    if (!labelStyle.text.empty() && labelStyle.font) {
        SFUI::Vector2f textPosition;
        if (computedHorizontalAlign == "left")
            textPosition.x = computedLayout.position.x + computedLayout.padding.x;
        else if (computedHorizontalAlign == "center")
            textPosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (textObject.getLocalBounds().size.x / 2.0f);
        else if (computedHorizontalAlign == "right")
            textPosition.x = computedLayout.position.x + computedLayout.size.x - computedLayout.padding.y - textObject.getLocalBounds().size.x;
        if (computedVerticalAlign == "top")
            textPosition.y = computedLayout.position.y + computedLayout.padding.z;
        else if (computedVerticalAlign == "center")
            textPosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (textObject.getLocalBounds().size.y / 2.0f) - (textObject.getCharacterSize() * VERTICAL_CENTER_OFFSET_FACTOR);
        else if (computedVerticalAlign == "bottom")
            textPosition.y = computedLayout.position.y + computedLayout.size.y - computedLayout.padding.w - textObject.getLocalBounds().size.y - (textObject.getCharacterSize() * BOTTOM_OFFSET_FACTOR);
    
        textPosition.x += labelStyle.textOffset.x;
        textPosition.y += labelStyle.textOffset.y;
        textObject.setPosition(textPosition);
    }
}
