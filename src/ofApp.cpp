#include "ofApp.h"
//https://github.com/sas87/h/blob/master/src/ofApp.cpp



//--------------------------------------------------------------
void ofApp::setup() {
	isRunning = false;
	time = 0;
	ofBackground(200, 200, 200);

	box[0] = 50;     //x座標
	box[1] = 50;     //y座標
	box[2] = 512;    //x幅
	box[3] = 512;    //y幅
	//512x512の箱を定義

	particle = vector<ofVec2f>(8000, ofVec2f(100, 100));
	//粒子8000個の生成	  (0,0)で初期化
	//粒子は二次元ベクトルクラス(ofVec2f)を使い、vectorでまとめて管理します

	rigidBox = vector<ofVec4f>();
	//rigidBox.push_back(ofVec4f(100, 120, 500, 500));
	rigidBox.push_back(ofVec4f(-180, -60, 100, 200));
}

//--------------------------------------------------------------

bool isMouseTrk = false;
int state = 0;

void ofApp::update() {
	time++;
	if (time >= 10000)time = 0;
	// (time % 5 == 0)     //コマ落とししたいときに
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

				int dtc = 10;

				ofVec3f nv(x + ofRandom(dtc)*cos(ofRandomf() * 1 * PI), y + ofRandom(dtc)*sin(ofRandomf() * 2 * PI));
				//ofVec2f nv(x + ofRandom(-dtc, dtc), y + ofRandom(-dtc, dtc));//新しい座標候補
				//dddddddddddddddddd
				
				//int i_r = 0;//debug
				for (size_t i_r = 0; i_r < rigidBox.size(); i_r++)
				{
					if (i == 0)state = 10;
					if (((nv.x - rigidBox[i_r].x)*(nv.x - rigidBox[i_r].x - rigidBox[i_r].z) <= 0) &&
						((nv.y - rigidBox[i_r].y)*(nv.y - rigidBox[i_r].y - rigidBox[i_r].w) <= 0))//nvが障害物内に存在するとき
					{					
						//dddddddddddddd
						if (i == 0) 
						{
							state = 20;
						}

						ofVec2f colliP; //現座標からnvへ直線を引いた時の境界線との交点
						if ((y >= rigidBox[i_r].y && y <= rigidBox[i_r].y + rigidBox[i_r].w) && (x < rigidBox[i_r].x || x > rigidBox[i_r].x + rigidBox[i_r].z))//左右
						{
							//ddddd
							if (i == 0)state += 1;
							colliP.x = (x - rigidBox[i_r].x > 0) ? rigidBox[i_r].x + rigidBox[i_r].z : rigidBox[i_r].x;
							colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.y);
						}
						else if ((x >= rigidBox[i_r].x && x <= rigidBox[i_r].x + rigidBox[i_r].z) && (y < rigidBox[i_r].y || y > rigidBox[i_r].y + rigidBox[i_r].w))//上下
						{
							//ddddd
							if (i == 0)state += 2;
							colliP.y = (y - rigidBox[i_r].y > 0) ? rigidBox[i_r].y + rigidBox[i_r].w : rigidBox[i_r].y;
							colliP.x = (x*(y - colliP.y) + nv.x*(colliP.y - nv.y))*1.0f / (y - nv.y);
						}
						else if (x < rigidBox[i_r].x && y < rigidBox[i_r].y)//左上
						{
							//ddddd
							if (i == 0)state += 3;
							/**/
							ofVec2f cv = rigidBox[i_r];
							float slp_C = (y - cv.y)*1.0f / (x - cv.x);
							float slp_NV = (y - nv.y)*1.0f / (x - nv.x);
							if (slp_C >= slp_NV)
							{
								colliP.x = cv.x;
								colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.y);
							}
							else
							{
								colliP.y = cv.y;
								colliP.x = (x*(y - colliP.y) + nv.x*(colliP.y - nv.y))*1.0f / (y - nv.y);
							}
						}
						else if (x < rigidBox[i_r].x && y > rigidBox[i_r].y + rigidBox[i_r].w)//左下
						{
							//ddddd
							if (i == 0)state += 4;
							/**/
							ofVec2f cv = { rigidBox[i_r].x ,rigidBox[i_r].y + rigidBox[i_r].w };
							float slp_C = (y - cv.y)*1.0f / (x - cv.x);
							float slp_NV = (y - nv.y)*1.0f / (x - nv.x);
							if (slp_C <= slp_NV)
							{
								colliP.x = cv.x;
								colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.y);
							}
							else
							{
								colliP.y = cv.y;
								colliP.x = (x*(y - colliP.y) + nv.x*(colliP.y - nv.y))*1.0f / (y - nv.y);
							}
						}
						else if (x > rigidBox[i_r].x + rigidBox[i_r].z && y > rigidBox[i_r].y + rigidBox[i_r].w)//右下
						{
							//ddddd
							if (i == 0)state += 5;
							/**/
							ofVec2f cv = { rigidBox[i_r].x + rigidBox[i_r].z ,rigidBox[i_r].y + rigidBox[i_r].w };
							float slp_C = (y - cv.y)*1.0f / (x - cv.x);
							float slp_NV = (y - nv.y)*1.0f / (x - nv.x);
							if (slp_C >= slp_NV)
							{
								colliP.x = cv.x;
								colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.y);
							}
							else
							{
								colliP.y = cv.y;
								colliP.x = (x*(y - colliP.y) + nv.x*(colliP.y - nv.y))*1.0f / (y - nv.y);
							}
						}
						else if (x > rigidBox[i_r].x + rigidBox[i_r].z && y < rigidBox[i_r].y)//右上
						{
							//ddddd
							if (i == 0)state += 6;
							/**/
							ofVec2f cv = { rigidBox[i_r].x + rigidBox[i_r].z ,rigidBox[i_r].y };
							float slp_C = (y - cv.y)*1.0f / (x - cv.x);
							float slp_NV = (y - nv.y)*1.0f / (x - nv.x);
							if (slp_C <= slp_NV)
							{
								colliP.x = cv.x;
								colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.y);
							}
							else
							{
								colliP.y = cv.y;
								colliP.x = (x*(y - colliP.y) + nv.x*(colliP.y - nv.y))*1.0f / (y - nv.y);
							}
						}
						nv = colliP;
					}
				}

				if (abs(nv.x) > box[2] / 2)nv.x = (nv.x >= 0) ? box[2] - nv.x : -nv.x - box[2];
				if (abs(nv.y) > box[3] / 2)nv.y = (nv.y >= 0) ? box[3] - nv.y : -nv.y - box[3];
				//はみ出し防止処理

				particle[i] = nv;
			}
		}
		if (isMouseTrk) 
		{
			rigidBox[0].x = toRelativeCx(mouseX);
			rigidBox[0].y = toRelativeCx(mouseY);
		}

	}

}
int ofApp::dtc(int x_d, int y_d)
{
	int dtc = ((x_d + 0) * (y_d + 0))*0.001;
	//int dtc = (x_d + y_d)*0.2;
	//加算

	//int dtc = ofApp::dtc(x_d,y_d);
	//乗算

	//int dtc = (x_d + y_d <= 100) ? dtc = 0 : dtc = 50;
	//離散
	return dtc;
}
ofVec2f ofApp::toRelativeC(ofVec2f v)
{
	return ofVec2f(v.x - box[0] - (box[2] / 2), v.y - box[1] - (box[3] / 2));
}
void ofApp::toRelativeC(int* vx, int* vy)
{
	*vx = *vx - box[0] - (box[2] / 2);
	*vy = *vy - box[1] - (box[3] / 2);
}
ofVec2f ofApp::toAbsoluteC(ofVec2f &v)
{
	return ofVec2f(v.x + box[0] + (box[2] / 2), v.y + box[1] + (box[3] / 2));;
}
void ofApp::toAbsoluteC(int* vx, int* vy)
{
	*vx = *vx + box[0] + (box[2] / 2);
	*vy = *vy + box[1] + (box[3] / 2);
}
int ofApp::toRelativeCx(int vx)
{
	return vx - box[0] - (box[2] / 2);
}
int ofApp::toRelativeCy(int vy)
{
	return vy - box[1] - (box[3] / 2);;
}
int ofApp::toAbsoluteCx(int vx)
{
	return vx + box[0] + (box[2] / 2);
}
int ofApp::toAbsoluteCy(int vy)
{
	return vy + box[0] + (box[2] / 2);
}
int p_size = 1;//粒子サイズ
int state_checker = 0;
//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	//ofNoFill();
	ofFill();
	ofDrawRectangle(box[0], box[1], box[2], box[3]);
	//
	ofSetColor(200, 200, 200);
	for (size_t i_r = 0; i_r < rigidBox.size(); i_r++)
	{
		ofDrawRectangle(toAbsoluteCx(rigidBox[i_r].x), toAbsoluteCy(rigidBox[i_r].y), rigidBox[i_r].z, rigidBox[i_r].w);
	}
	ofSetColor(0, 0, 0);

	//箱の描画

	ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", box[0] + box[2] + 50, box[1] + 15);
	ofDrawBitmapString("step: " + ofToString(time), box[0] + box[2] + 50, box[1] + 30);
	{
		int x = mouseX;
		int y = mouseY;
		toRelativeC(&x, &y);
		int x_d = (x >= 0) ? (box[2] / 2) - x : (box[2] / 2) + x;
		int y_d = (y >= 0) ? (box[3] / 2) - y : (box[3] / 2) + y;


		int dtc = ofApp::dtc(x_d, y_d);

		ofVec2f nv(x + ofRandom(-dtc, dtc), y + ofRandom(-dtc, dtc));
		for (size_t i_r = 0; i_r < rigidBox.size(); i_r++)
		{
			if ((nv.x - rigidBox[i_r].x)*(nv.x - rigidBox[i_r].x - rigidBox[i_r].w) <= 0) { nv = { 0,0 }; }
		}

		if (isRunning && state!=10)
		{
			state_checker++;
		}
		ofDrawBitmapString(
			(string)""
			+ "isRunning: " + ofToString(isRunning) + "\r\n"
			+ "mouse: (" + ofToString(toRelativeCx(mouseX)) + ", " + ofToString(toRelativeCy(mouseY)) + ")" + "\r\n"

			+ "\r\n"
			+ "dtc: " + ofToString(dtc) + "\r\n"

			+ "\r\n"
			+ "mouseX: " + ofToString(x) + "\r\n"
			+ "mouseY: " + ofToString(mouseX) + "\r\n"
			+ "x_distance: " + ofToString((x - rigidBox[0].x)*(x - rigidBox[0].x - rigidBox[0].z)) + "\r\n"
			+ "\r\n"
			+ "\r\n"
			+ "p[0]: ( " + ofToString(particle[0].x) + ", " + ofToString(particle[0].y) + " )" + "\r\n"
			+ "state: " + ofToString(state) + "\r\n"
			+ "stateC: " + ofToString(state_checker) + "\r\n"

			,
			box[0] + box[2] + 50, box[1] + 60);
	}
	//文の描画


	ofSetColor(0, 0, 0);
	for (size_t i = 0 + 1; i < particle.size(); i++)
	{
		ofDrawRectangle(toAbsoluteCx(particle[i].x - (p_size / 2)), toAbsoluteCy(particle[i].y - (p_size / 2)), p_size, p_size);
	}
	ofSetColor(255, 0, 0);
	ofDrawRectangle(toAbsoluteCx(particle[0].x - p_size), toAbsoluteCy(particle[0].y - p_size), p_size * 2, p_size * 2);
	ofNoFill();
	{
		int x = particle[0].x;
		int y = particle[0].y;

		int x_d = (x >= 0) ? (box[2] / 2) - x : (box[2] / 2) + x;
		int y_d = (y >= 0) ? (box[3] / 2) - y : (box[3] / 2) + y;
		ofDrawCircle(toAbsoluteCx(x), toAbsoluteCy(y), 10);
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'z')isRunning = !isRunning;
	//"z"を押すと実行/一時停止します
	if (key == 'a')ofApp::setup();
	//"a"を押すとリスタートします

	if (key == 'x')isMouseTrk = !isMouseTrk;
	//"x"でマウストラックon/off
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
	particle[0].x = mouseX - box[0] - (box[2] / 2);
	particle[0].y = mouseY - box[1] - (box[3] / 2);
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
