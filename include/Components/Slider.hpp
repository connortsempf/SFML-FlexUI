#pragma once
#include "Base/Component.hpp"
#include "Container.hpp"
#include "Button.hpp"




////////////////////////////
// Slider Component Class //
////////////////////////////

namespace SFUI {
    
    class Slider : public Component {

        public:
            SFUI::Prop::Style::Slider sliderStyle;
            SFUI::Prop::State::Slider sliderState;
            SFUI::Prop::Behavior::Slider sliderBehavior;

        public:
            Slider() = default;
            Slider(const SFUI::String& componentID);
            Slider(const SFUI::String& componentID, const SFUI::PropGroup::Slider& sliderPropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);

        private:
            SFUI::Bool isTrackHovered = false;
            SFUI::Bool isThumbHovered = false;
            SFUI::Bool isTrackLeftPressed = false;
            SFUI::Bool isTrackRightPressed = false;
            SFUI::Bool isTrackMiddlePressed = false;
            SFUI::Bool isThumbLeftPressed = false;
            SFUI::Bool isThumbRightPressed = false;
            SFUI::Bool isThumbMiddlePressed = false;
            SFUI::Bool isSliding = false;
            SFUI::Vector<SFUI::Float> intervalValues;
            SFUI::Vector<SFUI::Float> intervalPositions;
            SFUI::Vector2f thumbUpdatePosition = {0.0f, 0.0f};
            SFUI::Container unprogressedTrack;
            SFUI::Container progressedTrack;
            SFUI::Button thumb;
            SFUI::Prop::Style::Slider dirtySliderStyle;
            SFUI::Prop::State::Slider dirtySliderState;
            SFUI::ComputedProp::Style::Slider computedSliderStyle;
        
        private:
            SFUI::Void computeTrackAlign();
            SFUI::Void computeValueDynamics();
            SFUI::Void computeDynamicColors();
            SFUI::Void computeThumb();
            SFUI::Void computeTracks();
            SFUI::Bool trackHovered(SFUI::Vector2i mousePosition);
            SFUI::Void handleThumbMove(SFUI::Vector2i mousePosition);
    };
}
