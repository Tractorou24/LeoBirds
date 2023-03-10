export module BoatManager;

import <SFML/System/Vector2.hpp>;

import <array>;
import <memory>;
import <random>;

import Sling;
import FLib.Layer;

import Engine.Entity;
import Projectile;
import Boat;

namespace birds
{
    /**
     * @brief A class responsible for managing the boats.
     */
    export class BoatManager final : public Entity
    {
    public:
        explicit BoatManager();
        ~BoatManager() override = default;

        /**
         * @brief Checks if any of the boats have been hit by a projectile.
         * @param projectiles The projectiles to check.
         * @return The indices of the projectiles that hit a boat.
         */
        [[nodiscard]] std::vector<std::size_t> checkDamage(std::vector<std::shared_ptr<Projectile>> projectiles);

        /**
         * @copydoc Entity::update
         */
        void update(float dt) override;

        /**
         * @return The layer containing the boats.
         */
        std::shared_ptr<flib::Layer> layer() { return m_layer; }

        /**
         * @brief The signal emitted when a boat reaches the beach.
         */
        sling::Signal<> onLoose;

    private:
        std::shared_ptr<flib::Layer> m_layer;
        std::array<sf::Vector2f, 5> m_boatsStarts;
        std::array<std::shared_ptr<Boat>, 5> m_boats;
    };
}

module: private;

namespace birds
{
    BoatManager::BoatManager()
        : m_layer(std::make_shared<flib::Layer>())
    {
        std::random_device rd;
        std::uniform_int_distribution dist(0, 35);

        for (std::size_t i = 0; i < m_boats.size(); ++i)
        {
            const float x = 1000.0f + 500.0f * i;
            const float y = static_cast<float>(470 + dist(rd));
            m_boatsStarts[i] = {x, y};
            m_boats[i] = std::make_shared<Boat>(m_boatsStarts[i]);

            const auto scale = 0.25f + (y - 470) / 350;
            m_boats[i]->setScale({scale, scale});
        }

        std::ranges::sort(m_boats, [](const auto& lhs, const auto& rhs) { return lhs->position().y < rhs->position().y; });
        std::ranges::for_each(m_boats, [this](const auto& boat) { m_layer->addDrawable(boat); });
    }

    std::vector<std::size_t> BoatManager::checkDamage(std::vector<std::shared_ptr<Projectile>> projectiles)
    {
        std::vector<std::size_t> projectilesHit;
        std::ranges::for_each(projectiles, [&](const auto& projectile)
        {
            std::ranges::for_each(m_boats, [&](const auto& boat)
            {
                if (boat->globalBounds().intersects(projectile->globalBounds())) // Boat hit, reset it
                {
                    projectilesHit.push_back(std::distance(projectiles.begin(), std::ranges::find(projectiles, projectile)));
                    const std::size_t boat_idx = std::distance(m_boats.begin(), std::ranges::find(m_boats, boat));

                    m_boats[boat_idx]->setPosition(m_boatsStarts[boat_idx]);
                }
            });
        });
        return projectilesHit;
    }

    void BoatManager::update(const float dt)
    {
        for (std::size_t i = 0; i < m_boats.size(); ++i)
        {
            if (m_boats[i]->position().x < 375) // Boat at the beach, loose game
                onLoose.emit();
            m_boats[i]->update(dt);
        }
    }
}
