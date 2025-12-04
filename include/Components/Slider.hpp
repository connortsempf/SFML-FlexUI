/**
 * @file Slider.hpp
 * @brief Defines the Slider component and its properties.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.0
 *
 * This file contains the class definition, internal state,
 * and helper functions for the SFUI Slider component.
 */


#pragma once
#include "Base/Component.hpp"
#include "Container.hpp"
#include "Button.hpp"


namespace SFUI {

    /**
     * @brief Slider UI component that allows users to select a value from a range.
     *
     * The Slider class extends the base Component class and provides
     * functionality for selecting a value within a defined range using
     * a draggable thumb along a track.
     */
    class Slider : public Component {

        public:
            /**
             * @brief Layout properties specific to the Slider component.
             */
            SFUI::PropGroup::Slider::Layout layout;

            /**
             * @brief Style properties specific to the Slider component.
             */
            SFUI::PropGroup::Slider::Style style;

            /**
             * @brief State properties specific to the Slider component.
             */
            SFUI::PropGroup::Slider::State state;

            /**
             * @brief Behavior properties specific to the Slider component.
             */
            SFUI::PropGroup::Slider::Behavior behavior;

        public:
            Slider() = default;
            Slider(Slider&&) = default;
            Slider& operator=(Slider&&) = default;

            /**
             * @brief Construct a Slider with an ID.
             *
             * @param componentID Unique identifier for this slider.
             */
            Slider(SFUI::String componentID);

            /**
             * @brief Construct a Slider with ID, properties, and children.
             *
             * @param componentID Unique identifier.
             * @param propSet Set of container property groups.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            Slider(SFUI::String componentID, SFUI::PropSet::Slider propSet, Children&&... children) :
                Component(componentID),
                layout(propSet.layout),
                style(propSet.style),
                state(propSet.state),
                behavior(propSet.behavior),
                // style(propSet.style),
                // state(std::move(propSet.state)),
                // behavior(std::move(propSet.behavior)),
                unprogressedTrack(componentID + "_UnprogressedTrack"),
                progressedTrack(componentID + "_ProgressedTrack"),
                thumb(componentID + "_Thumb")
            {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);
            }

            /**
             * @brief Handle input events for the slider.
             *
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);

            /**
             * @brief Handle the pre updaate updates for the component.
             */
            SFUI::Void preUpdate();

            /**
             * @brief Update slider state.
             *
             * @param renderTargetSize Size of the render target.
             */
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);

            /**
             * @brief Draw the slider to the render target.
             *
             * @param drawTarget Target to draw on.
             * @param window Window reference.
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
             * @brief Whether the mouse is hovering over the slider track.
             */
            SFUI::Bool isTrackHovered = false;

            /**
             * @brief Whether the mouse is hovering over the slider thumb.
             */
            SFUI::Bool isThumbHovered = false;

            /**
             * @brief Whether the left mouse button is pressing the track.
             */
            SFUI::Bool isTrackLeftPressed = false;

            /**
             * @brief Whether the right mouse button is pressing the track.
             */
            SFUI::Bool isTrackRightPressed = false;

            /**
             * @brief Whether the middle mouse button is pressing the track.
             */
            SFUI::Bool isTrackMiddlePressed = false;

            /**
             * @brief Whether the left mouse button is pressing the thumb.
             */
            SFUI::Bool isThumbLeftPressed = false;

            /**
             * @brief Whether the right mouse button is pressing the thumb.
             */
            SFUI::Bool isThumbRightPressed = false;

            /**
             * @brief Whether the middle mouse button is pressing the thumb.
             */
            SFUI::Bool isThumbMiddlePressed = false;

            /**
             * @brief Whether the slider is currently in a sliding (dragging) interaction.
             */
            SFUI::Bool isSliding = false;

            /**
             * @brief Numerical values corresponding to each slider interval (useful for discrete modes).
             */
            SFUI::Vector<SFUI::Float> intervalValues;

            /**
             * @brief Normalized positions (0–1) of each interval marker along the slider track.
             */
            SFUI::Vector<SFUI::Float> intervalPositions;

            /**
             * @brief Cached thumb position used during sliding updates.
             */
            SFUI::Vector2f thumbUpdatePosition = {0.0f, 0.0f};

            /**
             * @brief The visual container representing the unprogressed portion of the slider track.
             */
            SFUI::Container unprogressedTrack;

            /**
             * @brief The visual container representing the progressed portion of the slider track.
             */
            SFUI::Container progressedTrack;

            /**
             * @brief The slider thumb button component.
             */
            SFUI::Button thumb;

            /**
             * @brief Fully computed slider style used for rendering.
             */
            SFUI::ComputedProp::Style::Slider computedSliderStyle;

        private:
            /**
             * @brief Compute the alignment of the track (horizontal/vertical).
             */
            SFUI::Void computeTrackAlign();

            /**
             * @brief Compute dynamics related to the slider's value.
             */
            SFUI::Void computeValueDynamics();

            /**
             * @brief Compute dynamic colors based on the slider's state.
             */
            SFUI::Void computeDynamicColors();

            /**
             * @brief Compute the position and size of the thumb.
             */
            SFUI::Void computeThumb();

            /**
             * @brief Compute the sizes and positions of the tracks.
             */
            SFUI::Void computeTracks();

            /**
             * @brief Determine if the track is hovered based on mouse position.
             *
             * @param mousePosition Current mouse position.
             *
             * @return True if the track is hovered, false otherwise.
             */
            SFUI::Bool trackHovered(SFUI::Vector2i mousePosition);

            /**
             * @brief Determine if the thumb is hovered based on mouse position.
             *
             * @param mousePosition Current mouse position.
             *
             * @return True if the thumb is hovered, false otherwise.
             */
            SFUI::Void handleThumbMove(SFUI::Vector2i mousePosition);
    };
}
