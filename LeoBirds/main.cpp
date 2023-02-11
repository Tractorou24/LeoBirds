#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <memory>

import Sling;
import FLib.Application;
import FLib.Scene;
import FLib.Layer;

import Projectile;

int main()
{
    flib::Application app(sf::VideoMode(800, 600), "LeoBirds");

    const auto layer = std::make_shared<flib::Layer>();
    const auto projectile = std::make_shared<birds::Projectile>(sf::Vector2f(0, 0), "Assets/ball.jpg", 600);
    projectile->addSpeed({50, 5});
    layer->addDrawable(std::reinterpret_pointer_cast<sf::Drawable>(projectile));

    const auto scene = std::make_shared<flib::Scene>();
    scene->addLayer("game", layer);
    scene->onDraw.connect(sling::Slot<float>([&projectile](const float dt)
    {
        projectile->update(dt);
    }));

    app.addScene("main", scene);
    app.setActiveScene("main");
    app.run();

    return 0;
}
