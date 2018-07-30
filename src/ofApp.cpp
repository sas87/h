#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	isRunning = false;
	time = 0;
	ofBackground(255, 255, 255);

	box[0] = 50;     //x座標
	box[1] = 50;     //y座標
	box[2] = 512;    //x幅
	box[3] = 512;    //y幅
	//512x512の箱を定義

	particle = vector<ofVec2f>(8000, ofVec2f(0, 0));
	//粒子8000個の生成	  (0,0)で初期化
	//粒子は二次元ベクトルクラス(ofVec2f)を使い、vectorでまとめて管理します

	rigidBox = vector<ofVec4f>();
	rigidBox.push_back(ofVec4f(-50, -120, 150, 100));
	rigidBox.push_back(ofVec4f(-180, -60, 100, 200));
}

//--------------------------------------------------------------
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

				int dtc = 50;

				//ofVec3f nv(x + ofRandom(dtc)*cos(ofRandomf() * 1 * PI), y + ofRandom(dtc)*sin(ofRandomf() * 2 * PI));
				ofVec2f nv(x + ofRandom(-dtc, dtc), y + ofRandom(-dtc, dtc));
				for (size_t i = 0; i < rigidBox.size(); i++)
				{
					if (((nv.x - rigidBox[i].x)*(nv.x - rigidBox[i].x - rigidBox[i].z) <= 0) &&
						((nv.y - rigidBox[i].y)*(nv.y - rigidBox[i].y - rigidBox[i].w) <= 0))//nvが障害物内に存在するとき
					{
						ofVec2f colliP; //現座標からnvへ直線を引いた時の境界線との交点
						if ((y <= rigidBox[i].y && y >= rigidBox[i].y + rigidBox[i].w) && (x < rigidBox[i].x || x > rigidBox[i].x + rigidBox[i].z))//左右
						{
							colliP.x = (x - rigidBox[i].x > 0) ? rigidBox[i].x + rigidBox[i].z : rigidBox[i].x;				
							colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.y);
						}
						else if ((x <= rigidBox[i].x&&x >= rigidBox[i].x + rigidBox[i].z) && (y < rigidBox[i].y || y > rigidBox[i].y + rigidBox[i].w))//上下
						{
							colliP.y= (y - rigidBox[i].y > 0) ? rigidBox[i].y + rigidBox[i].w : rigidBox[i].y;
							colliP.x = (x*(y - colliP.y) + nv.x*(colliP.y - nv.y))*1.0f / (y - nv.y);
						}
						else if (x < rigidBox[i].x && y < rigidBox[i].y)//左上
						{
							ofVec2f cv = { rigidBox[i].x ,rigidBox[i].y };
							float ktmk_C = (y - cv.y)*1.0f / (x - cv.x);
							float ktmk_NV = (y - nv.y)*1.0f / (x - nv.x);
							if (ktmk_C >= ktmk_NV) 
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
						else if (x < rigidBox[i].x && y > rigidBox[i].y + rigidBox[i].w)//左下
						{
							ofVec2f cv = { rigidBox[i].x ,rigidBox[i].y + rigidBox[i].w };
							float ktmk_C = (y - cv.y)*1.0f / (x - cv.x);
							float ktmk_NV = (y - nv.y)*1.0f / (x - nv.x);
							if (ktmk_C <= ktmk_NV)
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
						else if (x > rigidBox[i].x + rigidBox[i].z && y > rigidBox[i].y + rigidBox[i].w)//右下
						{
							ofVec2f cv = { rigidBox[i].x + rigidBox[i].z ,rigidBox[i].y + rigidBox[i].w };
							float ktmk_C = (y - cv.y)*1.0f / (x - cv.x);
							float ktmk_NV = (y - nv.y)*1.0f / (x - nv.x);
							if (ktmk_C >= ktmk_NV)
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
						else if (x > rigidBox[i].x + rigidBox[i].z && y < rigidBox[i].y )//右上
						{
							ofVec2f cv = { rigidBox[i].x + rigidBox[i].z ,rigidBox[i].y};
							float ktmk_C = (y - cv.y)*1.0f / (x - cv.x);
							float ktmk_NV = (y - nv.y)*1.0f / (x - nv.x);
							if (ktmk_C <= ktmk_NV)
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
						nv = 0.5f*particle[i] + 0.5f*colliP;
						//nv.x = ((nv.x - x) / 100.0f)*x + (1 - ((nv.x - x) / 100.0f))*((x - rigidBox[i].x > 0) ? rigidBox[i].x + rigidBox[i].z : rigidBox[i].x);
						//nv.y = ((nv.y - y) / 100.0f)*y + (1 - ((nv.y - y) / 100.0f))*((y - rigidBox[i].y > 0) ? rigidBox[i].y + rigidBox[i].w : rigidBox[i].y);
						//nv = { 0,0 }; 
					}
				}

				if (abs(nv.x) > box[2] / 2)nv.x = (nv.x >= 0) ? box[2] - nv.x : -nv.x - box[2];
				if (abs(nv.y) > box[3] / 2)nv.y = (nv.y >= 0) ? box[3] - nv.y : -nv.y - box[3];
				//はみ出し防止処理

				particle[i] = nv;
			}
		}
	}

}
int ofApp::dtc(int x_d,int y_d)
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
			   //--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(127, 127, 127);
	ofNoFill();
	ofDrawRectangle(box[0], box[1], box[2], box[3]);

	for (size_t i = 0; i < rigidBox.size(); i++)
	{
		ofDrawRectangle(toAbsoluteCx(rigidBox[i].x), toAbsoluteCy(rigidBox[i].y), rigidBox[i].z , rigidBox[i].w);
	}
	ofSetColor(0, 0, 0);
	ofFill();
	//箱の描画

	ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", box[0] + box[2] + 10, box[1] + 15);
	ofDrawBitmapString("step: " + ofToString(time), box[0] + box[2] + 10, box[1] + 30);
	{
		int x = mouseX;
		int y = mouseY;
		toRelativeC(&x, &y);
		int x_d = (x >= 0) ? (box[2] / 2) - x : (box[2] / 2) + x;
		int y_d = (y >= 0) ? (box[3] / 2) - y : (box[3] / 2) + y;


		int dtc = ofApp::dtc(x_d, y_d);

		ofVec2f nv(x + ofRandom(-dtc, dtc), y + ofRandom(-dtc, dtc));
		for (size_t i = 0; i < rigidBox.size(); i++)
		{
			if ((nv.x - rigidBox[i].x)*(nv.x - rigidBox[i].x - rigidBox[i].w) <= 0) { nv = { 0,0 }; }
		}

		ofDrawBitmapString(
			(string)""
			+ "isRunning: " + ofToString(isRunning) + "\r\n"
			+ "mouse: ("+ ofToString(toRelativeCx(mouseX))+", "+ofToString(toRelativeCy(mouseY))+")"+"\r\n"
			+"\r\n"
			+ "dtc: " + ofToString(dtc) + "\r\n"
			
			+ "dtc: " + ofToString(dtc) + "\r\n"
			+ "x_d: " + ofToString(x_d) + "\r\n"
			+ "y_d: " + ofToString(y_d) + "\r\n"
			+ "\r\n"
			+ "mouseX: " + ofToString(x) + "\r\n"
			+ "mouseX: " + ofToString(mouseX) + "\r\n"
			+ "x_distance: " + ofToString((x - rigidBox[0].x)*(x - rigidBox[0].x - rigidBox[0].z )) + "\r\n"
			+ "\r\n"
			+ "\r\n"
			+ "p[0]: ( " + ofToString(particle[0].x) + ", " + ofToString(particle[0].y) + " )" + "\r\n"

			,
			box[0] + box[2] + 10, box[1] + 60);
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
		ofDrawCircle(toAbsoluteCx(x), toAbsoluteCy(y), dtc(x_d, y_d));
	}

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
