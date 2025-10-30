#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


//////////////////////////////////////////////////
// SFML-FlexUI Scroll Container Component Class //
//////////////////////////////////////////////////


/**
 * @brief .
 * 
 * @param .
 */
SFUI::ScrollContainer::ScrollContainer(const SFUI::String& componentID) :
    Component(componentID)
{}


/**
 * @brief .
 * 
 * @param .
 * @param .
 */
SFUI::ScrollContainer::ScrollContainer(const SFUI::String& componentID, const SFUI::PropGroup::ScrollContainer& scrollContainerPropGroup) :
    Component(componentID, scrollContainerPropGroup.layout, scrollContainerPropGroup.style),
    scrollContainerStyle(scrollContainerPropGroup.scrollContainerStyle),
    scrollContainerBehavior(scrollContainerPropGroup.scrollContainerBehavior)
{}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::ScrollContainer::handleEvent(const SFUI::Event& event) {

    // Mouse Moved Event Handling //
    if (const SFUI::Event::MouseMoved* mouseMovedEvent = event.getIf<SFUI::Event::MouseMoved>()) {
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseMovedEvent->position.x, mouseMovedEvent->position.y);
        SFUI::Bool buttonHovered = isMouseHovered(mousePosition);
        
        if (buttonHovered) {
            if (!isHovered) {
                isHovered = true;
                // if (buttonBehavior.onHoverIn) buttonBehavior.onHoverIn(componentID);
            }
        }
        else if (!buttonHovered) {
            if (isHovered) {
                isHovered = false;
                // if (buttonBehavior.onHoverOut) buttonBehavior.onHoverOut(componentID);
            }
        }
    }

    // Mouse Button Pressed Event Handling //
    // if (const SFUI::Event::MouseButtonPressed* mousePressedEvent = event.getIf<SFUI::Event::MouseButtonPressed>()) {
    //     sf::Mouse::Button mouseButton = mousePressedEvent->button;
    //     const SFUI::Vector2i mousePosition = SFUI::Vector2i(mousePressedEvent->position.x, mousePressedEvent->position.y);
    //     SFUI::Bool buttonHovered = isMouseHovered(mousePosition);

    //     if (mouseButton == sf::Mouse::Button::Left) {
    //         if (isFocused) {
    //             isFocused = false;
    //             if (buttonBehavior.onBlur) buttonBehavior.onBlur(componentID);
    //         }
    //     }
    //     if (buttonHovered) {
    //         if (mouseButton == sf::Mouse::Button::Left) {
    //             if (!isLeftPressed) {
    //                 isLeftPressed = true;
    //                 if (buttonBehavior.onLeftPressIn) buttonBehavior.onLeftPressIn(componentID);
    //             }
    //         }   else if (mouseButton == sf::Mouse::Button::Right) {
    //             if (!isRightPressed) {
    //                 isRightPressed = true;
    //                 if (buttonBehavior.onRightPressIn) buttonBehavior.onRightPressIn(componentID);
    //             }
    //         }   else if (mouseButton == sf::Mouse::Button::Middle) {
    //             if (!isMiddlePressed) {
    //                 isMiddlePressed = true;
    //                 if (buttonBehavior.onMiddlePressIn) buttonBehavior.onMiddlePressIn(componentID);
    //             }
    //         }
    //     }
    // }

    // Mouse Button Released Event Handling //
    // if (const SFUI::Event::MouseButtonReleased* mouseReleasedEvent = event.getIf<SFUI::Event::MouseButtonReleased>()) {
    //     sf::Mouse::Button mouseButton = mouseReleasedEvent->button;
    //     const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseReleasedEvent->position.x, mouseReleasedEvent->position.y);
    //     SFUI::Bool buttonHovered = isMouseHovered(mousePosition);

    //     if (buttonHovered) {
    //         if (mouseButton == sf::Mouse::Button::Left && isLeftPressed && buttonBehavior.onLeftPress) buttonBehavior.onLeftPress(componentID);
    //         if (mouseButton == sf::Mouse::Button::Right && isRightPressed && buttonBehavior.onRightPress) buttonBehavior.onRightPress(componentID);
    //         if (mouseButton == sf::Mouse::Button::Middle && isMiddlePressed && buttonBehavior.onMiddlePress) buttonBehavior.onMiddlePress(componentID);
    //     }
    //     if (isLeftPressed) isLeftPressed = false;
    //     if (isRightPressed) isRightPressed = false;
    //     if (isMiddlePressed) isMiddlePressed = false;
    // }

    // Mouse Wheel Scrolled Event Handling //
    if (const SFUI::Event::MouseWheelScrolled* mouseWheelScrolledEvent = event.getIf<SFUI::Event::MouseWheelScrolled>()) {
        if (mouseWheelScrolledEvent->wheel == sf::Mouse::Wheel::Vertical) {
            if (computedScrollContainerStyle.scrollBarAlign == "vertical" || computedScrollContainerStyle.scrollBarAlign == "both") {
                SFUI::Float newScrollOffsetY = scrollOffset.y + (mouseWheelScrolledEvent->delta * computedScrollContainerStyle.scrollSpeedFactor);
                if ((computedLayout.alignDirection == "vertical" && computedLayout.alignPrimary == "start") ||
                    (computedLayout.alignDirection == "horizontal" && computedLayout.alignSecondary == "start")) {
                        if (newScrollOffsetY > -maxScrollOffset.y && newScrollOffsetY < 0.0f && scrollContainerBehavior.onScroll) scrollContainerBehavior.onScroll(componentID); 
                        scrollOffset.y = std::clamp(newScrollOffsetY, -maxScrollOffset.y, 0.0f);
                }   else if ((computedLayout.alignDirection == "vertical" && computedLayout.alignPrimary == "end") ||
                    (computedLayout.alignDirection == "horizontal" && computedLayout.alignSecondary == "end")) {
                        if (newScrollOffsetY > 0.0f && newScrollOffsetY < maxScrollOffset.y && scrollContainerBehavior.onScroll) scrollContainerBehavior.onScroll(componentID);
                        scrollOffset.y = std::clamp(newScrollOffsetY, 0.0f, maxScrollOffset.y);
                }
            }
        }   else if (mouseWheelScrolledEvent->wheel == sf::Mouse::Wheel::Vertical) {
            if (computedScrollContainerStyle.scrollBarAlign == "horizontal" || computedScrollContainerStyle.scrollBarAlign == "both") {
                SFUI::Float newScrollOffsetX = scrollOffset.x + (mouseWheelScrolledEvent->delta * computedScrollContainerStyle.scrollSpeedFactor);
                if ((computedLayout.alignDirection == "horizontal" && computedLayout.alignPrimary == "start") ||
                    (computedLayout.alignDirection == "vertical" && computedLayout.alignSecondary == "start")) {
                        if (newScrollOffsetX > -maxScrollOffset.y && newScrollOffsetX < 0.0f && scrollContainerBehavior.onScroll) scrollContainerBehavior.onScroll(componentID);
                        scrollOffset.x = std::clamp(newScrollOffsetX, -maxScrollOffset.x, 0.0f);
                }
                else if ((computedLayout.alignDirection == "horizontal" && computedLayout.alignPrimary == "end") ||
                    (computedLayout.alignDirection == "vertical" && computedLayout.alignSecondary == "end")) {
                        if (newScrollOffsetX > 0.0f && newScrollOffsetX < maxScrollOffset.x && scrollContainerBehavior.onScroll) scrollContainerBehavior.onScroll(componentID);
                        scrollOffset.x = std::clamp(newScrollOffsetX, 0.0f, maxScrollOffset.x);
                }
            }
        }
    }
}


