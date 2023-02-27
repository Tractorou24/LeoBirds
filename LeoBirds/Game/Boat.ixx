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
    export class Boat final : public sf::Drawable, public Entity
    {
    public:
        explicit Boat(sf::Vector2f position, std::size_t beach_x_position);
        ~Boat() override = default;

        sf::Vector2f position() const { return m_image.position(); }
        sf::FloatRect globalBounds() const { return m_image.globalBounds(); }

        void setPosition(sf::Vector2f position) { m_image.setPosition(std::move(position)); }
        void setScale(sf::Vector2f scale) { m_image.setScale(std::move(scale)); }

        void update(float dt) override;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        std::size_t m_beachXPosition;
        flib::DrawableImage m_image;
        characteristics::BoatCharacteristics m_characteristics;
    };
}

module: private;

namespace birds
{
    Boat::Boat(sf::Vector2f position, std::size_t beach_x_position)
        : m_beachXPosition(std::move(beach_x_position)), m_image("Assets/boat.png")
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
