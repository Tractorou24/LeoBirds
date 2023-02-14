export module Interfaces.IWeapon;

import <memory>;

import Characteristics;

namespace birds
{
    export class IWeapon
    {
    public:
        virtual ~IWeapon() = default;

        /**
         * @brief The method called when fire trigger is pressed
         */
        virtual void fire() = 0;

        /**
         * @brief The method called when the weapon is hit by a projectile
         */
        virtual void takeDamage() = 0;
    };
}
