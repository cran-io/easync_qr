#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetWindowTitle("Easync Media - QR Slate");
    ofBackground(25);
    EasyncVideo test("test.mov");
    videos.push_back(test);
    
    current=0;
    //videos[current].video.play();
    for(int i=0;i<THRESH_IMAGES;i++)
        thresh[i].allocate(videos[current].video.getWidth(), videos[current].video.getHeight(), OF_IMAGE_GRAYSCALE);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(videos[current].processed){
        unsigned int next=current;
        do{
            next=(next+1)%videos.size();
        }while(videos[next].processed && next!=current);
        if(next!=current){
            current=next;
            //videos[current].video.play();
            for(int i=0;i<THRESH_IMAGES;i++)
                thresh[i].allocate(videos[current].video.getWidth(), videos[current].video.getHeight(), OF_IMAGE_GRAYSCALE);
        }
    }
    
    videos[current].video.nextFrame();
    videos[current].video.update();
    if(!videos[current].processed){
        if(videos[current].video.isFrameNew()) {
            for(int i=0;i<THRESH_IMAGES;i++){
                ofxCv::convertColor(videos[current].video.getPixelsRef(), thresh[i], CV_RGB2GRAY);
                float thresholdValue = ofMap(i, 0, THRESH_IMAGES, 0, 255);
                cout<<thresholdValue<<endl;
                ofxCv::threshold(thresh[i], thresholdValue);
                thresh[i].update();
            }
            usedThresh=0;
            do{
                result = ofxZxing::decodeThresholded(thresh[usedThresh++].getPixelsRef());
            }while(!result.getFound() && usedThresh<THRESH_IMAGES);
            videos[current].update(result);
        }
        else{
            ofLog(OF_LOG_WARNING)<<"Moving without new frame."<<endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    ofTranslate(GUI_WIDTH,0);
    ofTranslate(videos[current].offset);
    ofScale(videos[current].scale,videos[current].scale);
    ofSetColor(255);
    videos[current].video.draw(0,0);
    if(result.getFound()) {
        /*float rotation = result.getRotation();
        ofVec2f position = result.getScreenPosition();
        float size = result.getScreenSize() / logo.getWidth();
        
        ofPushMatrix();
        ofTranslate(position);
        ofRotate(rotation);
        ofScale(size, size, size);
        logo.draw(-logo.getWidth() / 2, -logo.getHeight() / 2);
        ofPopMatrix();*/
        
        result.draw();		
    }
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(GUI_WIDTH,0);
    ofScale(videos[current].scale,videos[current].scale);
    for(int i=0;i<THRESH_IMAGES;i++){
        if(i<usedThresh)
            ofSetColor(255,200);
        else if(i==usedThresh)
            ofSetColor(255,255);
        else
            ofSetColor(255,125);
        thresh[i].draw(i*videos[current].video.getWidth()/THRESH_IMAGES,0,videos[current].video.getWidth()/THRESH_IMAGES,videos[current].video.getHeight()/THRESH_IMAGES);
    }
    ofSetColor(125);
    ofLine(0,videos[current].video.getHeight()/THRESH_IMAGES,videos[current].video.getWidth(),videos[current].video.getHeight()/THRESH_IMAGES);
    ofPopMatrix();
    ofPushMatrix();
    if((current*GUI_ITEM)>(0.75f*GUI_HEIGHT)){
        ofTranslate(0,(int(0.75f*GUI_HEIGHT/GUI_ITEM)-(int)current)*GUI_ITEM);
    }
    for(int i=0;i<videos.size();i++){
        ofSetColor(255);
        videos[i].drawInfo(i==current);
        ofTranslate(0,GUI_ITEM);
        ofSetColor(125);
        ofLine(0,0,GUI_WIDTH,0);
    }
    ofPopMatrix();
    ofPushMatrix();
    ofTranslate(GUI_WIDTH,0);
    ofSetColor(125);
    ofLine(0,0,0,GUI_HEIGHT);
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r'){
        for(int i=0;i<videos.size();i++)
            videos[i].reset();
    }
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

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo info){
    if( info.files.size() > 0 ){
        for(unsigned int i = 0; i < info.files.size(); i++){
            if(ofDirectory::doesDirectoryExist(info.files[i])){
                ofDirectory dir(info.files[i]);
                dir.listDir();
                dir.sort();
                dir.allowExt(".mov");
                
                for(int f=0;f<dir.size();f++){
                    videos.push_back(EasyncVideo(dir.getPath(f)));
                }
            }
        }
    }
}
