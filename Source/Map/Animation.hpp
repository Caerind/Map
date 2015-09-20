#ifndef MAP_ANIMATION_HPP
#define MAP_ANIMATION_HPP

#include <utility>
#include <vector>

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

namespace map
{

class Animation
{
    public:
        Animation();

        std::size_t getFrameCount() const;
        std::pair<int,sf::Time> getFrame(std::size_t index);
        void addFrame(int id, sf::Time t);
        sf::Vector2i update();
        void setLoop(bool loop);
        bool isLoop() const;

    private:
        sf::Clock mClock;
        bool mLoop;
        std::size_t mPosition;
        std::vector<std::pair<int,sf::Time>> mFrames;
};

} // namespace map

#endif // MAP_ANIMATION_HPP
