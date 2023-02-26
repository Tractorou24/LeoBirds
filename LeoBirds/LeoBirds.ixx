export module LeoBirds;

import <SFML/Graphics.hpp>;

import <format>;
import <memory>;
import <vector>;

import FLib.Application;

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
        flib::Application m_application;
        std::unique_ptr<levels::Level> m_level;
    };
}

module: private;

namespace birds
{
    LeoBirds::LeoBirds()
        : m_application(sf::VideoMode(1024, 607), "LeoBirds"), m_level(std::make_unique<levels::Level>())
    {
        m_application.addScene("level", m_level->scene());
        m_application.setActiveScene("level");
    }

    int LeoBirds::run()
    {
        m_application.run();
        return 0;
    }
}
