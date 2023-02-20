export module LeoBirds;

import <SFML/Graphics.hpp>;

import <memory>;

import Projectile;

import Sling;
import FLib.Application;
import FLib.Scene;
import FLib.Layer;
import FLib.Layer;

export class LeoBirds
{
public:
    LeoBirds();
    ~LeoBirds() = default;

    void run();

private:
    void on_event(const sf::Event& event);
    void on_draw(const float& dt);

private:
    flib::Application m_application;

    std::shared_ptr<flib::Scene> m_gameScene;
    std::shared_ptr<flib::Layer> m_gameLayer;

    std::shared_ptr<sf::VertexArray> m_line;
    bool m_is_tracing_line = false;
};

module: private;

LeoBirds::LeoBirds()
    : m_application(sf::VideoMode(800, 600), "LeoBirds"),
      m_gameScene(std::make_shared<flib::Scene>()),
      m_gameLayer(std::make_shared<flib::Layer>()),
      m_line(std::make_shared<sf::VertexArray>(sf::Lines, 2))
{
    m_gameScene->addLayer("main", m_gameLayer);
    m_gameScene->onEvent.connect(sling::Slot<sf::Event>([&](const sf::Event& ev) { on_event(ev); }));
    m_gameScene->onDraw.connect(sling::Slot<float>([&](const float& dt) { on_draw(dt); }));

    m_application.addScene("main", m_gameScene);
}

void LeoBirds::run()
{
    m_application.setActiveScene("main");
    m_application.run();
}

void LeoBirds::on_event(const sf::Event& event)
{
    switch (event.type)
    {
    case sf::Event::MouseButtonPressed:
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_is_tracing_line = true;
                const auto mouse_pos = m_application.mousePosition();
                (*m_line)[0].position = mouse_pos;
                (*m_line)[0].color = sf::Color(128, 128, 128);
                (*m_line)[1].position = mouse_pos;
                (*m_line)[1].color = sf::Color(128, 128, 128);
                m_gameLayer->addDrawable(m_line);
            }
            break;
        }
    case sf::Event::MouseButtonReleased:
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                m_is_tracing_line = false;
                m_gameLayer->removeDrawable(m_line);
            }
            break;
        }
    default:
        break;
    }
}

void LeoBirds::on_draw(const float&)
{
    if (m_is_tracing_line)
        (*m_line)[1].position = m_application.mousePosition();
}
