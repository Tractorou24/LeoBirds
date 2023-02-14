export module CharacteristicsFactory;

import <memory>;
import <string>;

import Engine.Core.Factory;
import Characteristics;

namespace birds::characteristics
{
    export class CharacteristicsFactory
    {
        template <typename CharacteristicsType, typename... Args>
        using CharacteristicsCreator = core::DerivedCreator<CharacteristicsType, Characteristics, Args...>;

    public:
        /**
         * \brief Creates the characteristics factory and register all usable types
         */
        CharacteristicsFactory();
        ~CharacteristicsFactory() = default;

        CharacteristicsFactory(const CharacteristicsFactory& other) = delete;
        CharacteristicsFactory(CharacteristicsFactory&& other) noexcept = delete;
        CharacteristicsFactory& operator=(const CharacteristicsFactory& other) = delete;
        CharacteristicsFactory& operator=(CharacteristicsFactory&& other) noexcept = delete;

        /**
         * \brief Instantiates a new characteristics structure from the given typename and memory buffer
         * \param name The registered name of the characteristics
         * \return The new empty characteristics structure
         */
        std::unique_ptr<Characteristics> create(const std::string& name);

    private:
        core::Factory<std::string, Characteristics> m_factory;
    };
}

module: private;

namespace birds::characteristics
{
    CharacteristicsFactory::CharacteristicsFactory()
    {
        m_factory.registerType("BoatCharacteristics", std::make_unique<CharacteristicsCreator<BoatCharacteristics>>());
        m_factory.registerType("CannonCharacteristics", std::make_unique<CharacteristicsCreator<CannonCharacteristics>>());
    }

    std::unique_ptr<Characteristics> CharacteristicsFactory::create(const std::string& name)
    {
        return m_factory.create(name);
    }
}
