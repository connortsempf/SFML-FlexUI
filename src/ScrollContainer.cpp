/**
 * @file ScrollContainer.cpp
 * @brief Implements the SFUI ScrollContainer component.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.0
 *
 * This file contains the function definitions and internal logic for the
 * SFUI ScrollContainer component. It handles:
 *   - Construction and destruction
 *   - Scrolling and viewport handling
 *   - Event handling and rendering
 *
 * It works in conjunction with ScrollContainer.hpp to provide full functionality
 * of the ScrollContainer component.
 */


#include "Components/ScrollContainer.hpp"


/**
 * @brief The constructor for ScrollContainer.
 *
 * @param componentID The unique identifier for the scroll container component.
 */
SFUI::ScrollContainer::ScrollContainer(SFUI::String componentID) :
    Component(std::move(componentID))
{}


/**
 * @brief Handle input events for the scroll container.
 *
 * @param event The input event to handle.
 */
SFUI::Void SFUI::ScrollContainer::handleEvent(const SFUI::Event& event) {
    // Mouse Moved Event Handling //
    if (const SFUI::Event::MouseMoved* mouseMovedEvent = event.getIf<SFUI::Event::MouseMoved>()) {
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseMovedEvent->position.x, mouseMovedEvent->position.y);
        SFUI::Bool scrollAreaHovered = isMouseHovered(mousePosition);

        if (scrollAreaHovered) {
            if (!isHovered) isHovered = true;
        }
        else if (!scrollAreaHovered) {
            if (isHovered) isHovered = false;
        }
    }

    // Mouse Wheel Scrolled Event Handling //
    if (const SFUI::Event::MouseWheelScrolled* mouseWheelScrolledEvent = event.getIf<SFUI::Event::MouseWheelScrolled>()) {
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseWheelScrolledEvent->position.x, mouseWheelScrolledEvent->position.y);
        SFUI::Bool scrollAreaHovered = isMouseHovered(mousePosition);
        if (!scrollAreaHovered) return;

        if (mouseWheelScrolledEvent->wheel == sf::Mouse::Wheel::Vertical) {
            if (computedScrollContainerStyle.scrollDirection == "vertical" || computedScrollContainerStyle.scrollDirection == "both") {
                SFUI::Float newScrollOffsetY = scrollOffset.y + (mouseWheelScrolledEvent->delta * computedScrollContainerStyle.scrollSpeedFactor);
                if ((computedLayout.alignDirection == SFUI::Component::AlignDirection::Vertical && computedLayout.alignPrimary == SFUI::Component::AlignPrimary::Start) ||
                    (computedLayout.alignDirection == SFUI::Component::AlignDirection::Horizontal && computedLayout.alignSecondary == SFUI::Component::AlignSecondary::Start)) {
                        if (newScrollOffsetY > -maxScrollOffset.y && newScrollOffsetY < 0.0f && behavior.onScroll) behavior.onScroll(componentID);
                        scrollOffset.y = std::clamp(newScrollOffsetY, -maxScrollOffset.y, 0.0f);
                }   else if ((computedLayout.alignDirection == SFUI::Component::AlignDirection::Vertical && computedLayout.alignPrimary == SFUI::Component::AlignPrimary::End) ||
                    (computedLayout.alignDirection == SFUI::Component::AlignDirection::Horizontal && computedLayout.alignSecondary == SFUI::Component::AlignSecondary::End)) {
                        if (newScrollOffsetY > 0.0f && newScrollOffsetY < maxScrollOffset.y && behavior.onScroll) behavior.onScroll(componentID);
                        scrollOffset.y = std::clamp(newScrollOffsetY, 0.0f, maxScrollOffset.y);
                }
            }
        }
        else if (mouseWheelScrolledEvent->wheel == sf::Mouse::Wheel::Vertical) {
            if (computedScrollContainerStyle.scrollDirection == "horizontal" || computedScrollContainerStyle.scrollDirection == "both") {
                SFUI::Float newScrollOffsetX = scrollOffset.x + (mouseWheelScrolledEvent->delta * computedScrollContainerStyle.scrollSpeedFactor);
                if ((computedLayout.alignDirection == SFUI::Component::AlignDirection::Horizontal && computedLayout.alignPrimary == SFUI::Component::AlignPrimary::Start) ||
                    (computedLayout.alignDirection == SFUI::Component::AlignDirection::Vertical && computedLayout.alignSecondary == SFUI::Component::AlignSecondary::Start)) {
                        if (newScrollOffsetX > -maxScrollOffset.y && newScrollOffsetX < 0.0f && behavior.onScroll) behavior.onScroll(componentID);
                        scrollOffset.x = std::clamp(newScrollOffsetX, -maxScrollOffset.x, 0.0f);
                }
                else if ((computedLayout.alignDirection == SFUI::Component::AlignDirection::Horizontal && computedLayout.alignPrimary == SFUI::Component::AlignPrimary::End) ||
                    (computedLayout.alignDirection == SFUI::Component::AlignDirection::Vertical && computedLayout.alignSecondary == SFUI::Component::AlignSecondary::End)) {
                        if (newScrollOffsetX > 0.0f && newScrollOffsetX < maxScrollOffset.x && behavior.onScroll) behavior.onScroll(componentID);
                        scrollOffset.x = std::clamp(newScrollOffsetX, 0.0f, maxScrollOffset.x);
                }
            }
        }
    }
}


