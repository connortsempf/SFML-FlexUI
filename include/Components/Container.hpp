#pragma once
#include "Base/Component.hpp"




///////////////////////////////
// Container Component Class //
///////////////////////////////

namespace SFUI {

    class Container : public Component {

        public:
            Container() = default;
            Container(Container&&) = default;
            Container& operator=(Container&&) = default;
            Container(SFUI::String componentID);
            template<typename... Children>
            Container(SFUI::String componentID, SFUI::PropGroup::Container containerPropGroup, Children&&... children) :
                Component(std::move(componentID), std::move(containerPropGroup.layout), std::move(containerPropGroup.style)) {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);

            }
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
    };
}
