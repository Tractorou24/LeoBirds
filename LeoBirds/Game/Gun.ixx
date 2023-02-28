export module Gun;

import <SFML/Audio/Music.hpp>;
import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderTarget.hpp>;

import <memory>;
import <string>;
import <valarray>;

import FLib.DrawableImage;
import Projectile;

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

        /**
         * \return The rotation angle of the gun in degrees
         */
        float rotation() const { return m_angle; }

        /**
         * \return The position of the gun
         */
        sf::Vector2f position() const { return m_cannon.position(); }

        /**
         * \return The position of the muzzle
         */
        sf::Vector2f muzzlePosition() const
        {
            const float angle = m_angle * (3.14159265358979323846f / 180.0f);
            const float y = std::tan(angle) * 100 + m_cannon.position().y;
            const float x = m_cannon.position().x + m_angle;
            return sf::Vector2f(x + 100, y - 65);
        }


        /**
         * @brief Rotates the gun from the mouse direction
         * @param mouse_position The current mouse position
         */
        void rotate(const sf::Vector2f& mouse_position);

        std::shared_ptr<Projectile> shoot(std::size_t ground_level);

        /**
         * \brief Draws the Gun into the target
         * \param target The target to draw the Gun into
         * \param states The states used to render the GunS
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        float m_angle = 0.0f;
        sf::Music m_shootSound;
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

        m_shootSound.openFromFile("Assets/fire.ogg");
        m_shootSound.setVolume(100);
    }

    void Gun::rotate(const sf::Vector2f& mouse_position)
    {
        // Calculate angle
        const auto angle = std::atan2(mouse_position.y - m_cannon.position().y,
                                      mouse_position.x - m_cannon.position().x) *
                           180.f / 3.14159265358979323846f;

        // Make sure angle is possible and rotate gun
        if (angle > -20 && angle < 20)
        {
            m_angle = angle;
            m_cannon.setRotation(std::move(angle));
        }
    }

    std::shared_ptr<Projectile> Gun::shoot(std::size_t ground_level)
    {
        const auto projectile = std::make_shared<Projectile>(muzzlePosition(), "Assets/bullet.png", ground_level);
        projectile->addSpeed({
            std::cos(m_angle * (3.14159265358979323846f / 180.0f)) * 100,
            std::sin(m_angle * (3.14159265358979323846f / 180.0f)) * 100
        });

        m_shootSound.play();
        m_shootSound.setPlayingOffset(sf::seconds(0));
        return projectile;
    }

    void Gun::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        target.draw(m_cannonBase, states);
        target.draw(m_cannon, states);
    }
}
