/**
 * @file Component.hpp
 * @brief Defines the base Component class for all SFUI components.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.1
 *
 * This file contains the class definition, internal state,
 * and helper functions for the SFUI Component base class.
 */


#pragma once
#include "Base/Animation.hpp"
#include "Types/base.hpp"
#include "Types/render.hpp"
#include "Types/vector.hpp"
#include "Types/prop.hpp"
#include "Types/propgroup.hpp"
#include "Types/propset.hpp"
#include "Types/computedprop.hpp"
#include <algorithm>
#include <cmath>


namespace SFUI {

    /**
     * @brief Base class for all UI components in SFML-FlexUI.
     *
     * Components are arranged in a parent-child hierarchy. This class
     * handles layout, style, event propagation, and rendering.
     */
    class Component {

        friend class UIRoot;

        protected:
            struct ComputedChildLayout;

        public:
            /**
             * @brief Unique identifier for this component.
             */
            SFUI::String componentID;

        public:
            Component() = default;
            Component(const Component&) = delete;
            Component(Component&&) = default;
            Component& operator=(Component&&) = default;
            Component& operator=(const Component&) = delete;
            virtual ~Component() = default;

            /**
             * @brief Construct a component with an ID.
             *
             * @param componentID Unique identifier for this component.
             */
            Component(SFUI::String componentID);

            /**
             * @brief Handle input events for the component.
             *
             * @param event Event to process.
             */
            virtual SFUI::Void handleEvent(const SFUI::Event& event) = 0;

            /**
             * @brief Handle the pre updaate updates for the component.
             */
            virtual SFUI::Void preUpdate() = 0;

            /**
             * @brief Update component state.
             *
             * @param renderTargetSize Size of the render target.
             */
            virtual SFUI::Void update(const SFUI::Vector2u renderTargetSize) = 0;

