/**
 * @file ScrollContainer.hpp
 * @brief Defines the ScrollContainer component and its properties.
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
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
             * @brief Style properties specific to the ScrollContainer component.
             */
            SFUI::Prop::Style::ScrollContainer scrollContainerStyle;
            
            /**
             * @brief Behavior properties specific to the ScrollContainer component.
             */
            SFUI::Prop::Behavior::ScrollContainer scrollContainerBehavior;

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
             * @param scrollContainerPropGroup Group of scroll container properties.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            ScrollContainer(SFUI::String componentID, SFUI::PropGroup::ScrollContainer scrollContainerPropGroup, Children&&... children) :
                Component(std::move(componentID), std::move(scrollContainerPropGroup.layout), std::move(scrollContainerPropGroup.style)),
                scrollContainerStyle(std::move(scrollContainerPropGroup.scrollContainerStyle)),
                scrollContainerBehavior(std::move(scrollContainerPropGroup.scrollContainerBehavior)) {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);

            }

            /**
             * @brief Handle input events for the scroll container.
             * 
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);
            
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

        private:
            /**
             * @brief Whether the mouse is hovering over the scroll container.
             */
            SFUI::Bool isHovered = false;

            /**
             * @brief Whether the mouse is hovering over the scroll track region.
             */
            SFUI::Bool isTrackHovered = false;

            /**
             * @brief Whether the mouse is hovering over the scroll thumb.
             */
            SFUI::Bool isThumbHovered = false;

            /**
             * @brief Whether the scroll track is currently being pressed.
             */
            SFUI::Bool isTrackPressed = false;

            /**
             * @brief Whether the scroll thumb is currently being pressed.
             */
            SFUI::Bool isThumbPressed = false;

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
             * @brief Dirty (unresolved) style properties for the scroll container.
             */
            SFUI::Prop::Style::ScrollContainer dirtyScrollContainerStyle;

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
