/**
 * @file Graphic.hpp
 * @brief Defines the Graphic component and its properties.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.2
 *
 * This file contains the class definition, internal state,
 * and helper functions for the SFUI Graphic component.
 */


#pragma once
#include "Base/Component.hpp"


namespace SFUI {

    /**
     * @brief Graphic UI component that displays images.
     *
     * The Graphic class extends the base Component class and provides
     * functionality for loading, displaying, and managing image graphics.
     */
    class Graphic : public Component {

        public:
            /**
             * @brief Layout properties specific to the Graphic component.
             */
            SFUI::PropGroup::Graphic::Layout layout;

            /**
             * @brief Style properties specific to the Graphic component.
             */
            SFUI::PropGroup::Graphic::Style style;

            /**
             * @brief State properties specific to the Graphic component.
             */
            SFUI::PropGroup::Graphic::State state;

            /**
             * @brief Behavior properties specific to the Graphic component.
             */
            SFUI::PropGroup::Graphic::Behavior behavior;

        public:
            Graphic() = default;
            Graphic(Graphic&&) = default;
            Graphic& operator=(Graphic&&) = default;

            /**
             * @brief Construct a Graphic with an ID.
             */
            Graphic(SFUI::String componentID);

            /**
             * @brief Construct a Graphic with ID, properties, and children.
             *
             * @param componentID Unique identifier.
             * @param propSet Set of graphic property groups.
             * @param children Variadic list of child components.
             */
            template<typename... Children>
            Graphic(SFUI::String componentID, SFUI::PropSet::Graphic propSet, Children&&... children) :
                Component(componentID),
                layout(propSet.layout),
                style(propSet.style),
                state(propSet.state),
                behavior(propSet.behavior),
                // style(std::move(propSet.style)),
                // behavior(std::move(propSet.behavior)),
                graphic(graphicSource)
            {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);
            }

            /**
             * @brief Handle input events for the graphic.
             *
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);

            /**
             * @brief Handle the pre updaate updates for the component.
             */
            SFUI::Void preUpdate();

            /**
             * @brief Update the graphic component.
             *
             * @param renderTargetSize Size of the render target.
             */
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);

            /**
             * @brief Draw the graphic component to the render target.
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

            /**
             * @brief Get the original size of the loaded texture.
             *
             * @return Original texture size.
             */
            SFUI::Vector2f getOriginalTextureSize();

        private:
            /**
             * @brief Load types for the graphic component.
             */
            enum class LoadType { PRE_LOAD, SELF_LOAD };

            /**
             * @brief Load states for the graphic component.
             */
            enum class LoadState { PRE_LOAD_UNLOADED, PRE_LOAD_LOADED, SELF_LOAD_UNLOADED, SELF_LOAD_LOADED, SELF_LOAD_ERROR };

        private:
            /**
             * @brief Texture source of the displayed graphic.
             */
            SFUI::Texture graphicSource;

            /**
             * @brief Sprite used to render the graphic texture.
             */
            SFUI::Sprite graphic;

            /**
             * @brief Current load type and state of the graphic.
             */
            SFUI::Graphic::LoadType loadType = SFUI::Graphic::LoadType::PRE_LOAD;

            /**
             * @brief Current load state of the graphic.
             */
            SFUI::Graphic::LoadState loadState = SFUI::Graphic::LoadState::PRE_LOAD_UNLOADED;

            /**
             * @brief Computed style properties after processing.
             */
            SFUI::ComputedProp::Style::Graphic computedGraphicStyle;

        private:
            /**
             * @brief Compute the graphic source based on load type and state.
             */
            SFUI::Void computeGraphicSource();

            /**
             * @brief Compute the layout of the graphic component.
             */
            SFUI::Void computeGraphicLayout();
    };
}
