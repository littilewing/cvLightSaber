#include "ofMain.h"



//色の基本的な情報を持ったクラスを作ります。
class Color {
public:
    float hue, sat, bri;//抽出する色
    ofVec2f     pos;    //マウスクリックされた位置
};


class SaberDelay{
public:
    ofVec2f pos1;
    ofVec2f pos2;
    
};

class LightSaber {


//    int state;  //状態
    
    ofImage sbimage;
    ofImage crossimage;
    ofColor color;
    
public:
    LightSaber(){};
    
    void setup(string filename,ofColor _color);
    void update(ofVec2f pos1, ofVec2f pos2);
    void draw();
    
    void drawSaber(ofVec2f pos1, ofVec2f pos2,float alpha);
    
//    int saberid;    //ID使うのかな？
    
    Color track;
    
    float scale     =   1.0;//2.0; // サイズ
    int zanzouCnt   =   8;//残す残像の数

    
    void show();
    void hide();
    void hit();
    
    vector <SaberDelay> delaypos;//位置を格納する配列（残像も残すため配列にする）
    bool display;//表示フラグ
    
    ofSoundPlayer mySound; //ofSoundクラスをインスタンス化
    ofSoundPlayer hitSound; //ofSoundクラスをインスタンス化
    ofSoundPlayer mySoundLoop; //ofSoundクラスをインスタンス化


    
    
    
};