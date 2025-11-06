#include <sfml-flexui-types.hpp>
#include <sfml-flexui-core.hpp>


//////////////////////////////////////////////////
// SFML-FlexUI Scroll Container Component Class //
//////////////////////////////////////////////////


/**
 * @brief The constructor for ScrollContainer.
 * 
 * @param componentID The unique identifier for the scroll container component.
 */
SFUI::ScrollContainer::ScrollContainer(const SFUI::String& componentID) :
    Component(componentID)
{}


/**
 * @brief The constructor for ScrollContainer.
 * 
 * @param componentID The unique identifier for the scroll container component.
 * @param scrollContainerPropGroup The property group for the scroll container component.
 */
SFUI::ScrollContainer::ScrollContainer(const SFUI::String& componentID, const SFUI::PropGroup::ScrollContainer& scrollContainerPropGroup) :
    Component(componentID, scrollContainerPropGroup.layout, scrollContainerPropGroup.style),
    scrollContainerStyle(scrollContainerPropGroup.scrollContainerStyle),
    scrollContainerBehavior(scrollContainerPropGroup.scrollContainerBehavior)
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
        if (mouseWheelScrolledEvent->wheel == sf::Mouse::Wheel::Vertical) {
            if (computedScrollContainerStyle.scrollDirection == "vertical" || computedScrollContainerStyle.scrollDirection == "both") {
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
            if (computedScrollContainerStyle.scrollDirection == "horizontal" || computedScrollContainerStyle.scrollDirection == "both") {
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
 * @brief Compute primary alignment for the scroll container.
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
 * @brief Compute scroll dynamics for the scroll container.
 */
SFUI::Void SFUI::ScrollContainer::computeScrollDynamics() {
    // Scroll Direction //
    SFUI::String tempAlign = scrollContainerStyle.scrollDirection;
    std::transform(tempAlign.begin(), tempAlign.end(), tempAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    if (tempAlign == "vertical" || tempAlign == "horizontal" || tempAlign == "both")
        computedScrollContainerStyle.scrollDirection = tempAlign;
    else
        computedScrollContainerStyle.scrollDirection = "vertical";

    // Scroll Speed Factor //
    if (scrollContainerStyle.scrollSpeedFactor == 0) computedScrollContainerStyle.scrollSpeedFactor = 15.0f;
    else computedScrollContainerStyle.scrollSpeedFactor = scrollContainerStyle.scrollSpeedFactor;

    // Maximum Scroll Offset //
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


/**
 * @brief Compute the scroll position of all child components.
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
