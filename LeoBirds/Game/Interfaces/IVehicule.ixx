export module Interfaces.IVehicule;

import <SFML/System/Vector2.hpp>;

namespace birds
{
    export class IVehicle
    {
    public:
        ~IVehicle() = default;

        /**
         * @brief The method called to move the vehicle
         * @param direction The direction and distance the vehicle need to move 
         */
        virtual void move(const sf::Vector2f& direction);
    };
}
