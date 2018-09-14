#pragma once

#include "ofMain.h"
#include <random>
#include <math.h>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

private:
	bool isRunning;
	//実行中はtrue それ以外はfalse
	int box[4];
	//箱を定義
	int time;
	//実行してから1フレームごと1増える10000でループする変数です。
	vector<ofVec2f> particle;
	vector<ofVec2f> particle0;
	vector<ofVec2f> ptcl0;
	//粒子
	float dtc(float x_d, float y_d);

	ofVec2f toRelativeC(ofVec2f v);
	void toRelativeC(float* vx, float* vy);
	ofVec2f toAbsoluteC(ofVec2f &v);
	void toAbsoluteC(float* vx, float* vy);

	float toRelativeCx(float vx);
	float toRelativeCy(float vy);
	float toAbsoluteCx(float vx);
	float toAbsoluteCy(float vy);
	vector<ofVec4f> rigidBox;
		
};
