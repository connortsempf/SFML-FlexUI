/**
 * @file Slider.cpp
 * @brief Implements the SFUI Slider component.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.1
 *
 * This file contains the function definitions and internal logic for the
 * SFUI Slider component. It handles:
 *   - Construction and destruction
 *   - Event handling (dragging, value changes)
 *   - Rendering and style updates
 *
 * It works in conjunction with Slider.hpp to provide full functionality
 * of the Slider component.
 */


#include "Components/Slider.hpp"


/**
 * @brief The constrctor for the slider component.
 *
 * @param componentID The unique identifier for the slider component.
 */
SFUI::Slider::Slider(SFUI::String componentID) :
    Component(componentID),
    unprogressedTrack(componentID + "_UnprogressedTrack"),
    progressedTrack(componentID + "_ProgressedTrack"),
    thumb(componentID + "_Thumb")
{}


/**
 * @brief Handle a user input event.
 *
 * @param event The event to handle.
 */
SFUI::Void SFUI::Slider::handleEvent(const SFUI::Event& event) {
    thumb.handleEvent(event);

    // Mouse Moved Event Handling //
    if (const SFUI::Event::MouseMoved* mouseMovedEvent = event.getIf<SFUI::Event::MouseMoved>()) {
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseMovedEvent->position.x, mouseMovedEvent->position.y);

        // Track Handling //
        const SFUI::Bool trackIsHovered = trackHovered(mousePosition);
        if (!isTrackHovered && trackIsHovered && behavior.onTrackHoverIn) behavior.onTrackHoverIn(componentID);
        if (isTrackHovered && !trackIsHovered && behavior.onTrackHoverOut) behavior.onTrackHoverOut(componentID);
        isTrackHovered = trackIsHovered;

        // Thumb Handling //
        if (isSliding) handleThumbMove(mousePosition);
    }

    // Mouse Button Pressed Event //
    else if (const SFUI::Event::MouseButtonPressed* mouseButtonPressedEvent = event.getIf<SFUI::Event::MouseButtonPressed>()) {
        const SFUI::Vector2i mousePosition = SFUI::Vector2i(mouseButtonPressedEvent->position.x, mouseButtonPressedEvent->position.y);
        const sf::Mouse::Button mouseButton = mouseButtonPressedEvent->button;

        if (mouseButton == sf::Mouse::Button::Left) state.isFocused = false;

        // Track Handling //
        if (isTrackHovered && !isThumbHovered) {
            if (mouseButton == sf::Mouse::Button::Left) {
                isTrackLeftPressed = true;
                if (behavior.onThumbLeftPressIn) behavior.onThumbLeftPressIn(componentID);
                handleThumbMove(mousePosition);
            }
            else if (mouseButton == sf::Mouse::Button::Right) {
                isTrackRightPressed = true;
                if (behavior.onThumbRightPressIn) behavior.onThumbRightPressIn(componentID);
            }
            else if (mouseButton == sf::Mouse::Button::Middle) {
                isTrackMiddlePressed = true;
                if (behavior.onThumbMiddlePressIn) behavior.onThumbMiddlePressIn(componentID);
            }
        }
    }

    // Mouse Button Released Event //
    else if (const SFUI::Event::MouseButtonReleased* mouseButtonReleasedEvent = event.getIf<SFUI::Event::MouseButtonReleased>()) {
        const sf::Mouse::Button mouseButton = mouseButtonReleasedEvent->button;

        // Track Handling //
        if (isTrackHovered && !isThumbHovered) {
            if (mouseButton == sf::Mouse::Button::Left) {
                if (isTrackLeftPressed && behavior.onThumbLeftPress) behavior.onThumbLeftPress(componentID);
            }
            else if (mouseButton == sf::Mouse::Button::Right) {
                if (isTrackRightPressed && behavior.onThumbRightPress) behavior.onThumbRightPress(componentID);
            }
            else if (mouseButton == sf::Mouse::Button::Middle) {
                if (isTrackMiddlePressed && behavior.onThumbMiddlePress) behavior.onThumbMiddlePress(componentID);
            }
        }
        isTrackLeftPressed = isTrackRightPressed = isTrackMiddlePressed = false;

        // Thumb Handling //
        if (isSliding && behavior.onSlidingEnd) behavior.onSlidingEnd(componentID, state.value);
        isSliding = false;
    }

    // Key Pressed Event //
    if (const SFUI::Event::KeyPressed* keyPressedEvent = event.getIf<SFUI::Event::KeyPressed>()) {
        if (state.isFocused) {
            SFUI::Float minValue = std::min(intervalValues.front(), intervalValues.back());
            SFUI::Float maxValue = std::max(intervalValues.front(), intervalValues.back());
            if (
                ((style.trackAlign == "horizontal") &&
                ((keyPressedEvent->code == sf::Keyboard::Key::Left && !state.isInverted) ||
                (keyPressedEvent->code == sf::Keyboard::Key::Right && state.isInverted))) ||
                ((style.trackAlign == "vertical") &&
                (keyPressedEvent->code == sf::Keyboard::Key::Down && !state.isInverted) ||
                (keyPressedEvent->code == sf::Keyboard::Key::Up && state.isInverted))) {
                    state.value = std::clamp(state.value - state.step, minValue, maxValue);
            }
            else if (
                ((style.trackAlign == "horizontal") &&
                ((keyPressedEvent->code == sf::Keyboard::Key::Right && !state.isInverted) ||
                (keyPressedEvent->code == sf::Keyboard::Key::Left && state.isInverted))) ||
                (style.trackAlign == "vertical") &&
                ((keyPressedEvent->code == sf::Keyboard::Key::Up && !state.isInverted) ||
                (keyPressedEvent->code == sf::Keyboard::Key::Down && state.isInverted))) {
                    state.value = std::clamp(state.value + state.step, minValue, maxValue);
            }
        }
    }
}


