/**
 * @file Container.cpp
 * @brief Implements the SFUI Container component. 
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
 *
 * This file contains the function definitions and internal logic for the
 * SFUI Container component. It handles:
 *   - Construction and destruction
 *   - Event propagation to children
 *   - Layout and rendering
 *
 * It works in conjunction with Container.hpp to provide full functionality
 * of the Container component.
 */


#include "components/Container.hpp"


/**
 * @brief The constructor for Container.
 * 
 * @param componentID The unique identifier for the container component.
 */
SFUI::Container::Container(SFUI::String componentID) :
    Component(std::move(componentID))
{}


/**
 * @brief Handle input events for the container.
 * 
 * @param event The input event to handle.
 */
SFUI::Void SFUI::Container::handleEvent(const SFUI::Event& event) {}


/**
 * @brief Recalculate the properties of the container.
 * 
 * @param renderTargetSize The size of the render target.
 */
SFUI::Void SFUI::Container::update(const SFUI::Vector2u renderTargetSize) {
    if (
        this->renderTargetSize != renderTargetSize ||
        layout != dirtyLayout ||
        style != dirtyStyle ||
        dirtyEvent
    ) {
        this->renderTargetSize = renderTargetSize;
        computeAlignment();
        computeLayoutBox();
        computeStyles();
        computeColors();
        computeShadows();
        computeGraphics();
        computeChildrenLayoutBox();
        updateChildren();
    }
    this->renderTargetSize = renderTargetSize;
    dirtyLayout = layout;
    dirtyStyle = style;
}


/**
 * @brief Draw the container and its contents.
 * 
 * @param drawTarget The render target to draw to.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::Container::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    drawTarget.draw(shadowRects);
    drawTarget.draw(shadowArcs);
    drawTarget.draw(backgroundRects);
    drawTarget.draw(backgroundArcs);
    drawTarget.draw(borderRects);
    drawTarget.draw(borderArcs);
}
