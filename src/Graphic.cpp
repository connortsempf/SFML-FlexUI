/**
 * @file Graphic.cpp
 * @brief Implements the SFUI Graphic component.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.1
 *
 * This file contains the function definitions and internal logic for the
 * SFUI Graphic component. It handles:
 *   - Construction and destruction
 *   - Rendering of custom graphics
 *   - Style updates and layout computation
 *
 * It works in conjunction with Graphic.hpp to provide full functionality
 * of the Graphic component.
 */


#include "Components/Graphic.hpp"


/**
 * @brief The constructor of the Graphic component.
 *
 * @param componentID The unique identifier for the component.
 */
SFUI::Graphic::Graphic(SFUI::String componentID) :
    Component(std::move(componentID)),
    graphic(graphicSource)
{}


/**
 * @brief Handle the given event.
 *
 * @param event The event to handle.
 */
SFUI::Void SFUI::Graphic::handleEvent(const SFUI::Event& event) {}


/**
 * @brief Handle the pre updaate updates for the component.
 */
SFUI::Void SFUI::Graphic::preUpdate() {
    this->baseLayout = this->layout;
    this->baseStyle = this->style;
    this->baseState = this->state;
}


/**
 * @brief Recalculate the component's properties.
 *
 * @param renderTargetSize The size of the render target.
 */
SFUI::Void SFUI::Graphic::update(const SFUI::Vector2u renderTargetSize) {
    this->renderTargetSize = renderTargetSize;
    computeAlignment();
    computeLayoutBox();
    computeStyles();
    computeColors();
    computeShadows();
    computeGraphics();
    computeChildrenLayoutBox();
    updateChildren();
    computeGraphicSource();
    computeGraphicLayout();
}


/**
 * @brief Draw the component to the given render target.
 *
 * @param drawTarget The render target to draw to.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::Graphic::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    drawTarget.draw(shadowRects);
    drawTarget.draw(shadowArcs);
    drawTarget.draw(backgroundRects);
    drawTarget.draw(backgroundArcs);
    drawTarget.draw(borderRects);
    drawTarget.draw(borderArcs);

    // Save Clipping State for Containing The Sprite within Graphic's Bounds and Padding //
    GLint parentClipping[4];
    GLboolean scissorWasEnabled = glIsEnabled(GL_SCISSOR_TEST);
    if (scissorWasEnabled) glGetIntegerv(GL_SCISSOR_BOX, parentClipping);
    SFUI::Vector2i graphicPosition = computedLayout.position;
    SFUI::Vector2f graphicSize = computedLayout.size;
    SFUI::Vector4f graphicPadding = computedLayout.padding;
    GLint newClipping[4] = {
        static_cast<GLint>(graphicPosition.x + graphicPadding.x),
        static_cast<GLint>(drawTarget.getSize().y - (graphicPosition.y + graphicPadding.z) - (graphicSize.y - graphicPadding.z - graphicPadding.w)),
        static_cast<GLint>(graphicSize.x - (graphicPadding.x + graphicPadding.y)),
        static_cast<GLint>(graphicSize.y - (graphicPadding.z + graphicPadding.w))
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

    // Draw Clipped Sprite //
    drawTarget.draw(graphic);

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
SFUI::Void SFUI::Graphic::drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {}


/**
 * @brief Get the original size at the time of loading for the texture used in the graphic.
 *
 * @return The original texture size as a Vector2f.
 */
SFUI::Vector2f SFUI::Graphic::getOriginalTextureSize() {
    return computedGraphicStyle.originalTextureSize;
}


/**
 * @brief Compute the graphic source based on the loading type and state.
 */
SFUI::Void SFUI::Graphic::computeGraphicSource() {
    // Check if Pre-Loaded Texture was Provided //
    if (style.loadedGraphic.has_value()) {
        loadType = SFUI::Graphic::LoadType::PRE_LOAD;
        if (loadState == SFUI::Graphic::LoadState::PRE_LOAD_UNLOADED) {
            graphic = SFUI::Sprite(*style.loadedGraphic.value());
            computedGraphicStyle.originalTextureSize = {graphic.getGlobalBounds().size.x, graphic.getGlobalBounds().size.y};
            loadState = SFUI::Graphic::LoadState::PRE_LOAD_LOADED;
        }
        return;
    }

    // Check if Non-Loaded Texture Path was Given //
    if (style.graphicPath.has_value() && style.graphicPath.value() != "") {
        SFUI::Graphic::LoadType::SELF_LOAD;
        if (style.graphicPath.value() != computedGraphicStyle.graphicPath) {
            loadState = SFUI::Graphic::LoadState::SELF_LOAD_UNLOADED;
            computedGraphicStyle.graphicPath = style.graphicPath.value();
            if (!graphicSource.loadFromFile(computedGraphicStyle.graphicPath)) {
                if (behavior.onLoadError) behavior.onLoadError(componentID);
            }   else {
                if (behavior.onLoad) behavior.onLoad(componentID);
                graphic = SFUI::Sprite(graphicSource);
                computedGraphicStyle.originalTextureSize = {graphic.getGlobalBounds().size.x, graphic.getGlobalBounds().size.y};
            }
        }
    }
}


/**
 * @brief Compute the layout of the graphic within its allocated space.
 */
SFUI::Void SFUI::Graphic::computeGraphicLayout() {

    // Alignment //
    SFUI::String computedAlign = style.graphicAlign;
    std::transform(computedAlign.begin(), computedAlign.end(), computedAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    if (computedAlign != "fit" && computedAlign != "fill" && computedAlign != "stretch" && computedAlign != "center")
        computedAlign = "fit";

    // Scale //
    SFUI::Vector2f computedScale = {0.0f, 0.0f};
    SFUI::Float scaleX = computedLayout.size.x / computedGraphicStyle.originalTextureSize.x;
    SFUI::Float scaleY = computedLayout.size.y / computedGraphicStyle.originalTextureSize.y;
    if (computedAlign == "fit") {
        SFUI::Float fitScale = std::min(scaleX, scaleY);
        computedScale = {fitScale, fitScale};
    }
    else if (computedAlign == "fill") {
        SFUI::Float fillScale = std::max(scaleX, scaleY);
        computedScale = {fillScale, fillScale};
    }
    else if (computedAlign == "stretch")
        computedScale = {scaleX, scaleY};
    else if (computedAlign == "center")
        computedScale = {1.0f, 1.0f};

    // Position //
    graphic.setScale(computedScale);
    graphic.setPosition({
        computedLayout.position.x + (computedLayout.size.x / 2.0f) - (graphic.getGlobalBounds().size.x / 2.0f),
        computedLayout.position.y + (computedLayout.size.y / 2.0f) - (graphic.getGlobalBounds().size.y / 2.0f)
    });
}