/**
 * @brief Handle the pre updaate updates for the component.
 */
SFUI::Void SFUI::Slider::preUpdate() {
    this->baseLayout = this->layout;
    this->baseStyle = this->style;
    this->baseState = this->state;
    unprogressedTrack.preUpdate();
    progressedTrack.preUpdate();
    thumb.preUpdate();
}


/**
 * @brief Recalculate the slider component's properties.
 *
 * @param renderTargetSize The size of the render target the component is being drawn to.
 */
SFUI::Void SFUI::Slider::update(const SFUI::Vector2u renderTargetSize) {
    this->renderTargetSize = renderTargetSize;
    computeAlignment();
    computeLayoutBox();
    computeStyles();
    computeColors();
    computeShadows();
    computeGraphics();
    computeChildrenLayoutBox();
    updateChildren();
    computeTrackAlign();
    computeValueDynamics();
    computeDynamicColors();
    computeThumb();
    computeTracks();
}


/**
 * @brief Draw the slider component to a render target.
 *
 * @param drawTarget The render target to draw to.
 * @param window The render window associated with the render target.
 */
SFUI::Void SFUI::Slider::draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    drawTarget.draw(shadowRects);
    drawTarget.draw(shadowArcs);
    drawTarget.draw(backgroundRects);
    drawTarget.draw(backgroundArcs);
    drawTarget.draw(borderRects);
    drawTarget.draw(borderArcs);
    unprogressedTrack.draw(drawTarget, window);
    progressedTrack.draw(drawTarget, window);
    thumb.draw(drawTarget, window);
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
SFUI::Void SFUI::Slider::drawOverlay(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window) {
    thumb.drawOverlay(drawTarget, window);
}


/**
 * @brief Compute the alignment of the tracks within its bounding box.
 */
SFUI::Void SFUI::Slider::computeTrackAlign() {
    SFUI::String tempTrackAlign = style.trackAlign;
    std::transform(tempTrackAlign.begin(), tempTrackAlign.end(), tempTrackAlign.begin(), [](unsigned char c) {
        return std::tolower(c);
    });

    if (tempTrackAlign == "horizontal" || tempTrackAlign == "vertical")
        computedSliderStyle.trackAlign = tempTrackAlign;
    else
        computedSliderStyle.trackAlign = "horizontal";
}


/**
 * @brief Compute the value dynamics of the slider component.
 */
