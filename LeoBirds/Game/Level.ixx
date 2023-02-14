export module Levels.Level;

import <string>;

import FLib.Scene;
import Engine.Entity;

namespace birds::levels
{
    /**
     * \brief A base class for any level type
     */
    export class Level : public Entity
    {
    public:
        /**
         * \brief Creates a level from a memory buffer
         * \param memory_buffer The memory buffer to load the level from
         */
        explicit Level(std::istringstream memory_buffer);
        ~Level() override = default;

        /**
         * \return The scene used to display the level
         */
        virtual flib::Scene scene() = 0;
    };
}
