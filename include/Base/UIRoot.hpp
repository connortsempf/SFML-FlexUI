/**
 * @file UIRoot.hpp
 * @brief Defines the UIRoot class which manages the UI hierarchy and rendering.
 * @author Connor Sempf
 * @date 2025-11-15
 * @version 1.0
 *
 * This file contains the class definition for UIRoot, which handles
 * top-level UI organization, event propagation, rendering, and
 * management of all components in an SFUI application.
 */


#pragma once
#include "Types/base.hpp"
#include "Types/render.hpp"
#include "Types/vector.hpp"
#include "Component.hpp"
#include <deque>


namespace SFUI {

    class Component;
    
    /**
     * @brief Root UI manager that handles the top-level component and event propagation.
     * 
     * The UIRoot class manages the root component of the UI hierarchy,
     * propagates events, updates, and handles rendering to the target.
     */
    class UIRoot {

        public:
            UIRoot() = default;
            
            /**
             * @brief Construct a UIRoot with a root component.
             * 
             * @param rootComponent Unique pointer to the root component.
             */
            UIRoot(SFUI::UniquePointer<SFUI::Component> rootComponent);

            /**
             * @brief Set the root component of the UI.
             * 
             * @param rootComponent Unique pointer to the new root component.
             */
            SFUI::Void setRootComponent(SFUI::UniquePointer<SFUI::Component> rootComponent);

            /**
             * @brief Handle an input event by propagating it to the root component.
             * 
             * @param event Event to process.
             */
            SFUI::Void handleEvent(const SFUI::Event& event);

            /**
             * @brief Update the UI state.
             * 
             * @param renderTargetSize Size of the render target.
             */
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            
            /**
             * @brief Draw the UI to the render target.
             * 
             * @param drawTarget Target to draw on.
             * @param window Window reference.
             */
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);

        private:
            /**
             * @brief Unique pointer to the root component of the UI.
             */
            SFUI::UniquePointer<SFUI::Component> rootComponent;
        
        private:
            /**
             * @brief Recursively draw a component and its children.
             * 
             * @param component Component to draw.
             * @param drawTarget Target to draw on.
             * @param window Window reference.
             */
            SFUI::Void drawRecursive(const SFUI::UniquePointer<SFUI::Component>& component, SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
    };
}
