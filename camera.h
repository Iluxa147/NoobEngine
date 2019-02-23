#pragma once

#include "context.h"

class Camera
{
public:
	explicit Camera(glm::vec3 pos, glm::vec3 target) : pos_(pos), target_(target)
	{
		direction_ = glm::normalize(pos_ - target_); //+z, opposite view direction
		right_ = glm::cross(direction_, glm::vec3(0.0f, 1.0f, 0.0f));
		up_ = glm::cross(direction_, right_);
	};
	
	~Camera() {};

private:
	glm::vec3 pos_;
	glm::vec3 target_;
	glm::vec3 direction_;
	glm::vec3 right_;
	glm::vec3 up_;

};