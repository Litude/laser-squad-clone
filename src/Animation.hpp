#ifndef LASER_ANIMATION_HPP
#define LASER_ANIMATION_HPP

class Animation
{
public:
	Animation(unsigned int row, unsigned int startFrame, unsigned int endFrame, unsigned int duration, bool loop = true)
	{
		this->offset = row;
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->duration = duration;
		this->loop = loop;
	}
	const unsigned int getLength() const { return endFrame - startFrame + 1; }
	unsigned int offset;
	unsigned int startFrame;
	unsigned int endFrame;
	bool loop;
	unsigned int duration;
};

#endif