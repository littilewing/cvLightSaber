#include "ofApp.h"

#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){

    originCamWidth = 1280;
    originCamHeight = 720;

    
    camWidth = 320;
    camHeight = (int)(originCamHeight /(originCamWidth/camWidth));
    

    displayscale = ofGetScreenWidth()/camWidth;
    
    ofLog(OF_LOG_NOTICE,"w:%f",displayscale);
    
    ofBackground(0, 0, 0);
//    ofSetFrameRate(30);
    ofShowCursor();
    
    
//  ofSetVerticalSync(true);
    
//  cameraDeviceID(use another camera )
  vidGrabber.setDeviceID(0);
    
//    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(originCamWidth, originCamHeight);
    
    fbo.allocate(camWidth, camHeight, GL_RGB);
    
    
    //それぞれの映像情報の大きさを指定してあげます。
    colorImgHSV.allocate(camWidth, camHeight);
    
    hueImg.allocate(camWidth, camHeight);
    satImg.allocate(camWidth, camHeight);
    briImg.allocate(camWidth, camHeight);
    
    reds.allocate(camWidth, camHeight);
    
    //二値画像を作るための配列の大きさを指定
    colorTrackedPixelsRed =new unsigned char [camWidth*camHeight];
    
    //二値画像の大きさ
    trackedTextureRed.allocate(camWidth, camHeight, GL_LUMINANCE);
    


    
    
    gui.setup();
    gui.setPosition(camWidth*displayscale - 40,0);
    gui.add(smooth.setup("smooth",5,1,91));
    gui.add(threshold.setup("threshold",0,0,256));
    
    gui.add(satMin.setup("satMin",24,0,255));
    gui.add(satMax.setup("satMax",200,0,255));
    gui.add(hueMin.setup("hueMin",12,0,255));
    gui.add(hueMax.setup("hueMax",12,0,255));
    
    gui.add(mirror.setup("mirror",true));
    
    gui.add(track1_hue.setup("trackhue1",0,0,255));
    gui.add(track1_sat.setup("tracksat1",0,0,255));
    gui.add(track1_bri.setup("trackbri1",0,0,255));
    gui.add(track2_hue.setup("trackhue2",0,0,255));
    gui.add(track2_sat.setup("tracksat2",0,0,255));
    gui.add(track2_bri.setup("trackbri2",0,0,255));

    
    gui.loadFromFile("settings.xml");
    
    
    

    
    LightSaber l1,l2;
    ofColor c1,c2;
    
    c1.set(200, 200, 200);
    c2.set(200,20,20);
    

    l1.setup("LightSaber3.png",c1);
    l1.track.hue = track1_hue;
    l1.track.sat = track1_sat;
    l1.track.bri = track1_bri;
    

    l2.setup("LightSaber3.png",c2);
    l2.track.hue = track2_hue;
    l2.track.sat = track2_sat;
    l2.track.bri = track2_bri;
    
    
    ls.push_back(l1);
    ls.push_back(l2);
    
    

    
    SaberDelay _s;
    _s.pos1 = ofVec2f(0,0);
    _s.pos2 = ofVec2f(0,0);
    
    lostPosition.push_back(_s);
    lostPosition.push_back(_s);
    
    lostCount.push_back(0);
    lostCount.push_back(0);
    
    box.setup(camWidth * displayscale,camHeight * displayscale);
    for(int i = 0;i  <ls.size();i++){
        box.edges.push_back( ofPtr<ofxBox2dEdge>(new ofxBox2dEdge));
        box.edges.back().get()-> create(box.box2d.getWorld());
        box.edges.back().get()->setPhysics(0.0, 0.5, 0.5);
    }
    
    traceid = 1;
    


    
    
    
    // shader
    string shaderProgram = STRINGIFY(
                                     uniform sampler2DRect tex0; // これから描画するテクスチャが入っている
                                     uniform float brightness; // メインプログラムから渡された変数を受け取る
                                     
                                     void main (void){
                                         
                                         vec2 pos = gl_TexCoord[0].st; // テクスチャ上の座標を取得する
                                         
                                         float r = texture2DRect(tex0, pos).r;
                                         float g = texture2DRect(tex0, pos).g;
                                         float b = texture2DRect(tex0, pos).b;

                                         float a = texture2DRect(tex0, pos).a;
//                                         float h = texture2DRect(tex0,pos).hue;
  //                                       float s = texture2DRect(tex0,pos).sat;
    //                                     float b = texture2DRect(tex0,pos).bri*2;
                                         
                                         vec4 color = vec4(r,g,b,a);
                                         //vec4 color = vec4(h,s,b,a);

                                         
                                         gl_FragColor = color *brightness; // gl_FragColorの値が最終的に画面に出力される画素値になる。


                                     }
                                     );
    
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    shader.linkProgram();
    
    
}

