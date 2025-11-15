#pragma once
#include <SFML-FlexUI.hpp>


SFUI::PropGroup::Container rootContainerProp{
    .layout = {
        .alignDirection = "vertical",
        .alignPrimary = "start",
        .alignSecondary = "center",
        .width = "100%",
        .height = "100%",
        .margin = 2.0f
    },
    .style = {
        .fillColor = SFUI::Color(32, 32, 32)
    }
};

SFUI::PropGroup::Container midContainerProp{
    .layout = {
        .alignDirection = "vertical",
        .alignPrimary = "center",
        .alignSecondary = "center",
        .width = "100%",
        .height = "35%",
        .padding = 5.0f
    },
    .style = {
        .fillColor = SFUI::Vector3ui8{77, 74, 52}
    }
};

SFUI::PropGroup::Slider sliderProp{
    .layout = {
        .width = "100%",
        .height = "40%",
        .margin = 5.0f
    },
    .style = {
        .cornerRadius = 5.0f
    },
    .sliderStyle = {
        .thumbWidth = 14.0f,
        .thumbHeight = 14.0f,
        .trackUnprogressedFillColor = SFUI::Color(150, 150, 150),
        .trackProgressedFillColor = SFUI::Color(235, 83, 85, 150),
        .thumbFillColor = SFUI::Color(235, 83, 85),
    },
    .sliderState = {
        .value = 5.0f,
        .minimumValue = 0.0f,
        .maximumValue = 20.0f,
        .step = 1.0f,
    }
};

SFUI::PropGroup::ScrollContainer scrollContainerProp{
    .layout = {
        .alignDirection = "vertical",
        .alignPrimary = "start",
        .alignSecondary = "center",
        .width = "100%",
        .height = "auto",
        .padding = SFUI::SubProp::Vector4dim{3.0f, 3.0f, 0.0f, 0.0f}
    }
};

SFUI::PropGroup::Button scrollButtonProp1{
    .layout = {
        .alignPrimary = "center",
        .alignSecondary = "center",
        .width = "100%",
        .height = 70.0f,
        .margin = SFUI::SubProp::Vector4dim{0.0f, 0.0f, 1.0f, 1.0f}
    },
    .style = {
        .cornerRadius = 5.0f,
        .fillColor = SFUI::Color(50, 50, 50)
    },
    .buttonStyle = {
        .hoveredFillColor = SFUI::Color(60, 60, 60)
    }
};

SFUI::PropGroup::Button scrollButtonProp2{
    .layout = {
        .alignPrimary = "center",
        .alignSecondary = "center",
        .width = "100%",
        .height = 70.0f,
        .margin = SFUI::SubProp::Vector4dim{1.0f, 1.0f, 0.0f, 0.0f}
    },
    .style = {
        .cornerRadius = 5.0f,
        .fillColor = SFUI::Color(59, 59, 59)
    },
    .buttonStyle = {
        .hoveredFillColor = SFUI::Color(50, 50, 50),
        .pressedFillColor = SFUI::Color(40, 40, 40)
    }
};

SFUI::PropGroup::Toggle scrollToggleProp{
    .layout = {
        .alignPrimary = "center",
        .alignSecondary = "center",
        .width = "50%",
        .height = 70.0f,
        .margin = SFUI::SubProp::Vector4dim{1.0f, 1.0f, 0.0f, 0.0f}
    },
    .style = {
        .cornerRadius = 5.0f,
    },
    .toggleStyle = {
        .offFillColor = SFUI::Color(150, 150, 150),
        .onFillColor = SFUI::Color(203, 197, 138),
        .hoveredOffFillColor = SFUI::Color(130, 130, 130),
        .hoveredOnFillColor = SFUI::Color(150, 146, 102),
        .pressedOffFillColor = SFUI::Color(110, 110, 110),
        .pressedOnFillColor = SFUI::Color(125, 121, 85)
    }
};