/**
 * @brief Handle the pre updaate updates for the component.
 */
SFUI::Void SFUI::ScrollContainer::preUpdate() {
    this->baseLayout = this->layout;
    this->baseStyle = this->style;
    this->baseState = this->state;
}


/**
 * @brief Recalculate the properties of the scroll container.
 *
 * @param renderTargetSize The size of the render target.
 */
SFUI::Void SFUI::ScrollContainer::update(const SFUI::Vector2u renderTargetSize) {
    this->renderTargetSize = renderTargetSize;
    computeAlignment();
    computeLayoutBox();
    computeStyles();
    computeColors();
    computeShadows();
    computeGraphics();
    computeChildrenLayoutBox();
    computeAlignPrimary();
    computeScrollDynamics();
    computeChildrenScrollPosition();
    updateChildren();
}


/**
 * @brief Draw the scroll container and its contents.
 *
 * @param drawTarget The render target to draw to.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::ScrollContainer::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
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
SFUI::Void SFUI::ScrollContainer::drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {}


/**
 * @brief Compute primary alignment for the scroll container.
 */
SFUI::Void SFUI::ScrollContainer::computeAlignPrimary() {
    SFUI::String tempAlignPrimary = layout.alignPrimary;
    std::transform(tempAlignPrimary.begin(), tempAlignPrimary.end(), tempAlignPrimary.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlignPrimary == "start") computedLayout.alignPrimary = SFUI::Component::AlignPrimary::Start;
    else if (tempAlignPrimary == "end") computedLayout.alignPrimary = SFUI::Component::AlignPrimary::End;
    else computedLayout.alignPrimary = SFUI::Component::AlignPrimary::Start;
}


/**
 * @brief Compute scroll dynamics for the scroll container.
 */
SFUI::Void SFUI::ScrollContainer::computeScrollDynamics() {
    // Scroll Direction //
    SFUI::String tempAlign = style.scrollDirection;
    std::transform(tempAlign.begin(), tempAlign.end(), tempAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    if (tempAlign == "vertical" || tempAlign == "horizontal" || tempAlign == "both")
        computedScrollContainerStyle.scrollDirection = tempAlign;
    else
        computedScrollContainerStyle.scrollDirection = "vertical";

    // Scroll Speed Factor //
    if (style.scrollSpeedFactor == 0) computedScrollContainerStyle.scrollSpeedFactor = 15.0f;
    else computedScrollContainerStyle.scrollSpeedFactor = style.scrollSpeedFactor;

    // Maximum Scroll Offset //
    const SFUI::Vector<SFUI::UniquePointer<SFUI::Component>>& children = this->getChildren();
    SFUI::Vector2f contentSize = {0.0f, 0.0f};
    for (const auto& child : children) {
        SFUI::Vector4f childMargin = child->getMargin();
        contentSize.x += (child->getSize().x + (childMargin.x + childMargin.y));
        contentSize.y += (child->getSize().y + (childMargin.z + childMargin.w));
    }
    maxScrollOffset = {contentSize.x - computedLayout.size.x, contentSize.y - computedLayout.size.y};
    if (maxScrollOffset.x < 0.0f) maxScrollOffset.x = 0.0f;
    if (maxScrollOffset.y < 0.0f) maxScrollOffset.y = 0.0f;
}


/**
 * @brief Compute the scroll position of all child components.
 */
SFUI::Void SFUI::ScrollContainer::computeChildrenScrollPosition() {
    const SFUI::Vector<SFUI::UniquePointer<SFUI::Component>>& children = this->getChildren();
    for (int i = 0; i < children.size(); i++) {
        computedChildrenLayout[i].position = {
            computedChildrenLayout[i].position.x + static_cast<SFUI::Int>(scrollOffset.x),
            computedChildrenLayout[i].position.y + static_cast<SFUI::Int>(scrollOffset.y)
        };
    }
}
