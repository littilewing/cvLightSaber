#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"

#include "LightSaber.h"
#include "BoxObj.h"



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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    
    //camera
    ofVideoGrabber vidGrabber;
    ofFbo fbo;
    
    //カメラの幅と高さ
    int camWidth;
    int camHeight;
    int originCamWidth;
    int originCamHeight;
    float displayscale;
    bool showConsole = true;
    

    
    //HSV系に変換した映像情報
    ofxCvColorImage colorImgHSV;
    
    //HSV系の色相、彩度、明度のマップ
    ofxCvGrayscaleImage hueImg;
    ofxCvGrayscaleImage satImg;
    ofxCvGrayscaleImage briImg;
    
    //色を追跡して輪郭を出すための映像情報
    ofxCvGrayscaleImage reds;
    
    //もとの映像情報のピクセルの彩度と明度が
    //指定した色に近ければ255を代入、遠ければ0を代入
    unsigned char * colorTrackedPixelsRed;
    
    //二値画像
    ofTexture trackedTextureRed;
    

    ofxCvContourFinder finderRed;
    
    
    static int const NUM = 2;
    
    /*
     gui
     */
    ofxPanel gui;
    ofParameterGroup displayset;
    ofParameterGroup colorset;
    ofParameterGroup rangeset;
    
    ofParameter<int> smooth;
    ofParameter<int> threshold;
    ofParameter<bool> mirror;
    
    ofParameter<int> hueMin[NUM];
    ofParameter<int> hueMax[NUM];
    ofParameter<int> satMin[NUM];
    ofParameter<int> satMax[NUM];

    ofParameter<int> track_hue[NUM];
    ofParameter<int> track_sat[NUM];
    ofParameter<int> track_bri[NUM];

    
    
    /*
     ライトセーバー関連
     */
    vector <LightSaber> ls;
    vector<SaberDelay> lostPosition;
    vector<int>lostCount;
    
    //複数化
    vector <Color> traces;
    int traceid;
    
    bool hit = false;


    BoxObj box;
    ofShader shader;
    
    
    

    
    
};
