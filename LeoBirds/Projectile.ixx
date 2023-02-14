export module Projectile;

import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderTarget.hpp>;

import <memory>;
import <string>;

import FLib.DrawableImage;
import Engine.Physics;

namespace birds
{
    /**
     * \brief An object that can be seen on the scene and has physics.
     */
    export class Projectile final : public sf::Drawable, public birds::HasPhysics
    {
    public:
        /**
         * \brief Creates a new drawable physics object
         * \param initial_position The initial position of the object
         * \param image_path The path to the image to be used
         * \param ground_level The ground level in pixels
         */
        explicit Projectile(sf::Vector2f initial_position, std::string image_path, std::size_t ground_level);

        /**
         * \brief Creates a new drawable physics object
         * \param initial_position The initial position of the object
         * \param texture A shared ptr to the texture to be used
         * \param ground_level The ground level in pixels
         */
        explicit Projectile(sf::Vector2f initial_position, std::shared_ptr<sf::Texture> texture, std::size_t ground_level);
        ~Projectile() override = default;

        /**
         * \copydoc HasPhysics::update
         */
        void update(float dt) override;

        /**
         * \brief Draws the projectile into the target
         * \param target The target to draw the projectile into
         * \param states The states used to render the projectile
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        const std::size_t m_groundLevel;
        flib::DrawableImage m_image;
    };
}

module: private;

namespace birds
{
    Projectile::Projectile(const sf::Vector2f initial_position, std::string image_path, const std::size_t ground_level)
        : HasPhysics(initial_position), m_groundLevel(std::move(ground_level)), m_image(std::move(image_path)) { }

    Projectile::Projectile(const sf::Vector2f initial_position, std::shared_ptr<sf::Texture> texture, const std::size_t ground_level)
        : HasPhysics(initial_position), m_groundLevel(std::move(ground_level)), m_image(std::move(texture)) { }

    void Projectile::update(const float dt)
    {
        if (position().y > m_groundLevel - m_image.globalBounds().height)
            return;

        HasPhysics::update(dt);
        m_image.setPosition(position());
    }

    void Projectile::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        target.draw(m_image, states);
    }
}
