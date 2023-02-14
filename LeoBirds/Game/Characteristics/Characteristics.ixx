export module Characteristics;

namespace birds::characteristics
{
    /**
     * @brief A struct that contains the characteristics of anything in the game.
     */
    export struct Characteristics { };

    /**
     * @brief A struct that contains the characteristics of a weapon.
     */
    export struct WeaponCharacteristics : Characteristics
    {
        unsigned int Health;
        float FireRate;
    };

    /**
     * @brief A struct that contains the characteristics of a boat, which is a weapon here.
     */
    export struct BoatCharacteristics : WeaponCharacteristics
    {
        float Speed;
    };

    /**
     * @brief A struct that contains the characteristics of a cannon, which is a weapon here.
     */
    export struct CannonCharacteristics : WeaponCharacteristics { };
}
