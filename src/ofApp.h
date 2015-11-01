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
    
    //カメラの幅と高さ
    int camWidth;
    int camHeight;
    float displayscale;
    bool showConsole = true;
    
    //もともとの映像情報
    ofxCvColorImage colorImg;
    
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
    
    //輪郭を判別してくれるメチャクチャ便利なやつです。
    ofxCvContourFinder finderRed;
    
    
    
    
    /*
     gui
     */
    ofxPanel gui;
    ofxIntSlider smooth;
    ofxIntSlider threshold;
    
    
    ofxIntSlider hueMin;
    ofxIntSlider hueMax;
    ofxIntSlider satMin;
    ofxIntSlider satMax;
    
    ofxToggle mirror;
    ofxFloatSlider track1_hue;
    ofxFloatSlider track1_sat;
    ofxFloatSlider track1_bri;
    ofxFloatSlider track2_hue;
    ofxFloatSlider track2_sat;
    ofxFloatSlider track2_bri;
    
    
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
