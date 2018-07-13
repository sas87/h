#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	isRunning = false;
	time = 0;
	ofBackground(255, 255, 255);

	box[0] = 50;     //x座標
	box[1] = 50;     //y座標
	box[2] = 512;  //x幅
	box[3] = 512;    //y幅
					 //512x512の箱を定義

	particle = vector<ofVec2f>(8000, ofVec2f(0, 0));
	//粒子8192個の生成	  (0,0)で初期化
	//粒子は二次元ベクトルクラス(ofVec2f)を使い、vectorでまとめて管理します
}

//--------------------------------------------------------------
void ofApp::update() {
	time++;
	if (time >= 10000)time = 0;

	//if (time % 2 == 0)     //コマ落とししたいときに
	{
		if (isRunning)
		{
			for (size_t i = 0; i < particle.size(); i++)
			{
			 int x = particle[i].x;
			 int y = particle[i].y;

			 int x_d = (x >= 0) ? (box[2] / 2) - x : (box[2] / 2) + x;
			 int y_d = (y >= 0) ? (box[3] / 2) - y : (box[3] / 2) + y;
			 //最短の上下の壁の距離を測定

			 int dtc = (x_d + y_d)*0.2;
			 //加算

			 //int dtc = (x_d * y_d)*0.01;
			 //乗算

			 //int dtc = (x_d + y_d <= 100) ? dtc = 0 : dtc = 50;
			 //定数

			 //ofVec3f nv(x + ofRandom(dtc)*cos(ofRandomf() * 1 * PI), y + ofRandom(dtc)*sin(ofRandomf() * 2 * PI));
			 ofVec3f nv(x + ofRandom(-dtc, dtc), y + ofRandom(-dtc, dtc));
			 if (abs(nv.x) > box[2] / 2)nv.x = (nv.x >= 0) ? box[2] - nv.x : -nv.x - box[2];
			 if (abs(nv.y) > box[3] / 2)nv.y = (nv.y >= 0) ? box[3] - nv.y : -nv.y - box[3];
			 //はみ出し防止処理

			 particle[i] = nv;
			}
		}
	}

}

int p_size = 2;//粒子サイズ
			   //--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(127, 127, 127);
	ofNoFill();
	ofDrawRectangle(box[0], box[1], box[2], box[3]);
	ofSetColor(0, 0, 0);
	ofFill();
	//箱の描画

	ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", box[0] + box[2] + 10, box[1]+15);
	ofDrawBitmapString("step: " + ofToString(time), box[0] + box[2] + 10, box[1]+30);
	{
		int x = mouseX - box[0] - (box[2] / 2);
		int y = mouseY - box[1] - (box[3] / 2);
		int x_d = (x >= 0) ? (box[2] / 2) - x : (box[2] / 2) + x;
		int y_d = (y >= 0) ? (box[3] / 2) - y : (box[3] / 2) + y;
		ofDrawBitmapString("dtc: " + ofToString((x_d + y_d)*0.2), box[0] + box[2] + 10, box[1] + 60);
	}
	

	for (size_t i = 0; i < particle.size(); i++)
	{
		ofDrawRectangle(particle[i].x - (p_size / 2) + box[0] + (box[2] / 2), particle[i].y - (p_size / 2) + box[1] + (box[3] / 2), p_size, p_size);
	}
	//粒子の描画

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'z')isRunning = !isRunning;
	//"z"を押すと実行/一時停止します
	if (key == 'a')ofApp::setup();
	//"a"を押すとリスタートします
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