SFUI::Void SFUI::Slider::computeValueDynamics() {
    // Sanitize Minimum and Maximum Values //
    if (state.minimumValue > state.maximumValue) std::swap(state.minimumValue, state.maximumValue);
    if (state.minimumValue == state.maximumValue) {
        state.minimumValue = 0.0f;
        state.maximumValue = 100.0f;
    }

    SFUI::Float valueRange = state.maximumValue - state.minimumValue;
    SFUI::UnsignedInt numSteps = static_cast<SFUI::UnsignedInt>(std::floor(valueRange / state.step));
    if (numSteps <= 0) numSteps = 1u;
    SFUI::Float distanceInterval = 0.0f;
    if (computedSliderStyle.trackAlign == "horizontal")
        distanceInterval = ((computedLayout.position.x + computedLayout.size.x) - computedLayout.position.x) / static_cast<SFUI::Float>(numSteps);
    else if (computedSliderStyle.trackAlign == "vertical")
        distanceInterval = ((computedLayout.position.y + computedLayout.size.y) - computedLayout.position.y) / static_cast<SFUI::Float>(numSteps);

    // Calculate Interval Values //
    intervalValues.clear();
    for (int i = 0; i < numSteps + 1; i++)
        intervalValues.push_back(state.minimumValue + (i * state.step));

    // Calculate Interval Positions //
    intervalPositions.clear();
    for (int i = 0; i < numSteps + 1; i++) {
        SFUI::Float newPosition = 0.0f;
        if (computedSliderStyle.trackAlign == "horizontal")
            newPosition = !state.isInverted ? computedLayout.position.x + (i * distanceInterval) : computedLayout.position.x + computedLayout.size.x - (i * distanceInterval);
        else if (computedSliderStyle.trackAlign == "vertical")
            newPosition = !state.isInverted ? computedLayout.position.y + computedLayout.size.y - (i * distanceInterval) : computedLayout.position.y + (i * distanceInterval);
        intervalPositions.push_back(newPosition);
    }

    // React to Programmatic Changes to Value //
    SFUI::Float potentialNewValue = std::clamp(state.value, state.minimumValue, state.maximumValue);
    SFUI::UnsignedInt nearestIndex = 0u;
    SFUI::Float smallestDistance = std::numeric_limits<SFUI::Float>::max();
    for (SFUI::UnsignedInt i = 0; i < intervalValues.size(); ++i) {
        SFUI::Float distance = std::abs(intervalValues[i] - potentialNewValue);
        if (distance < smallestDistance) {
            smallestDistance = distance;
            nearestIndex = i;
        }
    }
    SFUI::Float snappedValue = intervalValues.at(nearestIndex);
    if (state.value != snappedValue && behavior.onValueChange) {
        behavior.onValueChange(componentID, snappedValue);
    }
    state.value = snappedValue;
    if (computedSliderStyle.trackAlign == "horizontal") {
        thumbUpdatePosition.x = intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.x / 2.0f);
        thumbUpdatePosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedSliderStyle.thumbSize.y / 2.0f);
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        thumbUpdatePosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedSliderStyle.thumbSize.x / 2.0f);
        thumbUpdatePosition.y = intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.y / 2.0f);
    }
}


/**
 * @brief Compute the dynamic colors of the slider component.
 */