            /**
             * @brief Draw the component to the render target.
             *
             * @param drawTarget Target to draw on.
             * @param window Window reference.
             */
            virtual SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) = 0;

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
            virtual SFUI::Void drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) = 0;

            /**
             * @brief Set a parent component for this component.
             *
             * @param newParent Pointer to the parent component.
             */
            SFUI::Void setParent(SFUI::Component* newParent);

            /**
             * @brief Add a single child component.
             *
             * @param newChild Unique pointer to the child component.
             */
            SFUI::Void addChild(SFUI::UniquePointer<SFUI::Component> newChild);

            /**
             * @brief Add multiple child components at once.
             *
             * @param newChildren Vector of unique pointers to child components.
             */
            SFUI::Void addChildren(SFUI::Vector<SFUI::UniquePointer<SFUI::Component>> newChildren);

            /**
             * @brief Update a child's computed layout from the parent.
             *
             * @param computedChildLayout Computed layout of the child.
             */
            SFUI::Void updateChildFromParent(SFUI::Component::ComputedChildLayout computedChildLayout);

            /**
             * @brief Get the list of children.
             *
             * @return Vector of unique pointers to children.
             */
            const SFUI::Vector<SFUI::UniquePointer<SFUI::Component>>& getChildren() const;

            /**
             * @brief Get computed size of the component.
             */
            const SFUI::Vector2f& getSize() const;

            /**
             * @brief Get computed position of the component.
             */
            const SFUI::Vector2i& getPosition() const;

            /**
             * @brief Get computed padding of the component.
             */
            const SFUI::Vector4f& getPadding() const;

            /**
             * @brief Get computed margin of the component.
             */
            const SFUI::Vector4f& getMargin() const;

        protected:
            /**
             * @brief Alignment direction for component's children.
             */
            enum class AlignDirection { Vertical, Horizontal };

            /**
             * @brief On-Axis alignment options.
             */
            enum class AlignPrimary { Start, End, Center, SpaceBetween, SpaceAround, SpaceEvenly };

            /**
             * @brief Cross-Axis alignment options.
             */
            enum class AlignSecondary { Start, End, Center };

        protected:
            /**
             * @brief Computed layout properties after resolving sub-properties.
             */
            struct ComputedLayout {
                SFUI::Component::AlignDirection alignDirection;
                SFUI::Component::AlignPrimary alignPrimary;
                SFUI::Component::AlignSecondary alignSecondary;
                SFUI::Vector2f size;
                SFUI::Vector2i position;
                SFUI::Vector4f padding;
                SFUI::Vector4f margin;
            };

            /**
             * @brief Computed layout for a child component.
             */
            struct ComputedChildLayout {
                SFUI::Vector2f size;
                SFUI::Vector2i position;
                SFUI::Vector4f margin;
            };

            /**
             * @brief Computed style properties after resolving sub-properties.
             */
            struct ComputedStyle {
                SFUI::Float borderWidth;
                SFUI::Vector4f cornerRadius;
                SFUI::Color fillColor;
                SFUI::Color borderColor;
                SFUI::Vector2f shadowOffset;
                SFUI::Float shadowRadius;
                SFUI::Color shadowFillColor;
            };

        protected:
            /**
             * @brief Layout properties of this component.
             */
            SFUI::PropGroup::Component::Layout baseLayout;

            /**
             * @brief Style properties of this component.
             */
            SFUI::PropGroup::Component::Style baseStyle;

            /**
             * @brief State properties of this component.
             */
            SFUI::PropGroup::Component::State baseState;

            /**
             * @brief Behavior properties of this component.
             */
            SFUI::PropGroup::Component::Behavior baseBehavior;

            /**
             * @brief Size of the render target that this component gets drawn to.
             */
            SFUI::Vector2u renderTargetSize;

            /**
             * @brief Pointer to the parent component.
             */
            SFUI::Component* parent = nullptr;

            /**
             * @brief List of child components stored as unique pointers.
             */
            SFUI::Vector<SFUI::UniquePointer<SFUI::Component>> children;

            /**
             * @brief List of animations to perform on the component's props.
             */
            SFUI::Vector<SFUI::Animation> animations;

            /**
             * @brief Vertex arrays for rendering geometry with just rectangles.
             */
            SFUI::VertexArray backgroundRects;

            /**
             * @brief Vertex arrays for rendering geometry with rounded corners.
             */
            SFUI::VertexArray backgroundArcs;

            /**
             * @brief Vertex arrays for rendering border geometry with just rectangles.
             */
            SFUI::VertexArray borderRects;

            /**
             * @brief Vertex arrays for rendering border geometry with rounded corners.
             */
            SFUI::VertexArray borderArcs;

            /**
             * @brief Vertex arrays for rendering shadow geometry with just rectangles.
             */
            SFUI::VertexArray shadowRects;

            /**
             * @brief Vertex arrays for rendering shadow geometry with rounded corners.
             */
            SFUI::VertexArray shadowArcs;

            /**
             * @brief Final computed layout properties.
             */
            SFUI::Component::ComputedLayout computedLayout;

            /**
             * @brief Final computed style properties.
             */
            SFUI::Component::ComputedStyle computedStyle;

            /**
             * @brief Final computed layouts of child components.
             */
            SFUI::Vector<SFUI::Component::ComputedChildLayout> computedChildrenLayout;

        protected:
            /**
             * @brief Check if the mouse is hovering over this component.
             *
             * @param mousePosition Current mouse position.
             *
             * @return true if hovered, false otherwise.
             */
            SFUI::Bool isMouseHovered(const SFUI::Vector2i& mousePosition);

            /**
             * @brief Resolve a Dimension sub-property to a color value.
             *
             * @param dimension Dimension sub-property.
             * @param reference Reference size for percentage calculations.
             *
             * @return Resolved color value.
             */
            SFUI::Color resolveColorSubProp(const SFUI::Prop::Color& color);

            /**
             * @brief Resolve a Dimension sub-property to a 4-value float vector .
             *
             * @param dimension Dimension sub-property.
             * @param reference Reference size for percentage calculations.
             *
             * @return Resolved float vector.
             */
            SFUI::Vector4f resolveUniQuadSubProp(const SFUI::Vector2f& size, const SFUI::Prop::UniQuad& subProp);

            /**
             * @brief Compute the alignment properties.
             */
            SFUI::Void computeAlignment();

            /**
             * @brief Compute all necessary properties for geometry and layout.
             */
            SFUI::Void computeLayoutBox();

            /**
             * @brief Compute all style-related properties.
             */
            SFUI::Void computeStyles();

            /**
             * @brief Compute color style properties.
             */
            SFUI::Void computeColors();

            /**
             * @brief Compute shadow properties.
             */
            SFUI::Void computeShadows();

            /**
             * @brief Compute all graphics and drawable geometry.
             */
            SFUI::Void computeGraphics();

            /**
             * @brief Compute layout and geometry for all child components.
             */
            SFUI::Void computeChildrenLayoutBox();

            /**
             * @brief Update all child components with their computed layout peroperties.
             */
            SFUI::Void updateChildren();

        private:
            /**
             * @brief Compute rectangular geometry for the component's main background.
             *
             * @param position Top-left position of the background.
             * @param size Size of the background.
             */
            SFUI::Void computeBackgroundRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size);

            /**
             * @brief Compute arc geometry for the component's main background.
             *
             * @param center Center position of the arc.
             * @param outerRadius Outer radius of the arc.
             * @param startAngleDeg Starting angle in degrees.
             * @param endAngleDeg Ending angle in degrees.
             */
            SFUI::Void computeBackgroundArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg);

            /**
             * @brief Compute rectangular geometry for the component's border.
             *
             * @param position Top-left position of the border.
             * @param size Size of the border.
             */
            SFUI::Void computeBorderRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size);

            /**
             * @brief Compute arc geometry for the component's border.
             *
             * @param center Center position of the arc.
             * @param outerRadius Outer radius of the arc.
             * @param innerRadius Inner radius of the arc.
             * @param startAngleDeg Starting angle in degrees.
             * @param endAngleDeg Ending angle in degrees.
             */
            SFUI::Void computeBorderArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float innerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg);

            /**
             * @brief Compute rectangular geometry for the component's shadow.
             *
             * @param position Top-left position of the shadow.
             * @param size Size of the shadow.
             * @param modifiedShadowColor Color of the shadow with applied alpha.
             */
            SFUI::Void computeShadowRectGeometry(SFUI::Vector2f position, SFUI::Vector2f size, SFUI::Color modifiedShadowColor);

            /**
             * @brief Compute arc geometry for the component's shadow.
             *
             * @param center Center position of the arc.
             * @param outerRadius Outer radius of the arc.
             * @param startAngleDeg Starting angle in degrees.
             * @param endAngleDeg Ending angle in degrees.
             * @param modifiedShadowColor Color of the shadow with applied alpha.
             */
            SFUI::Void computeShadowArcGeometry(SFUI::Vector2f center, SFUI::Float outerRadius, SFUI::Float startAngleDeg, SFUI::Float endAngleDeg, SFUI::Color modifiedShadowColor);
    };
}
