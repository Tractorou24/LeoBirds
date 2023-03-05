export module LeoBirds;

import <SFML/Graphics.hpp>;
import <SFML/Audio/Music.hpp>;

import <format>;
import <memory>;
import <vector>;

import Sling;
import FLib.Application;
import FLib.Scene;
import FLib.Layer;
import FLib.DrawableImage;
import FLib.MessageBox;
import FLib.Button;
import FLib.TextButton;

import Levels.Level;

namespace birds
{
    export class LeoBirds
    {
    public:
        LeoBirds();
        ~LeoBirds() = default;

        int run();

    private:
        void setup_main_menu();

    private:
        flib::Application m_application;
        sf::Music m_menuMusic;

        std::shared_ptr<flib::Scene> m_menu;
        std::unique_ptr<levels::Level> m_level;
    };
}

module: private;

namespace birds
{
    LeoBirds::LeoBirds()
        : m_application(sf::VideoMode(1024, 607), "LeoBirds")
    {
        setup_main_menu();
        m_application.addScene("menu", m_menu);
        m_application.setActiveScene("menu");

        m_menuMusic.openFromFile("Assets/menu_music.ogg");
        m_menuMusic.setVolume(100.0f);
        m_menuMusic.play();
    }

    int LeoBirds::run()
    {
        m_application.run();
        return 0;
    }

    void LeoBirds::setup_main_menu()
    {
        const auto title = std::make_shared<sf::Text>("LeoBirds", m_application.font(), 75);
        title->setOrigin({title->getGlobalBounds().width / 2, title->getGlobalBounds().height / 2});
        title->setPosition({1024 / 2, 75});
        title->setFillColor(sf::Color::Black);
        title->setOutlineColor(sf::Color::White);
        title->setOutlineThickness(3.0f);

        const auto button_play = std::make_shared<flib::TextButton>(sf::Vector2f(150, 300), sf::Vector2f(150, 75),
                                                                    sf::Text("Play", m_application.font()),
                                                                    sf::Color::Black, sf::Color::White, 5.0f);
        button_play->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>([this](std::shared_ptr<flib::Button>)
        {
            m_menuMusic.stop();

            m_level = std::make_unique<levels::Level>();
            m_level->scene()->onDraw.connect(sling::Slot<float>([&](const float dt) { m_level->update(dt); }));
            m_level->onLoose.connect(sling::Slot<>([this]()
            {
                // Stop the level
                m_level->stop();

                // Create and show choice
                flib::MessageBox msg(m_application.font());
                msg.setTitle("You lost!");
                msg.setMessage("You are now a POW in Russia...");

                const auto btn = msg.addButton("Exit");
                btn->setOutlineColor(sf::Color::White);
                btn->setOutlineThickness(4);
                btn->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>([this, &msg](std::shared_ptr<flib::Button>)
                {
                    msg.hide();
                    std::exit(0);
                }));
                msg.show(m_application);
            }));
            m_application.addScene("level", m_level->scene());
            m_application.setActiveScene("level");
        }));

        const auto button_quit = std::make_shared<flib::TextButton>(sf::Vector2f(750, 300), sf::Vector2f(150, 75),
                                                                    sf::Text("Quit", m_application.font()),
                                                                    sf::Color::Black, sf::Color::White, 5.0f);
        button_quit->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>([this](std::shared_ptr<flib::Button>)
        {
            m_application.close();
        }));

        const auto layer = std::make_shared<flib::Layer>();
        layer->addDrawable(std::make_shared<flib::DrawableImage>("Assets/menu.png"));
        layer->addDrawable(title);
        layer->addDrawable(button_play);
        layer->addDrawable(button_quit);

        m_menu = std::make_shared<flib::Scene>();
        m_menu->addLayer("main", layer);
    }
}