SFUI::Void SFUI::Slider::computeDynamicColors() {
    unprogressedTrack.style.fillColor = style.trackFillColor;
    unprogressedTrack.style.borderColor = style.trackBorderColor;
    progressedTrack.style.fillColor = style.trackFillColor;
    progressedTrack.style.borderColor = style.trackBorderColor;
    thumb.style.fillColor = style.thumbFillColor;
    thumb.style.borderColor = style.thumbBorderColor;

    if (style.trackUnprogressedFillColor.has_value())
        unprogressedTrack.style.fillColor = style.trackUnprogressedFillColor.value();
    if (style.trackUnprogressedBorderColor.has_value())
        unprogressedTrack.style.borderColor = style.trackUnprogressedBorderColor.value();
    if (style.trackProgressedFillColor.has_value())
        progressedTrack.style.fillColor = style.trackProgressedFillColor.value();
    if (style.trackProgressedBorderColor.has_value())
        progressedTrack.style.borderColor = style.trackProgressedBorderColor.value();

    if (state.isDisabled) {
        if (style.trackUnprogressedDisabledFillColor.has_value())
            unprogressedTrack.style.fillColor = style.trackUnprogressedDisabledFillColor.value();
        if (style.trackUnprogressedDisabledBorderColor.has_value())
            unprogressedTrack.style.borderColor = style.trackUnprogressedDisabledBorderColor.value();
        if (style.trackProgressedDisabledFillColor.has_value())
            progressedTrack.style.fillColor = style.trackProgressedDisabledFillColor.value();
        if (style.trackProgressedDisabledBorderColor.has_value())
            progressedTrack.style.borderColor = style.trackProgressedDisabledBorderColor.value();
        if (style.thumbDisabledFillColor.has_value())
            thumb.style.fillColor = style.thumbDisabledFillColor.value();
        if (style.thumbDisabledBorderColor.has_value())
            thumb.style.borderColor = style.thumbDisabledBorderColor.value();
    }
    else if (isTrackLeftPressed || isTrackRightPressed || isTrackMiddlePressed || isThumbLeftPressed || isThumbRightPressed || isThumbMiddlePressed) {
        if (style.trackUnprogressedPressedFillColor.has_value())
            unprogressedTrack.style.fillColor = style.trackUnprogressedPressedFillColor.value();
        if (style.trackUnprogressedPressedBorderColor.has_value())
            unprogressedTrack.style.borderColor = style.trackUnprogressedPressedBorderColor.value();
        if (style.trackProgressedPressedFillColor.has_value())
            progressedTrack.style.fillColor = style.trackProgressedPressedFillColor.value();
        if (style.trackProgressedPressedBorderColor.has_value())
            progressedTrack.style.borderColor = style.trackProgressedPressedBorderColor.value();
        if (style.thumbPressedFillColor.has_value())
            thumb.style.fillColor = style.thumbPressedFillColor.value();
        if (style.thumbPressedBorderColor.has_value())
            thumb.style.borderColor = style.thumbPressedBorderColor.value();
    }
    else if (isTrackHovered || isThumbHovered) {
        if (style.trackUnprogressedHoveredFillColor.has_value())
            unprogressedTrack.style.fillColor = style.trackUnprogressedHoveredFillColor.value();
        if (style.trackUnprogressedHoveredBorderColor.has_value())
            unprogressedTrack.style.borderColor = style.trackUnprogressedHoveredBorderColor.value();
        if (style.trackProgressedHoveredFillColor.has_value())
            progressedTrack.style.fillColor = style.trackProgressedHoveredFillColor.value();
        if (style.trackProgressedHoveredBorderColor.has_value())
            progressedTrack.style.borderColor = style.trackProgressedHoveredBorderColor.value();
        if (style.thumbHoveredFillColor.has_value())
            thumb.style.fillColor = style.thumbHoveredFillColor.value();
        if (style.thumbHoveredBorderColor.has_value())
            thumb.style.borderColor = style.thumbHoveredBorderColor.value();
    }
}


/**
 * @brief Compute the thumb properties of the slider component.
 */
