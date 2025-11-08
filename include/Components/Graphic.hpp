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
            Graphic(const SFUI::String& componentID);
            Graphic(const SFUI::String& componentID, const SFUI::PropGroup::Graphic& graphicPropGroup);
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
