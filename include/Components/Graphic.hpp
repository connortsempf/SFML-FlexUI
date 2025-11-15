#pragma once
#include "Base/Component.hpp"




/////////////////////////////
// Graphic Component Class //
/////////////////////////////

namespace SFUI {
    
    class Graphic : public Component {
        
        public:
            SFUI::Prop::Style::Graphic graphicStyle;
            SFUI::Prop::Behavior::Graphic graphicBehavior;

        public:
            Graphic() = default;
            Graphic(Graphic&&) = default;
            Graphic& operator=(Graphic&&) = default;
            Graphic(SFUI::String componentID);
            template<typename... Children>
            Graphic(SFUI::String componentID, SFUI::PropGroup::Graphic graphicPropGroup, Children&&... children) :
                Component(std::move(componentID), std::move(graphicPropGroup.layout), std::move(graphicPropGroup.style)),
                graphicStyle(std::move(graphicPropGroup.graphicStyle)),
                graphicBehavior(std::move(graphicPropGroup.graphicBehavior)),
                graphic(graphicSource) {
                (addChild(std::make_unique<std::decay_t<Children>>(std::forward<Children>(children))), ...);

            }
            SFUI::Void handleEvent(const SFUI::Event& event);
            SFUI::Void update(const SFUI::Vector2u renderTargetSize);
            SFUI::Void draw(SFUI::RenderTarget& drawTarget, SFUI::RenderWindow& window);
            SFUI::Vector2f getOriginalTextureSize();

        private:
            enum class LoadType { PRE_LOAD, SELF_LOAD };
            enum class LoadState { PRE_LOAD_UNLOADED, PRE_LOAD_LOADED, SELF_LOAD_UNLOADED, SELF_LOAD_LOADED, SELF_LOAD_ERROR };

        private:
            SFUI::Texture graphicSource;
            SFUI::Sprite graphic;
            SFUI::Graphic::LoadType loadType = SFUI::Graphic::LoadType::PRE_LOAD;
            SFUI::Graphic::LoadState loadState = SFUI::Graphic::LoadState::PRE_LOAD_UNLOADED;
            SFUI::Prop::Style::Graphic dirtyGraphicStyle;
            SFUI::ComputedProp::Style::Graphic computedGraphicStyle;
        
        private:
            SFUI::Void computeGraphicSource();
            SFUI::Void computeGraphicLayout();
    };
}
