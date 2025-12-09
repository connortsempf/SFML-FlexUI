/**
 * @file ScrollContainer.hpp
 * @brief Defines the ScrollContainer component and its properties.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.1
 *
 * This file contains the class definition, internal state,
 * and helper functions for the SFUI ScrollContainer component.
 */


#pragma once
#include "Base/Component.hpp"


namespace SFUI {

    /**
     * @brief ScrollContainer UI component that provides scrollable content area.
     *
     * The ScrollContainer class extends the base Component class and provides
     * functionality for displaying a scrollable area that can contain other components.
     */
    class ScrollContainer : public Component {

        public:
            /**
             * @brief Layout properties specific to the ScrollContainer component.
             */
            SFUI::PropGroup::ScrollContainer::Layout layout;

            /**
             * @brief Style properties specific to the ScrollContainer component.
             */
            SFUI::PropGroup::ScrollContainer::Style style;

            /**
             * @brief State properties specific to the ScrollContainer component.
             */
            SFUI::PropGroup::ScrollContainer::State state;

            /**
             * @brief Behavior properties specific to the ScrollContainer component.
             */
            SFUI::PropGroup::ScrollContainer::Behavior behavior;

        public:
            ScrollContainer() = default;
            ScrollContainer(ScrollContainer&&) = default;
            ScrollContainer& operator=(ScrollContainer&&) = default;

            /**
             * @brief Construct a ScrollContainer with an ID.
             *
             * @param componentID Unique identifier for this scroll container.
             */
            ScrollContainer(SFUI::String componentID);

            /**
             * @brief Construct a ScrollContainer with ID, properties, and children.
             *
             * @param componentID Unique identifier.
             * @param propSet Set of scroll container property groups.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            ScrollContainer(SFUI::String componentID, SFUI::PropSet::ScrollContainer propSet, Children&&... children) :
                Component(componentID),
                layout(propSet.layout),
                style(propSet.style),
                state(propSet.state),
                behavior(propSet.behavior)
                // style(std::move(propSet.style)),
                // behavior(std::move(propSet.behavior))
            {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);
            }

            /**
             * @brief Handle input events for the scroll container.
             *
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);

            /**
             * @brief Handle the pre updaate updates for the component.
             */
            SFUI::Void preUpdate();

            /**
             * @brief Update the scroll container and its children.
             *
             * @param renderTargetSize Size of the render target.
             */
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);

            /**
             * @brief Draw the scroll container to the render target.
             *
             * @param drawTarget Render target to draw to.
             * @param window Render window context.
             */
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);

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
            SFUI::Void drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);

        private:
            /**
             * @brief Whether the mouse is hovering over the scroll container.
             */
            SFUI::Bool isHovered = false;

            /**
             * @brief Current scroll offset of the content (x/y).
             */
            SFUI::Vector2f scrollOffset = {0.0f, 0.0f};

            /**
             * @brief Maximum allowed scroll offset based on content size.
             */
            SFUI::Vector2f maxScrollOffset = {0.0f, 0.0f};

            /**
             * @brief Total computed size of the scrollable content.
             */
            SFUI::Vector2f contentSize;

            /**
             * @brief Position where the drag gesture began on the scroll thumb.
             */
            SFUI::Float dragStartPosition;

            /**
             * @brief Fully computed and resolved scroll container style values.
             */
            SFUI::ComputedProp::Style::ScrollContainer computedScrollContainerStyle;

        private:
            /**
             * @brief Compute the layout of the scroll container and its children's alignments.
             */
            SFUI::Void computeAlignPrimary();

            /**
             * @brief Compute the scroll offset and bounds.
             */
            SFUI::Void computeScrollDynamics();

            /**
             * @brief Compute the new position of the children based on the current scroll offset.
             */
            SFUI::Void computeChildrenScrollPosition();
    };
}
