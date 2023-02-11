export module Engine.Entity;

namespace birds
{
    /**
     * \brief Any object than can change position when time passes
     */
    export class Entity
    {
    public:
        virtual ~Entity() = default;

        /**
         * \brief Updates the position of the object based on time
         * \param dt The time since the last update
         */
        virtual void update(float dt) = 0;
    };
}
