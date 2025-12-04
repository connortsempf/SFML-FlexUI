/**
 * @file Container.cpp
 * @brief Implements the SFUI Container component.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.0
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


#include "Components/Container.hpp"


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
 * @brief Handle the pre updaate updates for the component.
 */
SFUI::Void SFUI::Container::preUpdate() {
    this->baseLayout = this->layout;
    this->baseStyle = this->style;
    this->baseState = this->state;
}


/**
 * @brief Recalculate the properties of the container.
 *
 * @param renderTargetSize The size of the render target.
 */
SFUI::Void SFUI::Container::update(const SFUI::Vector2u renderTargetSize) {
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
SFUI::Void SFUI::Container::drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {}
