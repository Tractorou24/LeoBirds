export module Levels.Level;

import <SFML/Audio/Music.hpp>;
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
import BoatManager;
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
        BoatManager m_boatManager;

        std::shared_ptr<flib::Scene> m_scene;
        std::shared_ptr<flib::Layer> m_backgroundLayer, m_gunLayer, m_boatLayer;

        std::shared_ptr<Gun> m_gun;
        std::shared_ptr<Trajectory> m_trajectory;
        std::vector<std::shared_ptr<Projectile>> m_bullets;

        sf::Music m_gameMusic, m_hitMusic;
    };
}

module: private;

namespace birds::levels
{
    Level::Level()
        : m_scene(std::make_shared<flib::Scene>()), m_backgroundLayer(std::make_shared<flib::Layer>()),
          m_gunLayer(std::make_shared<flib::Layer>()), m_gun(std::make_shared<Gun>(sf::Vector2f(100, 500)))
    {
        m_boatLayer = m_boatManager.layer();

        m_gameMusic.openFromFile("Assets/game_music.ogg");
        m_gameMusic.setVolume(100);
        m_gameMusic.setLoop(true);
        m_gameMusic.play();

        m_hitMusic.openFromFile("Assets/hit.ogg");
        m_hitMusic.setVolume(100);

        // Ensure gun is initialized before trajectory, so it's not nullptr
        m_trajectory = std::make_shared<Trajectory>(*m_gun);

        m_scene->addLayer("gun", m_gunLayer);
        m_scene->addLayer("boat", m_boatLayer);
        m_scene->addLayer("background", m_backgroundLayer);

        m_scene->onEvent.connect(sling::Slot<sf::Event>([this](const sf::Event& event) { this->onEvent(event); }));

        m_backgroundLayer->addDrawable(std::make_shared<flib::DrawableImage>("Assets/background.png"));
        m_gunLayer->addDrawable(m_gun);
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
            if (bullet->position().y > 600 - bullet->globalBounds().height)
            {
                m_gunLayer->removeDrawable(bullet);
                return true;
            }
            return false;
        });

        // Handle boats damage and destruction
        const auto projectiles_to_destroy = m_boatManager.checkDamage(m_bullets);
        std::ranges::for_each(projectiles_to_destroy, [&](const auto& idx)
        {
            m_hitMusic.play();
            m_hitMusic.setPlayingOffset(sf::seconds(0));
            m_gunLayer->removeDrawable(m_bullets[idx]);
            std::erase(m_bullets, m_bullets[idx]);
        });

        // Update boats
        m_boatManager.update(dt);

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
                m_bullets.push_back(std::move(m_gun->shoot(600)));
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
