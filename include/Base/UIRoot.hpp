#pragma once
#include "Types/base.hpp"
#include "Types/render.hpp"
#include "Types/vector.hpp"
#include "Component.hpp"
#include <deque>




///////////////////////////////////////
// SFML-FlexUI Root UI Manager Class //
///////////////////////////////////////

namespace SFUI {

    class Component;

    class UIRoot {

        public:
            UIRoot() = default;
            UIRoot(SFUI::UniquePointer<SFUI::Component> rootComponent);
            SFUI::Void setRootComponent(SFUI::UniquePointer<SFUI::Component> rootComponent);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);

        private:
            SFUI::UniquePointer<SFUI::Component> rootComponent;

            double count = 0.0;
            double accumulator = 0.0;

        
        private:
            SFUI::Void drawRecursive(const SFUI::UniquePointer<SFUI::Component>& component, SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
    };
}