SFUI::Void SFUI::Slider::computeThumb() {
    // Thumb Size //
    SFUI::Vector2f computedThumbSize = {15.0f, 15.0f};
    if (std::holds_alternative<SFUI::Float>(style.thumbWidth))
        computedThumbSize.x = std::get<SFUI::Float>(style.thumbWidth);
    else if (std::holds_alternative<SFUI::String>(style.thumbWidth)) {
        SFUI::String thumbWidthString = std::get<SFUI::String>(style.thumbWidth);
        if (thumbWidthString.size() > 1 && thumbWidthString.back() == '%') {
            thumbWidthString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempThumbWidth = std::stod(thumbWidthString, &index);
                if (index == thumbWidthString.size()) {
                    SFUI::Float relativeTrackWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedThumbSize.x = relativeTrackWidthFactor * std::clamp(static_cast<SFUI::Float>(tempThumbWidth) / 100.0f, 0.0f, 0.5f);
                }
            }   catch (...) {}
        }
    }
    if (std::holds_alternative<SFUI::Float>(style.thumbWidth))
        computedThumbSize.y = std::get<SFUI::Float>(style.thumbWidth);
    else if (std::holds_alternative<SFUI::String>(style.thumbWidth)) {
        SFUI::String thumbHeightString = std::get<SFUI::String>(style.thumbWidth);
        if (thumbHeightString.size() > 1 && thumbHeightString.back() == '%') {
            thumbHeightString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempThumbHeight = std::stod(thumbHeightString, &index);
                if (index == thumbHeightString.size()) {
                    SFUI::Float relativeTrackWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedThumbSize.y = relativeTrackWidthFactor * std::clamp(static_cast<SFUI::Float>(tempThumbHeight) / 100.0f, 0.0f, 0.5f);
                }
            }   catch (...) {}
        }
    }
    computedSliderStyle.thumbSize = computedThumbSize;

    // Thumb Layout //
    thumb.layout.width = computedThumbSize.x;
    thumb.layout.height = computedThumbSize.y;
    if (computedSliderStyle.trackAlign == "horizontal") {
        thumb.layout.xPosition = thumbUpdatePosition.x;
        thumb.layout.yPosition = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedThumbSize.y / 2.0f);
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        thumb.layout.xPosition = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedThumbSize.x / 2.0f);
        thumb.layout.yPosition = thumbUpdatePosition.y;
    }

    // Thumb Style //
    thumb.style.borderWidth = style.borderWidth;
    thumb.style.cornerRadius = style.thumbCornerRadius;
    thumb.style.focusWidth = style.focusWidth;
    thumb.style.focusOffset = style.focusOffset;
    thumb.style.focusCornerRadius = style.focusCornerRadius;
    thumb.style.focusFillColor = style.focusFillColor;
    thumb.style.toolTipPadding = style.toolTipPadding;
    thumb.style.toolTipCornerRadius = style.toolTipCornerRadius;
    thumb.style.toolTipText = style.toolTipText;
    thumb.style.toolTipFont = style.toolTipFont;
    thumb.style.toolTipTextSize = style.toolTipTextSize;
    thumb.style.toolTipFillColor = style.toolTipFillColor;
    thumb.style.toolTipTextColor = style.toolTipTextColor;

    // Thumb State //
    thumb.state.isDisabled = state.isDisabled;
    thumb.state.isFocused = state.isFocused;

    // Thumb Behavior //
    thumb.behavior.onEnable = [this](const SFUI::String& componentID) {
        if (behavior.onEnable) behavior.onEnable(componentID);
    };
    thumb.behavior.onDisable = [this](const SFUI::String& componentID) {
        if (behavior.onDisable) behavior.onDisable(componentID);
    };
    thumb.behavior.onFocus = [this](const SFUI::String& componentID) {
        if (behavior.onFocus) behavior.onFocus(componentID);
    };
    thumb.behavior.onBlur = [this](const SFUI::String& componentID) {
        if (behavior.onBlur) behavior.onBlur(componentID);
    };
    thumb.behavior.onHoverIn = [this](const SFUI::String& componentID) {
        if (behavior.onThumbHoverIn) behavior.onThumbHoverIn(componentID);
        isThumbHovered = true;
    };
    thumb.behavior.onHoverOut = [this](const SFUI::String& componentID) {
        if (behavior.onThumbHoverOut) behavior.onThumbHoverOut(componentID);
        isThumbHovered = false;
    };
    thumb.behavior.onLeftPressIn = [this](const SFUI::String& componentID) {
        if (behavior.onThumbLeftPressIn) behavior.onThumbLeftPressIn(componentID);
        if (behavior.onSlidingStart) behavior.onSlidingStart(componentID, state.value);
        isThumbLeftPressed = true;
        isSliding = true;
        state.isFocused = false;
    };
    thumb.behavior.onLeftPress = [this](const SFUI::String& componentID) {
        if (behavior.onThumbLeftPress) behavior.onThumbLeftPress(componentID);
        if (behavior.onSlidingEnd) behavior.onSlidingEnd(componentID, state.value);
        isThumbLeftPressed = false;
        isSliding = false;
    };
    thumb.behavior.onRightPressIn = [this](const SFUI::String& componentID) {
        if (behavior.onThumbRightPressIn) behavior.onThumbRightPressIn(componentID);
    };
    thumb.behavior.onRightPress = [this](const SFUI::String& componentID) {
        if (behavior.onThumbRightPress) behavior.onThumbRightPress(componentID);
    };
    thumb.behavior.onMiddlePressIn = [this](const SFUI::String& componentID) {
        if (behavior.onThumbMiddlePressIn) behavior.onThumbMiddlePressIn(componentID);
    };
    thumb.behavior.onMiddlePress = [this](const SFUI::String& componentID) {
        if (behavior.onThumbMiddlePress) behavior.onThumbMiddlePress(componentID);
    };
    thumb.behavior.onKeyPress = [this](SFUI::String componentID, sf::Keyboard::Key pressedKey) {
        if (behavior.onKeyPress) behavior.onKeyPress(componentID, pressedKey);
    };

    // Update //
    thumb.update(renderTargetSize);
}


