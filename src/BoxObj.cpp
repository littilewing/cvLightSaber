//
//  BoxObj.cpp
//  myColor4
//
//  Created by 林知樹 on 2015/01/20.
//
//

#include "BoxObj.h"



void BoxObj::setup(int cW,int cH){
    
    camWidth = cW;
    camHeight = cH;
    

    box2d.init();
    box2d.enableEvents();   // <-- turn on the event listenerc
    box2d.setGravity(100, 0);
    box2d.createGround();
    box2d.setFPS(30.0);
    box2d.registerGrabbing();
    
    // register the listener so that we get the events
    ofAddListener(box2d.contactStartEvents, this, &BoxObj::contactStart);
    ofAddListener(box2d.contactEndEvents, this, &BoxObj::contactEnd);


    
  hitSound.loadSound("./sound/strike2.wav");
    
//    upcount = 0;
  //          hitSound.play();
}
void BoxObj::update(){
    box2d.update();
    ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
    
    
    upcount++;
    if(upcount > 60){
     //  createEnemy();
        upcount = 0;
    }

    
}
void BoxObj::draw(){

    for(int i=0; i<circles.size(); i++) {
//        ofFill();
  //      ofNoFill();
//        ofSetHexColor(0x90d4e3);
        circles[i].get()->draw();
    }


//    ofNoFill();
  //  ofSetHexColor(0x444342);

    for(int i = 0;i <edges.size();i++){
    //   edges[i].get()->updateShape();
  //      edges[i].get()->draw();

    }

    

}

//--------------------------------------------------------------
void BoxObj::keyPressed(int key) {
    
    if(key == 'c') {
        createEnemy();
    }
    
    
}
void BoxObj::createEnemy(){
    float r = ofRandom(4, 20);		// a random radius 4px - 20px
    /*
    circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle));
    circles.back().get()->setPhysics(3.0, 100, 0.1);
    circles.back().get()->setup(box2d.getWorld(), ofRandom(100, camWidth), 0, r);
    */
    
    ofPtr<CustomParticle> p = ofPtr<CustomParticle>(new CustomParticle);
    circles.push_back(ofPtr<CustomParticle>(new CustomParticle));

    circles.back().get()->setPhysics(60.0, 10, 20);
//    circles.back().get()->setup(box2d.getWorld(), ofRandom(100, camWidth), 0, r);
    circles.back().get()->setup(box2d.getWorld(), 0, ofRandom(camHeight/3, camHeight*2/3), r);
    circles.back().get()->setupTheCustomData();
    

}

//--------------------------------------------------------------
void BoxObj::contactStart(ofxBox2dContactArgs &e) {
    
//    ofLog(OF_LOG_NOTICE,"contactStart");
    
    
  //          hitSound.play();
    if(e.a != NULL && e.b != NULL) {
        
        // if we collide with the ground we do not
        // want to play a sound. this is how you do that
        
        //    hitSound.play();

        if(e.a->GetType() == b2Shape::e_edge && e.b->GetType() == b2Shape::e_circle) {
            
            hitSound.play();
/*
            SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
            SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
            
            if(aData) {
                aData->bHit = true;
                sound[aData->soundID].play();
            }
            
            if(bData) {
                bData->bHit = true;
                sound[bData->soundID].play();
            }
 */
        }
    }
}

//--------------------------------------------------------------
void BoxObj::contactEnd(ofxBox2dContactArgs &e) {
    
    
    if(e.a != NULL && e.b != NULL) {
        
//        hitSound.play();
        
        /*
        SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
        SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
        
        if(aData) {
            aData->bHit = false;
        }
        
        if(bData) {
            bData->bHit = false;
        }
         */
    }
}

