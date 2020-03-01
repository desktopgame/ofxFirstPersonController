#pragma once
#ifndef OFXFIRSTPERSONCONTROLLER_HPP
#define OFXFIRSTPERSONCONTROLLER_HPP
#include <glm/glm.hpp>
#include <ofEvents.h>

namespace ofxFirstPersonController {
/**
 * Key.
 */
struct Key {
	explicit Key(int keycode);
	int keycode;
	bool enabled;
};

/**
 * Transform.
 */
struct Transform {
	explicit Transform();
	glm::vec3 forward() const;
	glm::vec3 backward() const;
	glm::vec3 left() const;
	glm::vec3 right() const;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
};

/**
 * Controller.
 */
class Controller {
public:
	explicit Controller();
	~Controller();

	Transform transform;
	float moveSpeed;
	float rotationSpeed;
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	glm::vec3 upVector;
	glm::vec2 rotationAxis;

	Key forwardKey;
	Key backwardKey;
	Key leftKey;
	Key rightKey;

	Key downKey;
	Key upKey;

	void pushMatrix() const;
	void popMatrix() const;

	glm::mat4 viewMatrix() const;
	glm::mat4 projectionMatrix() const;
protected:

	void update(ofEventArgs& e);
	void keyPressed(ofKeyEventArgs& e);
	void keyReleased(ofKeyEventArgs& e);
	void mouseMoved(ofMouseEventArgs& e);
	void mouseDragged(ofMouseEventArgs& e);
private:
	bool wasdEnabled;
	bool isSprint;
	glm::ivec2 wasdAxis;

	bool mouseEnabled;
	glm::vec2 oldMousePos;
	glm::vec2 diffMousePos;
};
}
#endif