/**
 * @brief .
 * 
 * @param .
 */
SFUI::Void SFUI::ScrollContainer::update(const SFUI::Vector2u renderTargetSize) {
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
    
    // Scroll Container Specific Computation //
    computeScrollDirection();
    computeScrollSpeedFactor();
    computeChildrenScrollPosition();
    computeMaxScrollOffset();

    updateChildren();
}


/**
 * @brief .
 * 
 * @param .
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
 * @brief .
 * 
 * @return .
 */
SFUI::String SFUI::ScrollContainer::getScrollDirection() {
    return computedScrollContainerStyle.scrollDirection;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::ScrollContainer::getScrollSpeedFactor() {
    return computedScrollContainerStyle.scrollSpeedFactor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Bool SFUI::ScrollContainer::getUsingScrollBar() {
    return computedScrollContainerStyle.usingScrollBar;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::String SFUI::ScrollContainer::getScrollBarAlign() {
    return computedScrollContainerStyle.scrollBarAlign;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::ScrollContainer::getTrackOnAxisSize() {
    return computedScrollContainerStyle.trackOnAxisSize;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Float SFUI::ScrollContainer::getTrackOffAxisSize() {
    return computedScrollContainerStyle.trackOffAxisSize;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector4f SFUI::ScrollContainer::getTrackCornerRadius() {
    return computedScrollContainerStyle.trackCornerRadius;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::ScrollContainer::getTrackFillColor() {
    return computedScrollContainerStyle.trackFillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::ScrollContainer::getTrackHoveredFillColor() {
    return computedScrollContainerStyle.trackHoveredFillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::ScrollContainer::getTrackPressedFillColor() {
    return computedScrollContainerStyle.trackPressedFillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Vector4f SFUI::ScrollContainer::getThumbCornerRadius() {
    return computedScrollContainerStyle.thumbCornerRadius;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::ScrollContainer::getThumbFillColor() {
    return computedScrollContainerStyle.thumbFillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::ScrollContainer::getThumbHoveredFillColor() {
    return computedScrollContainerStyle.thumbHoveredFillColor;
}


/**
 * @brief .
 * 
 * @return .
 */
SFUI::Color SFUI::ScrollContainer::getThumbPressedFillColor() {
    return computedScrollContainerStyle.thumbPressedFillColor;
}


/**
 * @brief .
 */
SFUI::Void SFUI::ScrollContainer::computeAlignPrimary() {
    SFUI::String tempAlignPrimary = layout.alignPrimary;
    std::transform(tempAlignPrimary.begin(), tempAlignPrimary.end(), tempAlignPrimary.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlignPrimary == "start" || tempAlignPrimary == "end") {
        computedLayout.alignPrimary = tempAlignPrimary;
    }   else {
        computedLayout.alignPrimary = "start";
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::ScrollContainer::computeScrollDirection() {
    SFUI::String tempAlign = scrollContainerStyle.scrollBarAlign;
    std::transform(tempAlign.begin(), tempAlign.end(), tempAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempAlign == "vertical" || tempAlign == "horizontal" || tempAlign == "both")
    computedScrollContainerStyle.scrollBarAlign = tempAlign;
    else
    computedScrollContainerStyle.scrollBarAlign = "vertical";
}


/**
 * @brief .
 */
SFUI::Void SFUI::ScrollContainer::computeScrollSpeedFactor() {
    if (scrollContainerStyle.scrollSpeedFactor == 0) {
        computedScrollContainerStyle.scrollSpeedFactor = 15.0f;
        return;
    }
    computedScrollContainerStyle.scrollSpeedFactor = scrollContainerStyle.scrollSpeedFactor;
}


/**
 * @brief .
 */
SFUI::Void SFUI::ScrollContainer::computeChildrenScrollPosition() {
    SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> children = this->getChildren();
    for (int i = 0; i < children.size(); i++) {
        childrenComputedLayout[i].position = {
            childrenComputedLayout[i].position.x + static_cast<SFUI::Int>(scrollOffset.x),
            childrenComputedLayout[i].position.y + static_cast<SFUI::Int>(scrollOffset.y)
        };
    }
}


/**
 * @brief .
 */
SFUI::Void SFUI::ScrollContainer::computeMaxScrollOffset() {
    SFUI::Vector<SFUI::SharedPointer<SFUI::Component>> children = this->getChildren();
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
