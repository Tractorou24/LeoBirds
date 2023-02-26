export module Boat;

import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderTarget.hpp>;

import <memory>;
import <stdexcept>;

import Characteristics;
import CharacteristicsFactory;

import FLib.DrawableImage;

namespace birds
{
    export class Boat final : public sf::Drawable
    {
    public:
        explicit Boat(sf::Vector2f position);
        ~Boat() override = default;

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
        m_image.setScale({0.4f, 0.4f});

        m_characteristics.Speed = 20;
        m_characteristics.Health = 100;
        m_characteristics.FireRate = 60;
    }

    void Boat::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        target.draw(m_image, states);
    }
}
