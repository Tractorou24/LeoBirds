export module LeoBirds;

import <SFML/Graphics.hpp>;

import <format>;
import <memory>;

import Projectile;

import Sling;
import FLib.Application;
import FLib.Scene;
import FLib.Layer;
import FLib.Layer;
import FLib.DrawableImage;

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

    // tmp
    std::shared_ptr<sf::VertexArray> m_line;
    std::shared_ptr<flib::DrawableImage> m_image, m_boat;
    std::shared_ptr<sf::Text> m_text;
    //end tmp

    bool m_is_tracing_line = false;
};

module: private;

LeoBirds::LeoBirds()
    : m_application(sf::VideoMode(1024, 607), "LeoBirds"),
      m_gameScene(std::make_shared<flib::Scene>()),
      m_gameLayer(std::make_shared<flib::Layer>())
{
    m_gameScene->addLayer("main", m_gameLayer);
    m_gameScene->onEvent.connect(sling::Slot<sf::Event>([&](const sf::Event& ev) { on_event(ev); }));
    m_gameScene->onDraw.connect(sling::Slot<float>([&](const float& dt) { on_draw(dt); }));

    // tmp
    m_image = std::make_shared<flib::DrawableImage>("Assets/background.png");
    m_boat = std::make_shared<flib::DrawableImage>("Assets/boat.png");
    m_text = std::make_shared<sf::Text>("Hello World", m_application.font(), 17);
    m_boat->setPosition({1024, 400});
    m_boat->setScale({0.4f, 0.4f});
    m_gameLayer->addDrawable(m_image);
    m_gameLayer->addDrawable(m_text);
    m_gameLayer->addDrawable(m_boat);
    // end tmp

    m_application.addScene("main", m_gameScene);
}

void LeoBirds::run()
{
    m_application.setActiveScene("main");
    m_application.run();
}

void LeoBirds::on_event(const sf::Event&) {}

void LeoBirds::on_draw(const float& dt)
{
    // Show frame time
    m_text->setString(std::format("Frame Time: {} ms", dt).c_str());

    // Move boat
    auto pos = m_boat->position();
    pos.x -= 100 * dt;
    m_boat->setPosition(pos);
    if (pos.x < 375)
        m_boat->setPosition({1024, 400});
}
