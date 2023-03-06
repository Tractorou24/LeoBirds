export module Boat;

import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderTarget.hpp>;

import <memory>;

import FLib.DrawableImage;

import Engine.Entity;
import Characteristics;
import CharacteristicsFactory;

namespace birds
{
    /**
     * @brief A class that represents a boat.
     */
    export class Boat final : public sf::Drawable, public Entity
    {
    public:
        /**
         * @brief Instantiates a new boat.
         * @param position The position of the boat.
         */
        explicit Boat(sf::Vector2f position);
        ~Boat() override = default;

        /**
         * @return The current position of the boat.
         */
        sf::Vector2f position() const { return m_image.position(); }

        /**
         * @return The current bounds of the boat.
         */
        sf::FloatRect globalBounds() const { return m_image.globalBounds(); }


        /**
         * @brief Sets the position of the boat.
         * @param position The new position.
         */
        void setPosition(sf::Vector2f position) { m_image.setPosition(std::move(position)); }

        /**
         * @brief Sets the scale of the boat.
         * @param scale The new scale.
         */
        void setScale(sf::Vector2f scale) { m_image.setScale(std::move(scale)); }


        /**
         * @copydoc Entity::update 
         */
        void update(float dt) override;

        /**
         * @copydoc sf::Drawable::draw
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        flib::DrawableImage m_image;
        characteristics::BoatCharacteristics m_characteristics;
    };
}

module: private;

namespace birds
{
    Boat::Boat(sf::Vector2f position)
        : m_image("Assets/boat.png")
    {
        m_image.setPosition(std::move(position));

        m_characteristics.Speed = 50;
        m_characteristics.Health = 100;
        m_characteristics.FireRate = 60;
    }

    void Boat::update(const float dt)
    {
        m_image.setPosition({m_image.position().x - m_characteristics.Speed * dt, m_image.position().y});
    }

    void Boat::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        target.draw(m_image, states);
    }
}
