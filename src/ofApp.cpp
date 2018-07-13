#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	time = 0;
	ofBackground(255, 255, 255);

	base[0] = 50;   //xç¿ïW
	base[1] = 50;   //yç¿ïW
	base[2] = 256*2;  //xïù
	base[3] = 512;  //yïù

	using my_engine = mt19937; // <-1
	using my_distribution = normal_distribution<>; // <-2

	particle = vector<ofVec2f>(8056, ofVec2f(0, 0));
}

//--------------------------------------------------------------
void ofApp::update() {
	time++;
	if (time >= 10000)time = 0;

	//if (time % 2 == 0)
	{


		for (size_t i = 0; i < particle.size(); i++)
		{
			int x = particle[i].x;
			int y = particle[i].y;

			int x_d = (x >= 0) ? (base[2] / 2) - x : (base[2] / 2) + x;
			int y_d = (y >= 0) ? (base[3] / 2) - y : (base[3] / 2) + y;

			//int dtc = (x_d + y_d)*0.8;
			int dtc = (x_d * y_d)*0.01;
			//int dtc = 50;
			//if (x_d + y_d <= 100)dtc = 10; else dtc = 50; 

			//ofVec3f nv(x + ofRandom(dtc)*cos(ofRandomf() * 1 * PI), y + ofRandom(dtc)*sin(ofRandomf() * 2 * PI));
			ofVec3f nv(x + ofRandom(-dtc, dtc), y + ofRandom(-dtc, dtc));
			if (abs(nv.x) > base[2] / 2)nv.x = (nv.x >= 0) ? base[2] - nv.x : -nv.x - base[2];
			if (abs(nv.y) > base[3] / 2)nv.y = (nv.y >= 0) ? base[3] - nv.y : -nv.y - base[3];

			particle[i] = nv;
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(127, 127, 127);
	ofNoFill();
	ofDrawRectangle(base[0], base[1], base[2], base[3]);
	ofSetColor(0, 0, 0);
	ofFill();

	int p_size = 2;
	ofDrawBitmapString("step: " + ofToString(time), 10, 30);
	ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 10, 15);
	for (size_t i = 0; i < particle.size(); i++)
	{
		ofDrawRectangle(particle[i].x - (p_size / 2) + base[0] + (base[2] / 2), particle[i].y - (p_size / 2) + base[1] + (base[3] / 2), p_size, p_size);
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'a')ofApp::setup();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
