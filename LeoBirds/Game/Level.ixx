export module Levels.Level;

import <SFML/System/Vector2.hpp>;
import <SFML/Window/Event.hpp>;

import <memory>;
import <sstream>;
import <string>;

import Sling;
import FLib.Application;
import FLib.Scene;
import FLib.Layer;
import FLib.DrawableImage;

import Engine.Entity;
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
        std::shared_ptr<flib::Scene> m_scene;
        std::shared_ptr<flib::Layer> m_backgroundLayer, m_gunLayer, m_boatLayer;

        std::shared_ptr<Gun> m_gun;
    };
}

module: private;

namespace birds::levels
{
    Level::Level()
        : m_scene(std::make_shared<flib::Scene>()), m_backgroundLayer(std::make_shared<flib::Layer>()),
          m_gunLayer(std::make_shared<flib::Layer>()), m_boatLayer(std::make_shared<flib::Layer>()),
          m_gun(std::make_shared<Gun>(sf::Vector2f(100, 500)))
    {
        m_scene = std::make_shared<flib::Scene>();
        m_backgroundLayer = std::make_shared<flib::Layer>();
        m_gunLayer = std::make_shared<flib::Layer>();
        m_boatLayer = std::make_shared<flib::Layer>();

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

    void Level::update(float) { }

    void Level::onEvent(const sf::Event& event)
    {
        switch (event.type)
        {
        case sf::Event::EventType::MouseMoved:
            m_gun->rotate(static_cast<sf::Vector2f>(sf::Mouse::getPosition()));
            break;
        default:
            break;
        }
    }
}