/**
 * @brief Compute the thumb properties of the slider component.
 */
SFUI::Void SFUI::Slider::computeTracks() {
    // Master Track Width //
    SFUI::Float computedTrackWidth = 7.0f;
    if (std::holds_alternative<SFUI::Float>(style.trackWidth))
        computedTrackWidth = std::get<SFUI::Float>(style.trackWidth);
    else if (std::holds_alternative<SFUI::String>(style.trackWidth)) {
        SFUI::String trackWidthString = std::get<SFUI::String>(style.trackWidth);
        if (trackWidthString.size() > 1 && trackWidthString.back() == '%') {
            trackWidthString.pop_back();
            try {
                size_t index = 0;
                SFUI::Double tempTrackWidth = std::stod(trackWidthString, &index);
                if (index == trackWidthString.size()) {
                    SFUI::Float relativeTrackWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                    computedTrackWidth = relativeTrackWidthFactor * std::clamp(static_cast<SFUI::Float>(tempTrackWidth) / 100.0f, 0.0f, 0.5f);
                }
            }   catch (...) {}
        }
    }

    // Progressed Track Width //
    SFUI::Float computedProgressedTrackWidth = computedTrackWidth;
    if (style.trackProgressedWidth.has_value()) {
        SFUI::Prop::Dimension trackProgressedWidth = style.trackProgressedWidth.value();
        if (std::holds_alternative<SFUI::Float>(trackProgressedWidth))
            computedTrackWidth = std::get<SFUI::Float>(trackProgressedWidth);
        else if (std::holds_alternative<SFUI::String>(trackProgressedWidth)) {
            SFUI::String trackWidthString = std::get<SFUI::String>(trackProgressedWidth);
            if (trackWidthString.size() > 1 && trackWidthString.back() == '%') {
                trackWidthString.pop_back();
                try {
                    size_t index = 0;
                    SFUI::Double tempTrackWidth = std::stod(trackWidthString, &index);
                    if (index == trackWidthString.size()) {
                        SFUI::Float relativeTrackWidthFactor = std::min(computedLayout.size.x, computedLayout.size.y);
                        computedTrackWidth = relativeTrackWidthFactor * std::clamp(static_cast<SFUI::Float>(tempTrackWidth) / 100.0f, 0.0f, 0.5f);
                    }
                }   catch (...) {}
            }
        }
    }

    // Track Size and Positions //
    SFUI::Vector2i thumbPosition = {static_cast<SFUI::Int>(thumbUpdatePosition.x), static_cast<SFUI::Int>(thumbUpdatePosition.y)};
    SFUI::Vector2f thumbCenterPosition = {
        static_cast<SFUI::Float>(thumbPosition.x) + (computedSliderStyle.thumbSize.x / 2.0f),
        static_cast<SFUI::Float>(thumbPosition.y) + (computedSliderStyle.thumbSize.y / 2.0f)
    };
    if (computedSliderStyle.trackAlign == "horizontal") {
        unprogressedTrack.layout.width = computedLayout.size.x;
        unprogressedTrack.layout.height = computedTrackWidth;
        unprogressedTrack.layout.xPosition = computedLayout.position.x;
        unprogressedTrack.layout.yPosition = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedTrackWidth / 2.0f);
        progressedTrack.layout.height = computedProgressedTrackWidth;
        progressedTrack.layout.yPosition = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedProgressedTrackWidth / 2.0f);
        if (!state.isInverted) {
            progressedTrack.layout.xPosition = computedLayout.position.x;
            progressedTrack.layout.width = thumbCenterPosition.x - computedLayout.position.x;
        }
        else if (state.isInverted) {
            progressedTrack.layout.xPosition = thumbCenterPosition.x + 1;
            progressedTrack.layout.width = (computedLayout.position.x + computedLayout.size.x) - thumbCenterPosition.x;
        }
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        unprogressedTrack.layout.width = computedTrackWidth;
        unprogressedTrack.layout.height = computedLayout.size.y;
        unprogressedTrack.layout.xPosition = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedTrackWidth / 2.0f);
        unprogressedTrack.layout.yPosition = computedLayout.position.y;
        progressedTrack.layout.width = computedProgressedTrackWidth;
        progressedTrack.layout.xPosition = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedProgressedTrackWidth / 2.0f);
        if (!state.isInverted) {
            progressedTrack.layout.yPosition = thumbCenterPosition.y + 1;
            progressedTrack.layout.height = (computedLayout.position.y + computedLayout.size.y) - thumbCenterPosition.y;
        }
        else if (state.isInverted) {
            progressedTrack.layout.yPosition = computedLayout.position.y;
            progressedTrack.layout.height = thumbCenterPosition.y - computedLayout.position.y;
        }
    }

    // Track Style //
    unprogressedTrack.style.cornerRadius = style.trackCornerRadius;
    unprogressedTrack.style.shadowOffset = style.shadowOffset;
    unprogressedTrack.style.shadowRadius = style.shadowRadius;
    unprogressedTrack.style.shadowFillColor = style.shadowFillColor;
    progressedTrack.style.cornerRadius = style.trackCornerRadius;

    // Update //
    unprogressedTrack.update(renderTargetSize);
    progressedTrack.update(renderTargetSize);
}


