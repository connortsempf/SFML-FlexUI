/**
 * @file Animation.hpp
 * @brief Defines the base Component class for all SFUI components.
 * @author Connor Sempf
 * @date 2025-12-04
 * @version 2.0.0
 *
 * This file contains the class definition, internal
 * state, and functions for the SFUI Animation class.
 */


#pragma once
#include "Types/base.hpp"
#include "Types/render.hpp"
#include <cmath>


namespace SFUI {

    /**
     * @brief Class blueprint for handling animations.
     */
    class Animation {

        public:
            /**
             * @brief The animation type.
             */
            enum class Type { TIMING, SPRING, DECAY };

            /**
             * @brief Types of easing functions for timing-based animations.
             */
            enum class EasingFunction {
                /**
                 * @brief Linear easing function.
                 */
                LINEAR,

                /**
                 * @brief Ease-in beginning with quadratic acceleration.
                 */
                EASE_IN_QUAD,

                /**
                 * @brief Ease-out ending with quadratic deceleration.
                 */
                EASE_OUT_QUAD,

                /**
                 * @brief Ease-in and ease-out with quadratic acceleration and deceleration.
                 */
                EASE_IN_OUT_QUAD,

                /**
                 * @brief Ease-in beginning with cubic acceleration.
                 */
                EASE_IN_CUBIC,

                /**
                 * @brief Ease-out ending with cubic deceleration.
                 */
                EASE_OUT_CUBIC,

                /**
                 * @brief Ease-in and ease-out with cubic acceleration and deceleration.
                 */
                EASE_IN_OUT_CUBIC,

                /**
                 * @brief Ease-in with sinusoidal velocity changes.
                 */
                EASE_IN_SINE,

                /**
                 * @brief Ease-out with sinusoidal velocity changes.
                 */
                EASE_OUT_SINE,

                /**
                 * @brief Ease-in and ease-out with sinusoidal velocity changes.
                 */
                EASE_IN_OUT_SINE,

                /**
                 * @brief Ease-out with bouncing ending effect.
                 */
                EASE_OUT_BOUNCE,

                /**
                 * @brief Ease-out with overshooting ending effect.
                 */
                EASE_OUT_BACK
            };

            /**
             * @brief The configuration for timing-based animations.
             */
            struct TimingConfig {
                /**
                 * @brief The duration for which the timing-based animation lasts.
                 */
                SFUI::Float duration = 0.5f;

                /**
                 * @brief The easing function to use for the timing-based animation.
                 */
                SFUI::Animation::EasingFunction easingFunction = SFUI::Animation::EasingFunction::LINEAR;
            };

            /**
             * @brief The configuration for spring-based animations.
             */
            struct SpringConfig {
                /**
                 * @brief The stiffness of the simulated spring.
                 */
                SFUI::Float stiffness = 100.0f;

                /**
                 * @brief The damping factor of the simulated spring.
                 */
                SFUI::Float damping = 10.0f;

                /**
                 * @brief The mass of the simulated spring.
                 */
                SFUI::Float mass = 1.0f;

                /**
                 * @brief The initial velocity of the mass of the simulated spring.
                 */
                SFUI::Float startVelocity = 0.0f;

                /**
                 * @brief The mid-animation velocity of the mass of the simulated spring.
                 */
                SFUI::Float velocity = 0.0f;
            };

            /**
             * @brief The configuration for decay-based animations.
             */
            struct DecayConfig {
                /**
                 * @brief The initial velocity of the simulated decay.
                 */
                SFUI::Float startVelocity = 80.0f;

                /**
                 * @brief The mid-animation velocity of the simulated decay.
                 */
                SFUI::Float velocity = 80.0f;

                /**
                 * @brief The deceleration factor of the simulated decay.
                 */
                SFUI::Float deceleration = 0.998f;
            };

            /**
             * @brief Animation configuration used as a user input when creating an animation.
             */
            struct Config {
                /**
                 * @brief The starting value from which to start the animation.
                 */
                SFUI::Float startValue;

