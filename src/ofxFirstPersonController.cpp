#include "ofxFirstPersonController.h"
#include <ofEventUtils.h>
#include <ofUtils.h>
#include <ofMath.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ofMatrixStack.h>
#include <ofGraphics.h>

namespace ofxFirstPersonController {
// Transform
	Transform::Transform() : position(), scale(1,1,1), rotation() {
}
glm::vec3 Transform::forward() const {
	const float DEG2RAD = 3.14f / 180.0f;
	auto ax =
		std::sin(rotation.x * DEG2RAD) * std::cos(rotation.y * DEG2RAD) * 1;
	auto ay = std::sin(rotation.y * DEG2RAD);
	auto az =
		std::cos(rotation.x * DEG2RAD) * std::cos(rotation.y * DEG2RAD) * 1;
	return glm::vec3(ax, ay, az);
}
glm::vec3 Transform::backward() const {
	return -forward();
}
glm::vec3 Transform::left() const {
	return -right();
}
glm::vec3 Transform::right() const {
	auto fwd = forward();
	return glm::vec3(-fwd.z, 0, fwd.x);
}
// Controller
Controller::Controller()
 :  // public
	transform(), moveSpeed(1.0f), rotationSpeed(0.6f),
    fov(30), aspectRatio(1280.0f / 720.0f), nearPlane(1.0f), farPlane(1000.0f), upVector(0,-1,0), rotationAxis(1,-1),
	// private
	wasdEnabled(true), isSprint(true), wasdAxis(0,0),
	oldMousePos(-1, -1), diffMousePos(), mouseEnabled(false) 
{
	auto &events = ofEvents();
	ofAddListener(events.update, this, &Controller::update, OF_EVENT_ORDER_BEFORE_APP);
	ofAddListener(events.keyPressed, this, &Controller::keyPressed, OF_EVENT_ORDER_BEFORE_APP);
	ofAddListener(events.keyReleased, this, &Controller::keyReleased, OF_EVENT_ORDER_BEFORE_APP);
	ofAddListener(events.mouseMoved, this, &Controller::mouseMoved, OF_EVENT_ORDER_BEFORE_APP);
	ofAddListener(events.mouseDragged, this, &Controller::mouseDragged, OF_EVENT_ORDER_BEFORE_APP);
}
Controller::~Controller() {
	auto &events = ofEvents();
	ofRemoveListener(events.update, this, &Controller::update, OF_EVENT_ORDER_BEFORE_APP);
	ofRemoveListener(events.keyPressed, this, &Controller::keyPressed, OF_EVENT_ORDER_BEFORE_APP);
	ofRemoveListener(events.keyReleased, this, &Controller::keyReleased, OF_EVENT_ORDER_BEFORE_APP);
	ofRemoveListener(events.mouseMoved, this, &Controller::mouseMoved, OF_EVENT_ORDER_BEFORE_APP);
	ofRemoveListener(events.mouseDragged, this, &Controller::mouseDragged, OF_EVENT_ORDER_BEFORE_APP);
}

void Controller::pushMatrix() const {
	ofSetMatrixMode(ofMatrixMode::OF_MATRIX_PROJECTION);
	ofPushMatrix();
	ofLoadIdentityMatrix();
	ofLoadMatrix(projectionMatrix());

	ofSetMatrixMode(ofMatrixMode::OF_MATRIX_MODELVIEW);
	ofPushMatrix();
	ofLoadIdentityMatrix();
	ofLoadMatrix(viewMatrix());
}

void Controller::popMatrix() const {
	ofSetMatrixMode(ofMatrixMode::OF_MATRIX_PROJECTION);
	ofPopMatrix();

	ofSetMatrixMode(ofMatrixMode::OF_MATRIX_MODELVIEW);
	ofPopMatrix();
}

glm::mat4 Controller::viewMatrix() const {
	return glm::lookAt(this->transform.position, this->transform.position + this->transform.forward(), upVector);
}
glm::mat4 Controller::projectionMatrix()  const {
	return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}
void Controller::update(ofEventArgs & e) {
	// calculate rotation
	transform.rotation.x += diffMousePos.x * this->rotationSpeed * this->rotationAxis.x;
	transform.rotation.y += diffMousePos.y * this->rotationSpeed * this->rotationAxis.y;
	transform.rotation.y = ofClamp(transform.rotation.y, -90.0f, 90.0f);
	if (transform.rotation.x > 360) {
		transform.rotation.x -= 360;
	}
	if (transform.rotation.y > 360) {
		transform.rotation.y -= 360;
	}
	if (transform.rotation.z > 360) {
		transform.rotation.z -= 360;
	}
	// calculate velocity
	glm::vec3 velocity = glm::vec3();
	glm::vec3 force = glm::vec3(moveSpeed, 0.0f, moveSpeed);
	float saveY = transform.rotation.y;
	transform.rotation.y = 0;
	if (wasdAxis.y == 1) {
		velocity = transform.forward() * force;
	} else if (wasdAxis.y == -1) {
		velocity = transform.backward() * force;
	}
	if (wasdAxis.x == -1) {
		velocity = transform.left() * force;
	} else if (wasdAxis.x == 1) {
		velocity = transform.right() * force;
	}
	transform.rotation.y = saveY;
	transform.position += velocity;
	diffMousePos.x = diffMousePos.y = 0;
}
void Controller::keyPressed(ofKeyEventArgs & e) {
	// check move key
	if (e.key == 'w') {
		this->wasdAxis.y = 1;
		std::cout << "press " <<  'w' << std::endl;
	} else if (e.key == 's') {
		this->wasdAxis.y = -1;
		std::cout << "press " << 's' << std::endl;
	}
	if (e.key == 'a') {
		this->wasdAxis.x = -1;
		std::cout << "press " << 'a' << std::endl;
	} else if (e.key == 'd') {
		this->wasdAxis.x = 1;
		std::cout << "press " << 'd' << std::endl;
	}
}
void Controller::keyReleased(ofKeyEventArgs & e) {
	// check move key
	if (e.key == 'w' && wasdAxis.y == 1) {
		this->wasdAxis.y = 0;
		std::cout << "release " << 'w' << std::endl;
	} else if (e.key == 's' && wasdAxis.y == -1) {
		this->wasdAxis.y = 0;
		std::cout << "release " << 's' << std::endl;
	}
	if (e.key == 'a' && wasdAxis.x == -1) {
		this->wasdAxis.x = 0;
		std::cout << "release " << 'a' << std::endl;
	} else if (e.key == 'd' && wasdAxis.x == 1) {
		this->wasdAxis.x = 0;
		std::cout << "release " << 'd' << std::endl;
	}
}
void Controller::mouseMoved(ofMouseEventArgs & e) {
	diffMousePos.x = diffMousePos.y = 0;
	double dmx = e.x;
	double dmy = e.y;
	if (oldMousePos.x == -1 && oldMousePos.y == -1) {
		this->oldMousePos.x = dmx;
		this->oldMousePos.y = dmy;
	} else {
		this->diffMousePos.x = -(dmx - oldMousePos.x);
		this->diffMousePos.y = -(dmy - oldMousePos.y);
		this->oldMousePos.x = dmx;
		this->oldMousePos.y = dmy;
	}
}
void Controller::mouseDragged(ofMouseEventArgs & e) {
}
}