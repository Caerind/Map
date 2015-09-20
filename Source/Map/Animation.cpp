#include "Animation.hpp"

namespace map
{

Animation::Animation()
{
    mPosition = 0;
    mLoop = true;
}

std::size_t Animation::getFrameCount() const
{
    return mFrames.size();
}

std::pair<int,sf::Time> Animation::getFrame(std::size_t index)
{
    return mFrames.at(index);
}

void Animation::addFrame(int id, sf::Time t)
{
    std::pair<int,sf::Time> frame;
    frame.first = id;
    frame.second = t;
    mFrames.push_back(frame);
}

sf::Vector2i Animation::update()
{
    if (mFrames.size() > 0 && mPosition < mFrames.size())
    {
        if (mClock.getElapsedTime() > mFrames[mPosition].second)
        {
            std::size_t oldPosition = mPosition;
            mClock.restart();
            mPosition++;
            if (mLoop && mPosition == mFrames.size())
            {
                mPosition -= mFrames.size();
            }
            else if (!mLoop && mPosition == mFrames.size())
            {
                mPosition--;
                return sf::Vector2i();
            }
            return sf::Vector2i(mFrames[oldPosition].first,mFrames[mPosition].first);
        }
    }
    return sf::Vector2i();
}

void Animation::setLoop(bool loop)
{
    mLoop = loop;
}

bool Animation::isLoop() const
{
    return mLoop;
}

} // namespace map
