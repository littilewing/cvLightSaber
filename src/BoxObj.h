//
//  BoxObj.h
//  myColor4
//
//  Created by 林知樹 on 2015/01/20.
//
#include "ofMain.h"
#include "ofxBox2d.h"





// A simple little Data class. This is were
// you can store anything you want.
class Data {
public:
    ofColor color;
    string  name;
    int		id;

};


// A Custom Particle extedning the box2d circle
class CustomParticle : public ofxBox2dCircle {
    
public:
    ofQuaternion curRot;
    
    ofImage img;
    
    void setupTheCustomData() {
        
        static int colors[] = {0xcae72b, 0xe63b8f, 0x2bb0e7};
        static string abc   = "abcdefghijklmnopqrstuvwxyz";
        
        // we are using a Data pointer because
        // box2d needs to have a pointer not
        // a referance
        setData(new Data());
        Data * theData = (Data*)getData();
        
        theData->id = ofRandom(0, 100);
        theData->name += abc[(int)ofRandom(0, abc.size())];
        theData->color.setHex(colors[(int)ofRandom(0, 3)]);
        
        printf("setting the custom data!\n");
        
        img.loadImage("Imperial-Probe-Droid-icon.png");
        
    }
    
    void draw() {
        Data* theData = (Data*)getData();
        if(theData) {
            
            // Evan though we know the data object lets just
            // see how we can get the data out from box2d
            // you would use this when using a contact listener
            // or tapping into box2d's solver.

            float radius = getRadius();
            ofPushMatrix();
            ofTranslate(getPosition());
            ofRotateZ(getRotation());
            ofSetColor(theData->color);
            ofFill();
            ofCircle(0, 0, radius);
            
            float textSize = radius/10;
            ofPushMatrix();
            ofScale(textSize, textSize);
            ofSetColor(255);
            ofDrawBitmapString(theData->name, -textSize/2, textSize);
            ofPopMatrix();
            
            img.draw(-256,-256,512,512);
            ofPopMatrix();
            

            
            /*
            ofNoFill();
            ofSetColor(theData->color);
            float radius = getRadius();
            //translate so that 0,0 is the center of the screen
            ofPushMatrix();
            ofTranslate(getPosition());
            //Extract the rotation from the current rotation
            ofVec3f axis;
            float angle;
            curRot.getRotate(angle, axis);
            
            //apply the quaternion's rotation to the viewport and draw the sphere
            ofRotate(angle, axis.x, axis.y, axis.z);
            ofDrawSphere(0, 0, 0, radius);
            
            ofPopMatrix();
                         */
            
            
        }
    }
    
    
};

class BoxObj {
    
public:
    void setup(int camWidth,int camHeight);
    void update();
    void draw();
    
    void keyPressed(int key);
    
    // this is the function for contacts
    void contactStart(ofxBox2dContactArgs &e);
    void contactEnd(ofxBox2dContactArgs &e);
    void createEnemy();
    
        
    //
    //カメラの幅と高さ
    int camWidth;
    int camHeight;

    ofxBox2d    box2d;			  //	the box2d world
    vector		<ofPtr<CustomParticle> > 	circles;		  //	default box2d circles
    
    ofPolyline                              drawing;		  //	we draw with this first
    
    
    
    vector <ofPtr<ofxBox2dEdge> > edges;
//    vector <ofPtr<ofxBox2dPolygon> > edges;

//    ofxBox2dEdge                            edgeLine;		  //	the box2d edge/line shape (min 2 points)



    //current state of the rotation
    ofQuaternion curRot;

    
    ofSoundPlayer hitSound; //ofSoundクラスをインスタンス化

    int upcount;
};
