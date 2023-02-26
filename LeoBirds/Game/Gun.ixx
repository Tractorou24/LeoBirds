export module Gun;

import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderTarget.hpp>;

import <memory>;
import <string>;
import <valarray>;

import FLib.DrawableImage;

namespace birds
{
    export class Gun final : public sf::Drawable
    {
    public:
        /**
         * \brief Creates a new drawable physics object
         * \param initial_position The initial position of the object
         */
        explicit Gun(sf::Vector2f initial_position);

        ~Gun() override = default;

        void rotate(const sf::Vector2f& mouse_position);

        /**
         * \brief Draws the Gun into the target
         * \param target The target to draw the Gun into
         * \param states The states used to render the GunS
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        flib::DrawableImage m_cannon;
        flib::DrawableImage m_cannonBase;
    };
}

module: private;

namespace birds
{
    Gun::Gun(const sf::Vector2f initial_position)
        : m_cannon("Assets/cannon.png"), m_cannonBase("Assets/cannon_base.png")
    {
        m_cannonBase.setPosition(initial_position);

        m_cannon.setOrigin({0, m_cannon.globalBounds().height});
        m_cannon.setPosition({initial_position.x + 62, initial_position.y + 20});
    }

    void Gun::rotate(const sf::Vector2f& mouse_position)
    {
        // Calculate angle
        const auto angle = std::atan2(mouse_position.y - m_cannon.position().y,
                                      mouse_position.x - m_cannon.position().x) *
                           180.f / 3.14159265358979323846f;

        // Make sure angle is possible and rotate gun
        if (angle > -20 && angle < 20)
            m_cannon.setRotation(angle);
    }

    void Gun::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        target.draw(m_cannonBase, states);
        target.draw(m_cannon, states);
    }
}
