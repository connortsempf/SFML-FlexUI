/**
 * @file Label.cpp
 * @brief Implements the SFUI Label component.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.1
 *
 * This file contains the function definitions and internal logic for the
 * SFUI Label component. It handles:
 *   - Construction and destruction
 *   - Text rendering
 *   - Layout and style updates
 *
 * It works in conjunction with Label.hpp to provide full functionality
 * of the Label component.
 */


#include "Components/Label.hpp"


/**
 * @brief Vertical offset factor for center-aligned text.
 */
const SFUI::Float SFUI::Label::VERTICAL_CENTER_OFFSET_FACTOR = 0.3f;


/**
 * @brief Vertical offset factor for bottom-aligned text.
 */
const SFUI::Float SFUI::Label::BOTTOM_OFFSET_FACTOR = 0.65f;


/**
 * @brief The constructor for Label.
 *
 * @param componentID The unique identifier for the label component.
 */
SFUI::Label::Label(SFUI::String componentID) :
    Component(std::move(componentID)),
    textObject(*(style.font), style.text, style.textSize)
{}


/**
 * @brief Handle input events for the label.
 *
 * @param event The input event to handle.
 */
SFUI::Void SFUI::Label::handleEvent(const SFUI::Event& event) {}


/**
 * @brief Handle the pre updaate updates for the component.
 */
SFUI::Void SFUI::Label::preUpdate() {
    this->baseLayout = this->layout;
    this->baseStyle = this->style;
    this->baseState = this->state;
}


/**
 * @brief Recalculate the properties of the label.
 *
 * @param renderTargetSize The size of the render target.
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
 * @brief Draw the label and its contents.
 *
 * @param drawTarget The render target to draw to.
 * @param window The render window associated with the render target.
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
SFUI::Void SFUI::Label::drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {}


/**
 * @brief Get the font used by the label.
 *
 * @return The font used by the label.
 */
SFUI::SharedPointer<SFUI::Font> SFUI::Label::getFont() {
    return style.font;
}


/**
 * @brief Get the text size of the label.
 *
 * @return The text size of the label.
 */
SFUI::Float SFUI::Label::getTextSize() {
    return textObject.getCharacterSize();
}


/**
 * @brief Get the bounds of the label's text.
 *
 * @return The bounds of the label's text.
 */
SFUI::FloatRect SFUI::Label::getTextBounds() {
    SFUI::FloatRect textBounds = {{textObject.getPosition().x, textObject.getPosition().y}, {0.0f, 0.0f}};
    if (textObject.getString().getSize() > 0) textBounds = textObject.getGlobalBounds();
    return textBounds;
}


/**
 * @brief Get the position of a character in the label's text.
 *
 * @param charIndex The index of the character to get the position of.
 *
 * @return The position of the character in the label's text.
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
 * @brief Compute the core properties of the character text.
 */
SFUI::Void SFUI::Label::computeTextCore() {
    textObject.setString(style.text);
    textObject.setFont(*style.font);
    if (style.textSize > 0.0f) textObject.setCharacterSize(style.textSize);
    else textObject.setCharacterSize(12.0f);
}


/**
 * @brief Compute the style properties of the character text.
 */
SFUI::Void SFUI::Label::computeTextStyles() {
    // Style //
    SFUI::UnsignedInt32 computedTextStyle = SFUI::Text::Style::Regular;
    if (std::holds_alternative<SFUI::UnsignedInt32>(style.textStyle)) {
        computedTextStyle = std::get<SFUI::UnsignedInt32>(style.textStyle);
    }
    else if (std::holds_alternative<SFUI::String>(style.textStyle)) {
        SFUI::String tempTextStyle = std::get<SFUI::String>(style.textStyle);
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
    if (style.letterSpacing.has_value() && style.letterSpacing.value() >= 0.0f)
        textObject.setLetterSpacing(style.letterSpacing.value());

    // Line Spacing //
    if (style.lineSpacing.has_value() && style.lineSpacing.value() >= 0.0f)
        textObject.setLineSpacing(style.lineSpacing.value());

    // Outline Thickness //
    if (style.textOutlineThickness >= 0.0f) textObject.setOutlineThickness(style.textOutlineThickness);

    // Colors //
    textObject.setFillColor(resolveColorSubProp(style.textColor));
    textObject.setOutlineColor(resolveColorSubProp(style.textOutlineColor));
}


/**
 * @brief Compute the layout of the character text.
 */
SFUI::Void SFUI::Label::computeTextLayout() {
    // Horizontal Align //
    SFUI::String computedHorizontalAlign = style.textAlignHorizontal;
    std::transform(computedHorizontalAlign.begin(), computedHorizontalAlign.end(), computedHorizontalAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    if (computedHorizontalAlign != "left" && computedHorizontalAlign != "center" && computedHorizontalAlign != "right")
        computedHorizontalAlign = "center";

    // Vertical Align //
    SFUI::String computedVerticalAlign = style.textAlignVertical;
    std::transform(computedVerticalAlign.begin(), computedVerticalAlign.end(), computedVerticalAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    if (computedVerticalAlign != "top" && computedVerticalAlign != "center" && computedVerticalAlign == "bottom")
        computedVerticalAlign = "center";

    // Text Position //
    if (!style.text.empty() && style.font) {
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

        textPosition.x += style.textOffset.x;
        textPosition.y += style.textOffset.y;
        textObject.setPosition(textPosition);
    }
}