                /**
                 * @brief The final value to approach with the animation.
                 */
                SFUI::Float endValue;

                /**
                 * @brief The time to wait after starting an animation to perform the actual animations.
                 */
                SFUI::Float delayStartTime = 0.0f;

                /**
                 * @brief The number of times to repeat the animation.
                 */
                SFUI::UnsignedInt loopCount = 1;

                /**
                 * @brief The animation type (timing, spring, decay).
                 */
                SFUI::Animation::Type animationType;

                /**
                 * @brief The config for the specified animation type.
                 */
                SFUI::Variant<SFUI::Animation::TimingConfig, SFUI::Animation::SpringConfig, SFUI::Animation::DecayConfig> animationTypeConfig;
            };

        public:
            Animation() = delete;

            /**
             * @brief The constructor for the Animation class.
             *
             * @param animationConfig The configuration for the animation.
             */
            Animation(SFUI::Animation::Config animationConfig);

            /**
             * @brief Start the animation process.
             */
            SFUI::Void start();

            /**
             * @brief Pause the animation process.
             */
            SFUI::Void pause();

            /**
             * @brief Terminate the animation process.
             */
            SFUI::Void terminate();

            /**
             * @brief Update the animation state and internal animation values.
             */
            SFUI::Void update();

            /**
             * @brief Get the updated value during the active animation process.
             */
            SFUI::Float getValue() const;

            /**
             * @brief Get the current active state of the animation.
             *
             * @return True if currently animating and running, false if not.
             */
            SFUI::Bool getIsAnimating() const;

            /**
             * @brief Get the completion state of the animation.
             *
             * @return True if complete or terminated, false if currently animating or paused or not started.
             */
            SFUI::Bool getIsAnimationComplete() const;

        private:
            /**
             * @brief Easing functions mapping normalized elapsed time to normalized progress.
             */
            static const SFUI::UnorderedMap<SFUI::Animation::EasingFunction, SFUI::Function<SFUI::Float(SFUI::Float)>> easingFunctions;

        private:
            /**
             * @brief Flag for if the animation is currently running.
             */
            SFUI::Bool isAnimating;

            /**
             * @brief Flag for if the animation is complete.
             */
            SFUI::Bool isAnimationComplete;

            /**
             * @brief Clock for tracking total animation duration between loops.
             */
            SFUI::Clock totalFramesClock;

            /**
             * @brief Clock for tracking time between individual frames.
             */
            SFUI::Clock previousFrameClock;

        private:
            /**
             * @brief The start value to begin the animation from.
             */
            SFUI::Float startValue;

            /**
             * @brief The end value to approach through the animation.
             */
            SFUI::Float endValue;

            /**
             * @brief The current animated value during the animation process.
             */
            SFUI::Float currentValue;

            /**
             * @brief Delay time before starting the animation after official user-decided start.
             */
            SFUI::Float delayStartTime;

            /**
             * @brief Number of times to loop the animation.
             */
            SFUI::UnsignedInt loopCount;

            /**
             * @brief Current loop iteration.
             */
            SFUI::UnsignedInt currentLoop;

            /**
             * @brief The animation type (timing, spring, decay).
             */
            SFUI::Animation::Type animationType;

            /**
             * @brief The configuration for timing-based animations.
             */
            SFUI::Animation::TimingConfig timingConfig;

            /**
             * @brief The configuration for spring-based animations.
             */
            SFUI::Animation::SpringConfig springConfig;

            /**
             * @brief The configuration for decay-based animations.
             */
            SFUI::Animation::DecayConfig decayConfig;

        private:
            /**
             * @brief Update the current time-animated value based.
             */
            SFUI::Void updateTimingAnimation();

            /**
             * @brief Update the spring-animated value.
             */
            SFUI::Void updateSpringAnimation();

            /**
             * @brief Update the decay-animated value.
             */
            SFUI::Void updateDecayAnimation();
    };
}
