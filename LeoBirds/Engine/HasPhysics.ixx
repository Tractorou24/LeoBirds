export module Engine.Physics;

import <SFML/Graphics/Drawable.hpp>;

import Engine.Entity;
import FLib.DrawableImage;

namespace birds
{
    /*
     * @brief Handles physics of an object in the scene
     *
     * Uses the following formulas to calculate the position of the object (taking only gravity in effect):
     * Acceleration:
     * ax = initial_acceleration_x
     * ay = -g + initial_acceleration_y
     *
     * Speed:
     * vx = initial_acceleration_x * t + initial_speed_x
     * vy = -g * t + initial_acceleration_y * t + initial_speed_y
     *
     * Position:
     * px = initial_acceleration_x * t^2/2 + initial_speed_x * t + initial_position_x
     * py = (-g * t^2)/2 + (initial_acceleration_y * t^2)/2 + initial_speed_y * t + initial_position_y
     */
    export class HasPhysics : public Entity
    {
    public:
        /**
         * \brief Creates a new physics object
         * \param initial_position The start position of the object
         */
        explicit HasPhysics(sf::Vector2f initial_position);
        ~HasPhysics() override = default;

        /**
         * \return the current position of the object
         */
        sf::Vector2f position() const noexcept { return m_position; }

        /**
         * \return the current speed of the object
         */
        sf::Vector2f speed() const noexcept { return m_speed; }


        /**
         * \brief Adds a speed to the object
         * \param speed The vector containing the speed relative to the axis x and y
         */
        void addSpeed(const sf::Vector2f& speed);

        /**
         * \copydoc Entity::update
         */
        void update(float dt) override;

    private:
        sf::Vector2f m_acceleration;
        sf::Vector2f m_speed;
        sf::Vector2f m_position;
    };
}

module: private;

namespace birds
{
    HasPhysics::HasPhysics(const sf::Vector2f initial_position)
        : m_position(std::move(initial_position)) { }

    void HasPhysics::addSpeed(const sf::Vector2f& speed)
    {
        m_speed += speed;
    }

    void HasPhysics::update(const float dt)
    {
        m_speed.x = m_acceleration.x * dt + m_speed.x;
        m_speed.y = 9.81f * dt + m_acceleration.y * dt + m_speed.y;

        m_position.x = (m_acceleration.x * dt * dt) / 2 + m_speed.x * dt + m_position.x;
        m_position.y = (9.81f * dt * dt) / 2 + (m_acceleration.y * dt * dt) / 2 + m_speed.y * dt + m_position.y;
    }
};
