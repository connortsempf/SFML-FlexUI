/**
 * @file Animation.cpp
 * @brief Implements the Animation class.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.2
 *
 * This file implements the class functions for the SFUI Animation class.
 */


#include "Base/Animation.hpp"


/**
 * @brief Easing functions mapping normalized elapsed time to normalized progress.
 */
const SFUI::UnorderedMap<SFUI::Animation::EasingFunction, SFUI::Function<SFUI::Float(SFUI::Float)>> easingFunctions = {
    {SFUI::Animation::EasingFunction::LINEAR, [](SFUI::Float normalizedElapsedTime) {
        return normalizedElapsedTime;
    }},
    {SFUI::Animation::EasingFunction::EASE_IN_QUAD, [](SFUI::Float normalizedElapsedTime) {
        return normalizedElapsedTime * normalizedElapsedTime;
    }},
    {SFUI::Animation::EasingFunction::EASE_OUT_QUAD, [](SFUI::Float normalizedElapsedTime) {
        return 1 - (1 - normalizedElapsedTime) * (1 - normalizedElapsedTime);
    }},
    {SFUI::Animation::EasingFunction::EASE_IN_OUT_QUAD, [](SFUI::Float normalizedElapsedTime) {
        return normalizedElapsedTime < 0.5f ? 2 * normalizedElapsedTime * normalizedElapsedTime : 1 - std::pow(-2 * normalizedElapsedTime + 2, 2) / 2;
    }},
    {SFUI::Animation::EasingFunction::EASE_IN_CUBIC, [](SFUI::Float normalizedElapsedTime) {
        return normalizedElapsedTime * normalizedElapsedTime * normalizedElapsedTime;
    }},
    {SFUI::Animation::EasingFunction::EASE_OUT_CUBIC, [](SFUI::Float normalizedElapsedTime) {
        return 1 - std::pow(1 - normalizedElapsedTime, 3);
    }},
    {SFUI::Animation::EasingFunction::EASE_IN_OUT_CUBIC, [](SFUI::Float normalizedElapsedTime) {
        return normalizedElapsedTime < 0.5f ? 4 * normalizedElapsedTime * normalizedElapsedTime * normalizedElapsedTime : 1 - std::pow(-2 * normalizedElapsedTime + 2, 3) / 2;
    }},
    {SFUI::Animation::EasingFunction::EASE_IN_SINE, [](SFUI::Float normalizedElapsedTime) {
        return 1 - std::cos((normalizedElapsedTime * M_PI) / 2);
    }},
    {SFUI::Animation::EasingFunction::EASE_OUT_SINE, [](SFUI::Float normalizedElapsedTime) {
        return std::sin((normalizedElapsedTime * M_PI) / 2);
    }},
    {SFUI::Animation::EasingFunction::EASE_IN_OUT_SINE, [](SFUI::Float normalizedElapsedTime) {
        return -(std::cos(M_PI * normalizedElapsedTime) - 1) / 2;
    }},
    {SFUI::Animation::EasingFunction::EASE_OUT_BOUNCE, [](SFUI::Float normalizedElapsedTime) {
        const SFUI::Float n1 = 7.5625f;
        const SFUI::Float d1 = 2.75f;
        if (normalizedElapsedTime < 1 / d1) return n1 * normalizedElapsedTime * normalizedElapsedTime;
        else if (normalizedElapsedTime < 2 / d1) return n1 * (normalizedElapsedTime -= 1.5f / d1) * normalizedElapsedTime + 0.75f;
        else if (normalizedElapsedTime < 2.5 / d1) return n1 * (normalizedElapsedTime -= 2.25f / d1) * normalizedElapsedTime + 0.9375f;
        else return n1 * (normalizedElapsedTime -= 2.625f / d1) * normalizedElapsedTime + 0.984375f;
    }},
    {SFUI::Animation::EasingFunction::EASE_OUT_BACK, [](SFUI::Float normalizedElapsedTime) {
        const SFUI::Float c1 = 1.70158f;
        const SFUI::Float c3 = c1 + 1.f;
        return 1 + c3 * std::pow(normalizedElapsedTime - 1, 3) + c1 * std::pow(normalizedElapsedTime - 1, 2);
    }}
};


/**
 * @brief The constructor for the Animation class.
 *
 * @param animationConfig The configuration for the animation.
 */
SFUI::Animation::Animation(SFUI::Animation::Config animationConfig) :
    isAnimating(false),
    isAnimationComplete(false),
    startValue(animationConfig.startValue),
    endValue(animationConfig.endValue),
    currentValue(animationConfig.startValue),
    delayStartTime(animationConfig.delayStartTime),
    loopCount(animationConfig.loopCount),
    currentLoop(0),
    animationType(animationConfig.animationType)
{
    if (animationType == SFUI::Animation::Type::TIMING) {
        if (!std::holds_alternative<SFUI::Animation::TimingConfig>(animationConfig.animationTypeConfig)) {}
        else {
            timingConfig = std::get<SFUI::Animation::TimingConfig>(animationConfig.animationTypeConfig);
        }
    }
    else if (animationType == SFUI::Animation::Type::SPRING) {
        if (!std::holds_alternative<SFUI::Animation::SpringConfig>(animationConfig.animationTypeConfig)) {}
        else {
            springConfig = std::get<SFUI::Animation::SpringConfig>(animationConfig.animationTypeConfig);
            springConfig.velocity = springConfig.startVelocity;
        }
    }
    else if (animationType == SFUI::Animation::Type::DECAY) {
        if (!std::holds_alternative<SFUI::Animation::DecayConfig>(animationConfig.animationTypeConfig)) {}
        else {
            decayConfig = std::get<SFUI::Animation::DecayConfig>(animationConfig.animationTypeConfig);
            decayConfig.velocity = decayConfig.startVelocity;
        }
    }
}


