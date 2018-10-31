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
	box[3] = box[2];    //y幅
	//512x512の箱を定義
	p_ptc = { 100,100 };
	particle0 = 15000;
	particle = vector<ofVec2f>(particle0, p_ptc);
	for (size_t i = 0; i < particle.size(); i++)
	{
		particle[i] = { ofRandomf()*box[2] / 2.0f - 1, ofRandomf()*box[3] / 2.0f - 1 };
	}
	dnst = vector<vector<int>>(64, vector<int>(64, 0));

	window_ = { (float)ofGetWindowWidth(),(float)ofGetWindowHeight() };

	//粒子8000個の生成	  (0,0)で初期化
	//粒子は二次元ベクトルクラス(ofVec2f)を使い、vectorでまとめて管理します

	rigidBox = vector<ofVec4f>();

	rigidBox.push_back(ofVec4f(-200, -100, 100, 800));
	rigidBox.push_back(ofVec4f(50, -100, 100, 150));
	rigidBox.push_back(ofVec4f(100, 120, 500, 500));
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
			for (size_t i = 0; i < rigidBox.size(); i++)
			{
				rigidBox[i].x = rigidBox[i].x - ((int)rigidBox[i].x % (box[2] / dnst.size()));
				rigidBox[i].y = rigidBox[i].y - ((int)rigidBox[i].y % (box[3] / dnst[0].size()));
				rigidBox[i].z = rigidBox[i].z - ((int)rigidBox[i].z % (box[2] / dnst.size()));
				rigidBox[i].w = rigidBox[i].w - ((int)rigidBox[i].w % (box[3] / dnst[0].size()));
			}

			for (size_t i = 0; i < dnst.size(); i++)
			{
				for (size_t j = 0; j < dnst[0].size(); j++)
				{
					dnst[i][j] = 0;
				}
			}

			for (size_t i = 0; i < particle.size(); i++)
			{
				float x = particle[i].x;
				float y = particle[i].y;
				/**/
				//最短の上下の壁の距離を測定

				float dtc = ofApp::dtc(particle[i]);

				//float r = ofRandomf()*dtc;
				//float th = ofRandomf();
				//ofVec3f nv(x + r * cos(th * 2 * PI), y + r * sin(th * 2 * PI));
				//新しい座標候補:円

				ofVec3f nv(x + ofRandomf()*dtc, y + ofRandomf()*dtc);
				//新しい座標候補:格子

				if (rigidBox.size() >= 1)
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
								colliP.x = (x - rigidBox[i_r].x > 0) ? rigidBox[i_r].x + rigidBox[i_r].z + 1 : rigidBox[i_r].x - 1;
								colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.x);
							}
							else if ((x >= rigidBox[i_r].x && x <= rigidBox[i_r].x + rigidBox[i_r].z) && (y < rigidBox[i_r].y || y > rigidBox[i_r].y + rigidBox[i_r].w))//上下
							{
								//ddddd
								if (i == 0)state += 2;
								colliP.y = (y - rigidBox[i_r].y > 0) ? rigidBox[i_r].y + rigidBox[i_r].w + 1 : rigidBox[i_r].y - 1;
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
									colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.x);
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
									colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.x);
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
									colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.x);
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
									colliP.y = (y*(x - colliP.x) + nv.y*(colliP.x - nv.x))*1.0f / (x - nv.x);
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
				//障害物に対する当たり判定と処理

				if (abs(nv.x) > box[2] / 2.0f)nv.x = (nv.x >= 0) ? box[2] / 2.0f : -box[2] / 2.0f;
				if (abs(nv.y) > box[3] / 2.0f)nv.y = (nv.y >= 0) ? box[3] / 2.0f : -box[3] / 2.0f;
				//はみ出し判定と処理

				if (nv.x == 0 && nv.y == 0)nv = { ofRandomf()*box[2] / 2.0f, ofRandomf()*box[3] / 2.0f };
				particle[i] = nv;
				{
					nv.x = nv.x + box[2] / 2.0f;
					nv.y = nv.y + box[3] / 2.0f;

					int address_x = floor(nv.x*dnst.size() * 1.0f / box[2]);
					if (address_x < 0)address_x = 0;
					if (address_x > dnst.size() - 1)address_x = dnst.size() - 1;

					int address_y = floor(nv.y*dnst[0].size() * 1.0f / box[3]);
					if (address_y < 0)address_y = 0;
					if (address_y > dnst[0].size() - 1)address_y = dnst[0].size() - 1;

					dnst[address_x][address_y]++;
				}
			}
		}
		if (isMouseTrk)
		{
			rigidBox[0].x = toRx(mouseX);
			rigidBox[0].y = toRx(mouseY);
		}
	}
}
float ofApp::dtc(ofVec2f p)
{
	float x_d = (p.x >= 0) ? (box[2] / 2) - p.x : (box[2] / 2) + p.x;
	float y_d = (p.y >= 0) ? (box[3] / 2) - p.y : (box[3] / 2) + p.y;

	//float dtc = ((x_d + 0) * (y_d + 0))*0.001;
	float dtc = 15.0f;

	ofVec2f nv = ofVec2f(p.x + box[2] / 2.0f, p.y + box[3] / 2.0f);
	int address_x = floor(nv.x*dnst.size() * 1.0f / box[2]);
	if (address_x < 0)address_x = 0;
	if (address_x > dnst.size() - 1)address_x = dnst.size() - 1;

	int address_y = floor(nv.y*dnst[0].size() * 1.0f / box[3]);
	if (address_y < 0)address_y = 0;
	if (address_y > dnst[0].size() - 1)address_y = dnst[0].size() - 1;

	dtc = dtc * 1.0f / (1 + dnst[address_x][address_y]);
	//int dtc = (x_d + y_d)*0.2;
	//加算

	//int dtc = x_d * y_d*0.1;
	//乗算

	//int dtc = (x_d + y_d <= 100) ? dtc = 0 : dtc = 50;
	//離散
	return dtc;
}
ofVec2f ofApp::toRc(ofVec2f *v)
{
	*v = ofVec2f(toRx(v->x), toRx(v->y));
	return *v;
}
void ofApp::toRc(float* vx, float* vy)
{
	*vx = *vx - box[0] - (box[2] * 1.0f / 2);
	*vy = *vy - box[1] - (box[3] * 1.0f / 2);
}
ofVec2f ofApp::toAc(ofVec2f &v)
{
	return ofVec2f(v.x + box[0] + (box[2] * 1.0f / 2), v.y + box[1] + (box[3] * 1.0f / 2));;
}
void ofApp::toAc(float* vx, float* vy)
{
	*vx = *vx + box[0] + (box[2] * 1.0f / 2);
	*vy = *vy + box[1] + (box[3] * 1.0f / 2);
}
float ofApp::toRx(float vx)
{
	return vx - box[0] - (box[2] * 1.0f / 2);
}
float ofApp::toRy(float vy)
{
	return vy - box[1] - (box[3] * 1.0f / 2);
}
float ofApp::toAx(float vx)
{
	return vx + box[0] + (box[2] * 1.0f / 2);
}
float ofApp::toAy(float vy)
{
	return vy + box[0] + (box[3] * 1.0f / 2);
}
int p_size = 1;//粒子サイズ
int state_checker = 0;
string statee = "s: ";
//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255, 255, 255);
	//ofNoFill();
	ofFill();
	ofDrawRectangle(box[0], box[1], box[2], box[3]);
	//

	//箱の描画
	/**/
	{
		float width = box[2] * 1.0f / dnst.size();
		float height = box[3] * 1.0f / dnst[0].size();

		for (size_t i = 0; i < dnst.size(); i++)
		{
			for (size_t j = 0; j < dnst[0].size(); j++)
			{
				ofSetColor(250 - 3 * dnst[i][j], 250 - 3 * dnst[i][j], 250 - 3 * dnst[i][j]);
				ofDrawRectangle(toAx(i*width - box[2] / 2.0f), toAx(j*height - box[3] / 2.0f), width, height);
			}
		}
		//濃度
	}

	ofSetColor(200, 200, 200);
	for (size_t i_r = 0; i_r < rigidBox.size(); i_r++)
	{
		ofDrawRectangle(toAx(rigidBox[i_r].x), toAy(rigidBox[i_r].y), rigidBox[i_r].z, rigidBox[i_r].w);
	}

	ofSetColor(0, 0, 0);
	for (size_t i = 0 + 1; i < particle.size(); i++)
	{
		ofDrawRectangle((int)toAx(particle[i].x - (p_size / 2)), (int)toAy(particle[i].y - (p_size / 2)), p_size, p_size);
		/*
		float r = ofRandom(0,dtc);
		float th = ofRandomf();
		ofDrawRectangle(toAx(1.0f*r*cos(th * 3.0f * PI)+100), toAy(1.0f*r*sin(th * 3.0f * PI)), p_size, p_size);*/
	}
	ofSetColor(255, 0, 0);
	ofDrawRectangle(toAx(particle[0].x - p_size), toAy(particle[0].y - p_size), p_size * 2, p_size * 2);
	ofNoFill();
	{
		float x = particle[0].x;
		float y = particle[0].y;

		int dtcc = ofApp::dtc(particle[0]);
		ofDrawCircle(toAx(x), toAy(y), dtcc);
	}
	//粒子関連の描画

	ofSetColor(233, 152, 206);
	ofNoFill();
	ofDrawCircle(toAx(p_ptc.x), toAy(p_ptc.y), 12);
	ofFill();
	ofDrawCircle(toAx(p_ptc.x), toAy(p_ptc.y), 5);
	ofSetColor(0, 0, 0);

	{
		ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", box[0] + box[2] + 50, box[1] + 15);
		ofDrawBitmapString("step: " + ofToString(time), box[0] + box[2] + 50, box[1] + 30);

		float x = mouseX;
		float y = mouseY;
		toRc(&x, &y);

		if (isRunning && state != 10)
		{
			statee += ", \r\n" + ofToString(state);
			state_checker++;
		}

		ofDrawBitmapString(
			(string)""
			+ "isRunning: " + ofToString(isRunning) + "\r\n"
			+ "mouse: (" + ofToString(toRx(mouseX)) + ", " + ofToString(toRy(mouseY)) + ")" + "\r\n"
			+ "\r\n"
			+ "\r\n"
			+ "x_d: " + ofToString((x - rigidBox[0].x)*(x - rigidBox[0].x - rigidBox[0].z)) + "\r\n"
			+ "\r\n"
			+ "\r\n"
			+ "p[0]: ( " + ofToString(particle[0].x) + ", " + ofToString(particle[0].y) + " )" + "\r\n"
			+ "staC: " + ofToString(state_checker) + "\r\n"
			+ statee + "\r\n",
			box[0] + box[2] + 50, box[1] + 60);
	}
	//文字の描画
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'z')isRunning = !isRunning;
	//"z"を押すと実行/一時停止します
	if (key == 'a')
	{
		//particle = vector<ofVec2f>(particle0, p_ptc);
		for (size_t i = 0; i < particle.size(); i++)
		{
			particle[i] = { ofRandomf()*box[2] / 2.0f - 1, ofRandomf()*box[3] / 2.0f - 1 };
		}
		dnst = vector<vector<int>>(32, vector<int>(32, 0));
		isRunning = false;
	}
	//"a"を押すとリスタートします
	if (key == 'x')isMouseTrk = !isMouseTrk;
	//"x"でマウストラックon/off
	if (key == 's')statee = "ns: ";
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
	if (button == 0)
	{
		particle[0].x = toRx(mouseX);
		particle[0].y = toRy(mouseY);
	}
	if (button == 2)
	{
		p_ptc.x = toRx(mouseX);
		p_ptc.y = toRy(mouseY);
	}
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
	/*
	float k = ((float)h *1.0f / window_.y);
	box[2] *= k;    //x幅
	box[3] *= k;    //y幅

	for (size_t i = 0 + 1; i < particle.size(); i++)
	{
		particle[i] *= k;
	}
	for (size_t i_r = 0; i_r < rigidBox.size(); i_r++)
	{
		rigidBox[i_r] *= k;
	}

	window_ = { (float)w,(float)h };*/
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}