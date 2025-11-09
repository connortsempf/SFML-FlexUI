#pragma once
#include "Base/Component.hpp"




///////////////////////////////
// Container Component Class //
///////////////////////////////

namespace SFUI {

    class Container : public Component {

        public:
            Container() = default;
            Container(SFUI::String componentID);
            Container(SFUI::String componentID, SFUI::PropGroup::Container containerPropGroup);
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
    };
}
