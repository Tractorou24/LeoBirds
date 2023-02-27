export module Trajectory;

import <SFML/Graphics.hpp>;

import <array>;
import <iostream>;

import Engine.Entity;
import Gun;

namespace birds
{
    /**
     * @brief A class used to draw the trajectory of the bullet
     */
    export class Trajectory final : public sf::Drawable
    {
    public:
        explicit Trajectory(const Gun& gun);
        ~Trajectory() override = default;

        /**
         * @brief Calculates and stores the trajectory of the bullet
         * @param angle The current angle of the gun
         */
        void calculate(double angle);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        std::array<sf::CircleShape, 10> m_points;
        const Gun& m_gun;
    };
}

module: private;

namespace birds
{
    Trajectory::Trajectory(const Gun& gun)
        : m_gun(gun)
    {
        for (auto& circle : m_points)
        {
            circle.setRadius(5);
            circle.setFillColor(sf::Color::Red);
        }
    }

    void Trajectory::calculate(double angle)
    {
        angle = angle * (3.14159265358979323846 / 180.0) * -1;
        const auto initial_point = m_gun.muzzlePosition();

        for (std::size_t i = 0; i < m_points.size(); ++i)
        {
            const float x = i * 100.0f + 100;
            const float y = -static_cast<float>(x * std::tan(angle) - 9.807 * x * x / (
                                                    2 * 100 * 100 * std::cos(angle) * std::cos(angle)));
            m_points[i].setPosition(x + initial_point.x, initial_point.y + y);
        }
    }

    void Trajectory::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        for (auto& circle : m_points)
            target.draw(circle, states);
    }
}