//--------------------------------------------------------------
void ofApp::update(){


    vidGrabber.update();    //camera update
    
    if (vidGrabber.isFrameNew())
    {
        fbo.begin();
        if(mirror){
            vidGrabber.draw(fbo.getWidth(), 0, -fbo.getWidth(), fbo.getHeight());
        }
        else{
         vidGrabber.draw(0, 0, fbo.getWidth(), fbo.getHeight());
        }
        fbo.end();
        
        ofPixels pix;
        fbo.readToPixels(pix);
        


        colorImgHSV.setFromPixels(pix);
        colorImgHSV.convertRgbToHsv();
        
        //colorImgHSV.threshold( threshold );
        
        //色相、彩度、明度にマッピング
        colorImgHSV.convertToGrayscalePlanarImages(hueImg, satImg, briImg);

        hueImg.flagImageChanged();
        satImg.flagImageChanged();
        briImg.flagImageChanged();
        


    }
    

    
    //ピクセルの配列をそれぞれに作成
    unsigned char * huePixels = hueImg.getPixels();
    unsigned char * satPixels = satImg.getPixels();
    unsigned char * briPixels = briImg.getPixels();
    
    //ピクセルの数
    int nPixels = camWidth*camHeight;
    
    
    for(int t=0;t<ls.size();t++){
        
        if(traceid < t)
            break;
        
        //ピクセルの色が指定した色と色相と彩度が近ければ、
        //colorTrackedPixelsRedに255を、遠ければ0を代入。
        for (int i=0; i<nPixels; i++) {
            if ( (huePixels[i]>=ls[t].track.hue - hueMin && huePixels[i] <= ls[t].track.hue + hueMax) &&
                (satPixels[i]>=ls[t].track.sat-satMin && satPixels[i]<=ls[t].track.sat+satMax)){
                colorTrackedPixelsRed[i] = 255;
            }else {
                colorTrackedPixelsRed[i]=0;
            }
            
        }
        
        //colorTrackedPixelsRedをもとにredsを作成
        //redsは輪郭線を求めるためだけにあるのかな？
        reds.setFromPixels(colorTrackedPixelsRed, camWidth, camHeight);
        
        //輪郭線を見つける
        //finderRed.findContours(reds, 10, nPixels/3, 2, false, true);
        finderRed.findContours(reds, 10, nPixels/3, 3, false, true);
        
        //colorTrackedPixelsRedをもとにtrackedTextureRedを作成
        //これが二値画像になってるっぽい
        trackedTextureRed.loadData(colorTrackedPixelsRed,
                                   camWidth, camHeight, GL_LUMINANCE);
        
        //追跡する色の位置を中心にあわせる
        if (finderRed.blobs.size()>1) {
            
            SaberDelay d;
            d.pos1.set(ofVec2f(finderRed.blobs[0].centroid.x*displayscale,finderRed.blobs[0].centroid.y*displayscale));// = p;
            d.pos2.set(ofVec2f(finderRed.blobs[1].centroid.x*displayscale,finderRed.blobs[1].centroid.y*displayscale));// = p;
            
            //縦に構えた時は自動的にONにする
            if(ls[t].display == false && abs(d.pos1.x - d.pos2.x) < 20){
                ls[t].show();
            }

            //見失った時の復活処理
            if(lostCount[t] > 0){

                //残像を最新の位置と、見失う前の位置を自動補完する
                SaberDelay _l;
                _l.pos1.set((d.pos1.x - lostPosition[t].pos1.x) / ls[t].zanzouCnt,
                            (d.pos1.y - lostPosition[t].pos1.y) / ls[t].zanzouCnt);
                _l.pos2.set((d.pos2.x - lostPosition[t].pos2.x) / ls[t].zanzouCnt,
                            (d.pos2.y - lostPosition[t].pos2.y) / ls[t].zanzouCnt);
                
                
                for(int i = 0;i < ls[t].delaypos.size();i++){
                    ls[t].delaypos[i].pos1.x +=  (_l.pos1.x * i);
                    ls[t].delaypos[i].pos1.y +=  (_l.pos1.y * i);
                    ls[t].delaypos[i].pos2.x +=  (_l.pos2.x * i);
                    ls[t].delaypos[i].pos2.y +=  (_l.pos2.y * i);
                }
                
                lostCount[t] =0;
                
            }

            ls[t].update(d.pos1, d.pos2);
            
            //物理演算（当たり判定をライトセーバーにつける）

            if(ls[t].display){
                box.edges[t].get()->clear();
                box.edges[t].get()->addVertex(d.pos1);
                box.edges[t].get()->addVertex(d.pos2);
                box.edges[t].get()->addVertex(d.pos1);
                box.edges[t].get()->setPhysics(0.0, 0.5, 0.5);

                box.edges[t].get()->create(box.box2d.getWorld());
                
            }
 


        }else{
            
            //見つからなかった場合、
            //最後の位置に表示する
            
            if(ls[t].display == true ){
                if(lostCount[t] == 0){
                    lostPosition[t].pos1 =ls[t].delaypos[ls[t].delaypos.size()].pos1;
                    lostPosition[t].pos2 =ls[t].delaypos[ls[t].delaypos.size()].pos2;
                    
                    ls[t].update(lostPosition[t].pos1,lostPosition[t].pos2);
                
                }
                
                if(lostCount[t] < 10)
                    lostCount[t] +=1;//;//= 20;
                else{
                    ls[t].delaypos.clear();
                    ls[t].hide();
                    box.edges[t].get()->clear();
                }
            }
            else{
                ls[t].delaypos.clear();
                box.edges[t].get()->clear();
            }
        }

        
    }//複数化  endfor

    /*
     
     LineAは[x1,y1]-[x2,y2]、LineBは[x3,y3]-[x4,y4]とします。
     直線の交差ではなく線分の交差を調べるのは媒介変数を利用して判定します。
     r =((y4-y3)(x3-x1)-(x4-x3)(y3-y1))/((x2-x1)(y4-y3)-(y2-y1)(x4-x3))
     s =((y2-y1)(x3-x1)-(x2-x1)(y3-y1))/((x2-x1)(y4-y3)-(y2-y1)(x4-x3))
     
     */
    
    if (ls[0].display && ls[1].display){
        
        int z = ls[0].zanzouCnt-1;
        //    float r = ls[1].delaypos[z].pos2.y -
        
        SaberDelay a,b;
        a = ls[0].delaypos[z];
        b = ls[1].delaypos[z];
        float r = (( b.pos2.y - b.pos1.y)*(b.pos1.x-b.pos1.x)-(b.pos2.x-b.pos1.x)*(b.pos1.y-a.pos1.y))/
        ((a.pos2.x-a.pos1.x)*(b.pos2.y-b.pos1.y)-(a.pos2.y-a.pos1.y)*(b.pos2.x-b.pos1.x));
        
        float s = ((a.pos2.y-a.pos1.y)*(b.pos1.x-a.pos1.x)-(a.pos2.x-a.pos1.x)*(b.pos1.y-a.pos1.y))/
        ((a.pos2.x-a.pos1.x)*(b.pos2.y-b.pos1.y) -(a.pos2.y-a.pos1.y)*(b.pos2.x-b.pos1.x));
        
        
        //    0＜r≦1 & 0＜s≦1 なら交差する。
        
        if(0 < r && r <=1 && 0 < s && s <=1){
            ofLog(OF_LOG_NOTICE,"KOUSA");
            
            if(hit == false){
                ls[0].hit();
                hit = true;
            }
        }
        else{
//                    ofLog(OF_LOG_NOTICE,"Z:%f:%f",r,s);
            hit = false;
        }
    
    }

    
    box.update();

//  ofLog(OF_LOG_NOTICE,"A1");
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //背景色を指定
    ofBackground(0, 0, 0);
    

    ofSetHexColor(0xffffff);

    
//    ofPushMatrix();
  //  ofScale(2.0, 2.0);
    shader.begin();
    shader.setUniform1f("brightness", 0.6); // 暗く
    //shader.setUniform1f("brightness", 1.5 ); // 明るく
    
    //sampleImage.draw(0, 0);
    
    
    float vr = (1440.0/originCamWidth);
    if(mirror){
        //元映像を表示

        vidGrabber.draw(originCamWidth *vr,0,-originCamWidth * vr,originCamHeight * vr);
        //HSV系に変換したものを表示
//        colorImgHSV.draw(camWidth*displayscale, 0,-camWidth*displayscale,camHeight*displayscale);
        
    }
    else{
       
        //元映像を表示
        vidGrabber.draw(0,0,originCamWidth * vr,originCamHeight * vr);
        
        //HSV系に変換したものを表示
    //    colorImgHSV.draw(camWidth, 0,camWidth/2,camHeight/2);

    }
    


    for(int t=0;t<ls.size();t++){
        if(ls[t].delaypos.size() > 0 && ls[t].display )
            ls[t].draw();
    }
    shader.end();

    
    box.draw();
    
    if(showConsole){
        //二値画像を表示
        trackedTextureRed.draw(0, originCamHeight * vr,camWidth/4,camHeight/4);
        
        //二値画像の方に輪郭線表示
        finderRed.draw(camWidth/4,originCamHeight * vr,camWidth/4,camHeight/4);

        
        gui.draw();
        
        char tempStr1[255];
        sprintf(tempStr1, "trackid:%d e:%lu",traceid,box.edges.size());
        ofDrawBitmapString(tempStr1, camWidth * displayscale, 300);
        


    }

    
    string str = "framerate is ";
    str += ofToString(ofGetFrameRate(), 2)+"fps";
    ofSetWindowTitle(str);
    //set the window title to "framerate is 45.30fps"
    


    
 


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    
    if (key == '1'){
        traceid = 0;
        ls[traceid].hide();
        
    }else if (key == '2'){
        traceid = 1;

        ls[traceid].hide();

        
    }else if(key == 'h'){
        
        showConsole = (showConsole ? false : true);
    }
    else if(key == ' ' ){
        
        if(ls[traceid].display)
            ls[traceid].hide();
        else{
            if(traceid == 0){
                ls[traceid].track.hue = track1_hue;// = huePixels[x+(y*hueImg.width)];
                ls[traceid].track.sat = track1_sat;// = satPixels[x+(y*satImg.width)];
                ls[traceid].track.bri = track1_bri;// = briPixels[x+(y*briImg.width)];
                
            }
            else if(traceid == 1){
                ls[traceid].track.hue = track2_hue;// = huePixels[x+(y*hueImg.width)];
                ls[traceid].track.sat = track2_sat;// = satPixels[x+(y*satImg.width)];
                ls[traceid].track.bri = track2_bri;// = briPixels[x+(y*briImg.width)];
                
            }
            
            ls[traceid].show();
        }
    }
    else if(key == 'm'){
        if(mirror)
            mirror = false;
        else
            mirror = true;
    }
    

    box.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    unsigned char * huePixels = hueImg.getPixels();
    unsigned char * satPixels = satImg.getPixels();
    unsigned char * briPixels = briImg.getPixels();
    
    float vr = 1440.0/camWidth;
    
    //クリックした場所の色を追跡する色に設定。
    x=MIN( x/vr,hueImg.width-1);
    y=MIN( y/vr,hueImg.height-1);
        
    
    if (button==0) {
        
        if(traceid == 0){
            ls[traceid].track.hue = track1_hue = huePixels[x+(y*hueImg.width)];
            ls[traceid].track.sat = track1_sat = satPixels[x+(y*satImg.width)];
            ls[traceid].track.bri = track1_bri = briPixels[x+(y*briImg.width)];
            ls[traceid].track.pos = ofVec2f(x,y);
            
        }
        else if(traceid == 1){
            ls[traceid].track.hue = track2_hue = huePixels[x+(y*hueImg.width)];
            ls[traceid].track.sat = track2_sat = satPixels[x+(y*satImg.width)];
            ls[traceid].track.bri = track2_bri = briPixels[x+(y*briImg.width)];
            ls[traceid].track.pos = ofVec2f(x,y);
            
        }
        
        
        ls[traceid].show();
    }
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    ofShowCursor();

//    self.displayscale = w/self.camWidth;
    displayscale = (float)w /camWidth;
    


    ofLog(OF_LOG_NOTICE,"RESIZE1:%d:%f:%f",w,ofGetScreenWidth(),displayscale);
    ofLog(OF_LOG_NOTICE,"RESIZE2:%f:%f",ofGetScreenWidth(),camWidth);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