/**
 * @brief Start the animation process.
 */
SFUI::Void SFUI::Animation::start() {
    totalFramesClock.restart();
    previousFrameClock.restart();
}


/**
 * @brief Pause the animation process.
 */
SFUI::Void SFUI::Animation::pause() {
    totalFramesClock.stop();
    previousFrameClock.stop();
}


/**
 * @brief Terminate the animation process.
 */
SFUI::Void SFUI::Animation::terminate() {
    isAnimating = false;
    isAnimationComplete = true;
    totalFramesClock.stop();
    previousFrameClock.stop();
}


/**
 * @brief Update the animation state and internal animation values.
 */
SFUI::Void SFUI::Animation::update() {
    if (totalFramesClock.getElapsedTime().asSeconds() < static_cast<int32_t>(delayStartTime)) return;

    // Perform Animation Updates Here to Compute New Intermediate Animation Values //
    if (animationType == SFUI::Animation::Type::TIMING) {
        updateTimingAnimation();
        if (currentValue == endValue) {
            if (++currentLoop >= loopCount) {
                terminate();
                return;
            }
            totalFramesClock.restart();
        }
    }
    else if (animationType == SFUI::Animation::Type::SPRING) {
        updateSpringAnimation();
        if (currentValue == endValue) {
            if (++currentLoop >= loopCount) {
                terminate();
                return;
            }
            springConfig.velocity = springConfig.startVelocity;
            totalFramesClock.restart();
        }
    }
    else if (animationType == SFUI::Animation::Type::DECAY) {
        updateDecayAnimation();
        if (decayConfig.velocity == 0.0f) {
            if (++currentLoop >= loopCount) {
                terminate();
                return;
            }
            decayConfig.velocity = decayConfig.startVelocity;
            totalFramesClock.restart();
        }
    }
    previousFrameClock.restart();
}


/**
 * @brief Get the updated value during the active animation process.
 */
SFUI::Float SFUI::Animation::getValue() const {
    return currentValue;
}


/**
 * @brief Get the current active state of the animation.
 *
 * @return True if currently animating and running, false if not.
 */
SFUI::Bool SFUI::Animation::getIsAnimating() const {
    return isAnimating;
}


/**
 * @brief Get the completion state of the animation.
 *
 * @return True if complete or terminated, false if currently animating or paused or not started.
 */
SFUI::Bool SFUI::Animation::getIsAnimationComplete() const {
    return isAnimationComplete;
}


/**
 * @brief Update the current time-animated value based.
 */
SFUI::Void SFUI::Animation::updateTimingAnimation() {
    SFUI::Float totalFramesElapsedTime = totalFramesClock.getElapsedTime().asSeconds();
    if (totalFramesElapsedTime < delayStartTime) return;

    // Easing-Timed Animation Progression //
    SFUI::Float normalizedElapsedTime = (totalFramesElapsedTime - delayStartTime) / timingConfig.duration;
    normalizedElapsedTime = std::clamp(normalizedElapsedTime, 0.f, 1.f);
    currentValue = startValue + (endValue - startValue) * (easingFunctions.at(timingConfig.easingFunction))(normalizedElapsedTime);

    // Animation Completion Check //
    if (normalizedElapsedTime >= 1.0f) currentValue = endValue;
}


/**
 * @brief Update the spring-animated value.
 */
SFUI::Void SFUI::Animation::updateSpringAnimation() {
    SFUI::Float totalFramesElapsedTime = totalFramesClock.getElapsedTime().asSeconds();
    if (totalFramesElapsedTime < delayStartTime) return;

    // Spring Force Animation Progression //
    SFUI::Float previousFrameElapsedTime = previousFrameClock.getElapsedTime().asSeconds();
    SFUI::Float displacement = currentValue - endValue;
    SFUI::Float force = (-springConfig.stiffness * displacement) - (springConfig.damping * springConfig.velocity);
    SFUI::Float acceleration = force / springConfig.mass;
    springConfig.velocity += acceleration * previousFrameElapsedTime;
    currentValue += springConfig.velocity * previousFrameElapsedTime;
    displacement = currentValue - endValue;

    // Animation Completion Check //
    if (std::abs(springConfig.velocity) < 0.0001f && std::abs(displacement) < 0.0001f) currentValue = endValue;
}


/**
 * @brief Update the decay-animated value.
 */
SFUI::Void SFUI::Animation::updateDecayAnimation() {
    SFUI::Float totalFramesElapsedTime = totalFramesClock.getElapsedTime().asSeconds();
    if (totalFramesElapsedTime < delayStartTime) return;

    // Exponential Decay Animation Progression //
    SFUI::Float previousFrameElapsedTime = previousFrameClock.getElapsedTime().asSeconds();
    decayConfig.velocity *= std::pow(decayConfig.deceleration, previousFrameElapsedTime * 60.0f);
    currentValue += decayConfig.velocity * previousFrameElapsedTime;

    // Animation Completion Check //
    if (std::abs(decayConfig.velocity) < 0.0001f) decayConfig.velocity = 0.0f;
}
