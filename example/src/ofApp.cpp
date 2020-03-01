#include "ofApp.h"

ofApp::ofApp() : fpsCon() {
	fpsCon.transform.position = glm::vec3(0, 0, -15);
}

//--------------------------------------------------------------
void ofApp::setup(){
	ofxFirstPersonController::lockMouseCursor();
	this->cursor = true;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	fpsCon.pushMatrix();
	ofDrawSphere(glm::vec3(0,0,0), 5);
	fpsCon.popMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'e') {
		if (this->cursor) {
			ofxFirstPersonController::unlockMouseCursor();
		} else {
			ofxFirstPersonController::lockMouseCursor();
		}
		this->cursor = !this->cursor;
		fpsCon.mouseEnabled = this->cursor;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
