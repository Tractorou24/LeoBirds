export module Levels.Level;

import <SFML/System/Vector2.hpp>;
import <SFML/Window/Event.hpp>;

import <memory>;
import <sstream>;
import <string>;
import <vector>;

import Sling;
import FLib.Application;
import FLib.Scene;
import FLib.Layer;
import FLib.DrawableImage;

import Engine.Entity;
import Trajectory;
import Projectile;
import Boat;
import Gun;

namespace birds::levels
{
    /**
     * \brief The game level
     */
    export class Level final : public Entity
    {
    public:
        /**
         * @brief Creates a level without loading it from a save
         */
        explicit Level();

        /**
         * \brief Creates a level from a memory buffer
         * \param memory_buffer The memory buffer to load the level from
         */
        explicit Level(std::istringstream memory_buffer);
        ~Level() override = default;

        /**
        * \copydoc Level::update
        */
        void update(float dt) override;

        /**
         * @brief A method used to handle events
         * @param event The event to handle
         */
        void onEvent(const sf::Event& event);

        /**
         * \return The scene used to display the level
         */
        std::shared_ptr<flib::Scene> scene() { return m_scene; }

    private:
        bool m_isTrajectoryShown = false;

        std::shared_ptr<flib::Scene> m_scene;
        std::shared_ptr<flib::Layer> m_backgroundLayer, m_gunLayer, m_boatLayer;

        std::shared_ptr<Gun> m_gun;
        std::shared_ptr<Boat> m_boat;
        std::shared_ptr<Trajectory> m_trajectory;
        std::vector<std::shared_ptr<Projectile>> m_bullets;
    };
}

module: private;

namespace birds::levels
{
    Level::Level()
        : m_scene(std::make_shared<flib::Scene>()), m_backgroundLayer(std::make_shared<flib::Layer>()),
          m_gunLayer(std::make_shared<flib::Layer>()), m_boatLayer(std::make_shared<flib::Layer>()),
          m_gun(std::make_shared<Gun>(sf::Vector2f(100, 500))), m_boat(std::make_shared<Boat>(sf::Vector2f(1024, 400), 250))
    {
        // Ensure gun is initialized before trajectory, so it's not nullptr
        m_trajectory = std::make_shared<Trajectory>(*m_gun);

        m_scene->addLayer("gun", m_gunLayer);
        m_scene->addLayer("boat", m_boatLayer);
        m_scene->addLayer("background", m_backgroundLayer);

        m_scene->onEvent.connect(sling::Slot<sf::Event>([this](const sf::Event& event) { this->onEvent(event); }));

        m_backgroundLayer->addDrawable(std::make_shared<flib::DrawableImage>("Assets/background.png"));
        m_gunLayer->addDrawable(m_gun);
        m_boatLayer->addDrawable(m_boat);
    }

    Level::Level(std::istringstream)
    {
        // TODO: Implement loading from memory stream (save file)
        throw std::logic_error("Not implemented");
    }

    void Level::update(const float dt)
    {
        if (m_isTrajectoryShown)
            m_trajectory->calculate(m_gun->rotation());

        // Remove bullets that are in the ground
        std::erase_if(m_bullets, [&](const std::shared_ptr<Projectile>& bullet)
        {
            if (bullet->position().y > 550 - bullet->globalBounds().height)
            {
                m_gunLayer->removeDrawable(bullet);
                return true;
            }
            return false;
        });

        // TODO: Handle multiple boats
        if (!m_boatLayer->getDrawables().empty())
        {
            m_boat->update(dt);
            for (const auto& bullet : m_bullets)
            {
                if (bullet->globalBounds().intersects(m_boat->globalBounds()))
                {
                    m_gunLayer->removeDrawable(bullet);
                    std::erase(m_bullets, bullet);
                    m_boatLayer->removeDrawable(m_boat);
                }
            }
        }

        // Update bullets
        std::ranges::for_each(m_bullets, [&dt](const std::shared_ptr<Projectile>& bullet) { bullet->update(dt); });
    }

    void Level::onEvent(const sf::Event& event)
    {
        switch (event.type)
        {
        case sf::Event::EventType::MouseMoved:
            m_gun->rotate(static_cast<sf::Vector2f>(sf::Mouse::getPosition()));
            break;
        case sf::Event::EventType::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Button::Right)
            {
                m_gunLayer->addDrawable(m_trajectory);
                m_isTrajectoryShown = true;
            }
            if (event.mouseButton.button == sf::Mouse::Button::Left)
            {
                m_bullets.push_back(std::move(m_gun->shoot(550)));
                m_gunLayer->addDrawable(m_bullets.back());
            }
            break;
        case sf::Event::EventType::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Button::Right)
            {
                m_gunLayer->removeDrawable(m_trajectory);
                m_isTrajectoryShown = false;
            }
            break;
        default:
            break;
        }
    }
}