/**
 * @brief Check if the track is being hovered by the mouse.
 *
 * @param mousePosition The current mouse position.
 *
 * @return True if the track is being hovered, false otherwise.
 */
SFUI::Bool SFUI::Slider::trackHovered(SFUI::Vector2i mousePosition) {
    return (
        mousePosition.x >= unprogressedTrack.getPosition().x && mousePosition.x <= (unprogressedTrack.getPosition().x + unprogressedTrack.getSize().x) &&
        mousePosition.y >= unprogressedTrack.getPosition().y && mousePosition.y <= (unprogressedTrack.getPosition().y + unprogressedTrack.getSize().y)
    );
}


/**
 * @brief Handle thumb movement and update the slider value accordingly.
 *
 * @param mousePosition The current mouse position.
 */
SFUI::Void SFUI::Slider::handleThumbMove(SFUI::Vector2i mousePosition) {
    SFUI::Float oldValue = state.value;

    // Track Thumb Moving and New Value //
    SFUI::Float mouseClampDirection = 0.0f;
    if (computedSliderStyle.trackAlign == "horizontal")
        mouseClampDirection = mousePosition.x;
    else if (computedSliderStyle.trackAlign == "vertical")
        mouseClampDirection = mousePosition.y;

    SFUI::Float clampedMouse = 0.0f;
    if (computedSliderStyle.trackAlign == "horizontal") {
        if (!state.isInverted)
            clampedMouse = std::clamp(mouseClampDirection, intervalPositions.front(), intervalPositions.back());
        else if (state.isInverted)
            clampedMouse = std::clamp(mouseClampDirection, intervalPositions.back(), intervalPositions.front());
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        if (!state.isInverted)
            clampedMouse = std::clamp(mouseClampDirection, intervalPositions.back(), intervalPositions.front());
        else if (state.isInverted)
            clampedMouse = std::clamp(mouseClampDirection, intervalPositions.front(), intervalPositions.back());
    }



    // Find Nearest Interval Index //
    SFUI::UnsignedInt nearestIndex = 0u;
    SFUI::Float smallestDistance = std::numeric_limits<SFUI::Float>::max();

    for (SFUI::UnsignedInt i = 0; i < intervalPositions.size(); ++i) {
        SFUI::Float distance = std::abs(intervalPositions[i] - clampedMouse);
        if (distance < smallestDistance) {
            smallestDistance = distance;
            nearestIndex = i;
        }
    }

    SFUI::Float newValue = intervalValues.at(nearestIndex);
    if (oldValue != newValue && behavior.onValueChange) behavior.onValueChange(componentID, newValue);

    // Update Slider Value and Thumb Position //
    state.value = newValue;
    if (computedSliderStyle.trackAlign == "horizontal") {
        thumbUpdatePosition.x = intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.x / 2.0f);
        thumbUpdatePosition.y = computedLayout.position.y + (computedLayout.size.y / 2.0f) - (computedSliderStyle.thumbSize.y / 2.0f);
    }
    else if (computedSliderStyle.trackAlign == "vertical") {
        thumbUpdatePosition.x = computedLayout.position.x + (computedLayout.size.x / 2.0f) - (computedSliderStyle.thumbSize.x / 2.0f);
        thumbUpdatePosition.y = intervalPositions[nearestIndex] - (computedSliderStyle.thumbSize.y / 2.0f);
    }
}
