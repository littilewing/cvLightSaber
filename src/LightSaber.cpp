//
//  LightSaber.cpp
//  myColor4
//
//  Created by 林知樹 on 2015/01/20.
//
//

#include "LightSaber.h"

void LightSaber::setup(string filename,ofColor _color){
    
    
    display = false;
//    scale = 2.0;

    sbimage.loadImage(filename);
    
    color = _color;
    
    crossimage.loadImage("LightSaber4.png");
//    sbimage.setAnchorPercent(0.5, 0.5);
    
    mySoundLoop.loadSound("./sound/idle1.wav");
    mySoundLoop.setLoop(true); //ループ再生をONに
    
    

    mySound.loadSound("./sound/on0.wav");
    mySound.play();
    
    hitSound.loadSound("./sound/strike2.wav");
    

    
}

void LightSaber::update(ofVec2f pos1, ofVec2f pos2){
    
    
    SaberDelay d;
    d.pos1 = pos1;
    d.pos2 = pos2;
    
    delaypos.push_back(d);
    
    if(delaypos.size() >= zanzouCnt){
        delaypos.erase(delaypos.begin());
    }
    
    if(display){
        int e = delaypos.size();

        
        float dur = abs(delaypos[e].pos1.x - delaypos[0].pos1.x);
        
        if(dur > 100 && !mySound.getIsPlaying()){
//            mySound.loadSound("./sound/strike2.wav");
            mySound.loadSound("./sound/swing2.wav");
            mySound.play();
            
                 //   ofLog(OF_LOG_NOTICE,"dur2:%f",dur);
            
        }
        else if(dur> 10 && !mySound.getIsPlaying()){
//            mySound.loadSound("./sound/swing2.wav");
  //          mySound.play();
               //     ofLog(OF_LOG_NOTICE,"dur1:%f",dur);
        }

    }
    

    
}
void LightSaber::drawSaber(ofVec2f pos1, ofVec2f pos2,float alpha){
    

    //画像でライトセーバー

    float cenx =pos2.x;// -(pos2.x -pos1.x);
    float ceny =pos2.y;// -(pos2.y -pos1.y);
    float dis =sqrt(cenx*cenx + ceny*ceny);
    float r =ofDist(pos1.x, pos1.y, pos2.x, pos2.y) /200;
    double radian = atan2(pos2.x - pos1.x,pos2.y-pos1.y);

    sbimage.setAnchorPercent(0.5, 0.5);

    

    ofEnableAlphaBlending();
    
//    ofEnableBlendMode(OF_BLENDMODE_ADD);
//    ofSetColor(200,200,200,alpha-10);
    ofSetColor(color, alpha-10);
    
//    ofColor c;
  //  c.setHsb( 0,200,200 );
    //ofSetColor(c, alpha-10);
    
    
    //ofSetHexColor(0x0000ff);


//    ofCircle(pos1.x, pos1.y, 10);
//    ofCircle(pos2.x, pos2.y, 10);


    

    ofPushMatrix();
    ofTranslate(cenx, ceny);
    
    ofScale(r*scale, r*scale);
    ofRotate(-radian * 180.0 /PI +180);
    
    sbimage.draw(0, 0);
    
    ofPopMatrix();
    

    
    ofPushMatrix();
    ofTranslate(pos1.x, pos1.y);
    ofScale(r*scale, r*scale);
    ofRotate((-radian) * 180.0 /PI);
    sbimage.draw(0, 0);
    
    ofPopMatrix();


    /*
    //cross
    ofPushMatrix();
    if(pos1.y > pos1.y)
        ofTranslate(pos2.x, pos2.y);
    else
        ofTranslate(pos1.x, pos1.y);
    
    ofScale(r*scale, r*scale);
    ofRotate(-radian * 180.0 /PI +90);
    
    crossimage.draw(-110, -100);
    ofPopMatrix();
     */
    
    //ofDisableBlendMode();
    ofDisableAlphaBlending();
    
}

void LightSaber::draw(){
    
//    return false;
    if(display){
        float alphaP = 200 /(delaypos.size() ? delaypos.size() : 1);
        
        
        for(int i =0; i<= delaypos.size(); i++){
            drawSaber(delaypos[i].pos1, delaypos[i].pos2, alphaP * i);
        }
    }
}

void LightSaber::show(){


    
    mySound.play();
    mySoundLoop.play();
    
    display = true;
    
}
void LightSaber::hide(){
    mySound.loadSound("./sound/off0.wav");
    mySound.play();
    mySoundLoop.stop();

    display = false;
    delaypos.clear();
}

void LightSaber::hit(){
    
    ofLog(OF_LOG_NOTICE,"hit!");
    hitSound.play();
    
}




