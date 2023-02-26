export module Levels.InitialLevel;

import <SFML/Graphics.hpp>;

import <memory>;

import Sling;
import FLib.Application;
import FLib.Scene;
import FLib.Layer;
import FLib.DrawableImage;

import Levels.Level;
import Gun;

namespace birds::levels
{
    export class InitialLevel final : public Level
    {
    public:
        /**
         * \brief Creates the initial level
         */
        explicit InitialLevel(flib::Application& app);
        ~InitialLevel() override = default;

        /**
         * \copydoc Level::update
         */
        void update(float dt) override;

        void onEvent(const ::sf::Event& event);
        void onDraw(float dt);

        /**
         * \copydoc Level::scene
         */
        std::shared_ptr<flib::Scene> scene() override { return m_scene; }

    private:
        flib::Application& m_application;
        std::shared_ptr<flib::Scene> m_scene;
        std::shared_ptr<flib::Layer> m_backgroundLayer, m_gunLayer, m_boatLayer;

        std::shared_ptr<Gun> m_gun;
    };
}

module: private;

namespace birds::levels
{
    InitialLevel::InitialLevel(flib::Application& app)
        : m_application(app), m_scene(std::make_shared<flib::Scene>()), m_backgroundLayer(std::make_shared<flib::Layer>()),
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

        m_scene->onDraw.connect(sling::Slot<float>([this](const float dt) { this->onDraw(dt); }));
        m_scene->onEvent.connect(sling::Slot<sf::Event>([this](const sf::Event& event) { this->onEvent(event); }));

        m_backgroundLayer->addDrawable(std::make_shared<flib::DrawableImage>("Assets/background.png"));
        m_gunLayer->addDrawable(m_gun);
    }

    void InitialLevel::update(float) { }

    void InitialLevel::onEvent(const sf::Event& event)
    {
        switch (event.type)
        {
        case sf::Event::EventType::MouseMoved:
            m_gun->rotate(m_application.mousePosition());
            break;
        default:
            break;
        }
    }

    void InitialLevel::onDraw(float) { }
}
