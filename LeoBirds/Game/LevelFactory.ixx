export module Levels.LevelManager;

import <memory>;
import <sstream>;
import <string>;

import Engine.Core.Factory;
import Levels.Level;

namespace birds::managers
{
    /**
     * \brief A factory used to instantiate an level type based on a memory buffer
     */
    export class LevelFactory final
    {
        template <typename LevelType, typename... Args>
        using LevelCreator = core::DerivedCreator<LevelType, birds::levels::Level, Args...>;

    public:
        /**
         * \brief Creates the level factory and register all levels types
         */
        LevelFactory();
        ~LevelFactory() = default;

        LevelFactory(const LevelFactory& other) = delete;
        LevelFactory(LevelFactory&& other) noexcept = delete;
        LevelFactory& operator=(const LevelFactory& other) = delete;
        LevelFactory& operator=(LevelFactory&& other) noexcept = delete;

        /**
         * \brief Instantiates a new level from the given typename and memory buffer
         * \param name The registered name of the level
         * \param memory_buffer The memory buffer containing the level data
         * \return The new level
         */
        std::unique_ptr<birds::levels::Level> create(const std::string& name, std::istringstream memory_buffer);

    private:
        core::Factory<std::string, birds::levels::Level, std::istringstream> m_factory;
    };
}

module: private;

namespace birds::managers
{
    LevelFactory::LevelFactory()
    {
        //TODO: Register all levels here
    }

    std::unique_ptr<birds::levels::Level> LevelFactory::create(const std::string& name, const std::istringstream memory_buffer)
    {
        return m_factory.create(name, memory_buffer);
    }
}
