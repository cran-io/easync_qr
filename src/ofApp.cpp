#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetWindowTitle("Easync Media - QR Slate");

	font.loadFont("font.ttf",12);

    EasyncVideo test("test.mp4");
    videos.push_back(test);
    
    current=0;
    //videos[current].video.play();
    
#ifdef TRY_HARDER
    for(int i=0; i<PROCESS_IMAGES; i++)
        process[i].allocate(videos[current].video.getWidth(), videos[current].video.getHeight());
#endif
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
#ifdef TRY_HARDER
            for(int i=0; i<PROCESS_IMAGES; i++)
                process[i].allocate(videos[current].video.getWidth(), videos[current].video.getHeight());
#endif
        }
    }
    
    videos[current].video.nextFrame();
    videos[current].video.update();
    if(!videos[current].processed){
        if(videos[current].video.isFrameNew()) {
#ifdef TRY_HARDER
            for(usedProcess=0; usedProcess<PROCESS_IMAGES; usedProcess++){
                process[usedProcess].setFromPixels(videos[current].video.getPixelsRef());
                switch(usedProcess){
                    case 0:
                        break;
                    case 1:
                        process[usedProcess].convertToRange(0,510); //Max:765 Min:-510
                        break;
                    case 2:
                        process[usedProcess].convertToRange(-255,255); //Max:765 Min:-510
                        break;
                    case 3:
                        process[usedProcess].convertToRange(-255,510); //Max:765 Min:-500
                        break;
                    case 4:
                        process[usedProcess].convertToRange(-255,765); //Max:765 Min:-500
                        break;
                    case 5:
                        process[usedProcess].convertToRange(-510,765); //Max:765 Min:-500
                        break;
                }
                result = ofxZxing::decode(process[usedProcess].getPixelsRef());
                if(result.getFound())
                    break;
            }
#else
			result = ofxZxing::decode(videos[current].video.getPixelsRef());
#endif
            videos[current].update(result);
        }
        else{
            //ofLog(OF_LOG_WARNING)<<"Moving without new frame."<<endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
#ifndef APP_NO_WINDOW
	ofBackgroundGradient(ofColor(100),ofColor(0));
    ofNoFill();
    ofPushMatrix();
    ofTranslate(GUI_WIDTH,0);
    ofTranslate(videos[current].offset);
    ofScale(videos[current].scale,videos[current].scale);
    ofSetColor(255);
    videos[current].video.draw(0,0);
    if(result.getFound()) {
        result.draw();
    }
    ofPopMatrix();
#ifdef TRY_HARDER
    ofPushMatrix();
    ofTranslate(GUI_WIDTH,0);
    ofScale(videos[current].scale,videos[current].scale);
    for(int i=0;i<PROCESS_IMAGES;i++){
        if(i<usedProcess)
            ofSetColor(255,200);
        else if(i==usedProcess)
            ofSetColor(255,255);
        else
            ofSetColor(255,125);
        process[i].draw(i*videos[current].video.getWidth()/PROCESS_IMAGES,0,videos[current].video.getWidth()/PROCESS_IMAGES,videos[current].video.getHeight()/PROCESS_IMAGES);
    }
    ofSetColor(125);
    ofLine(0,videos[current].video.getHeight()/PROCESS_IMAGES,videos[current].video.getWidth(),videos[current].video.getHeight()/PROCESS_IMAGES);
    ofPopMatrix();
#endif
    ofPushMatrix();
    if((current*GUI_ITEM)>(0.75f*GUI_HEIGHT)){
        ofTranslate(0,(int(0.75f*GUI_HEIGHT/GUI_ITEM)-(int)current)*GUI_ITEM);
    }
    for(int i=0;i<videos.size();i++){
        ofSetColor(255);
        videos[i].drawInfo(font,i==current);
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
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r'){
        for(int i=0;i<videos.size();i++)
            videos[i].reset();
        current=0;
    }
	else if(key == 'p'){
		videos[current].processed=true;
	}
	else if(key == 'n'){
		videos[current].video.nextFrame();
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
                dir.allowExt("mov");
				dir.allowExt("mp4");
                dir.listDir();
                dir.sort();
                
                for(int f=0;f<dir.size();f++){
					string absolutePath = dir.getFile(f).getAbsolutePath();
#ifdef TARGET_WIN32
					if(absolutePath.at(1)!=':'){
						absolutePath="C:"+absolutePath;
						ofLog(OF_LOG_WARNING)<<"[WINDOWS] Absolute file path doesnt have ':'. Adding 'C:' to the path, please move them to the Local Disk directory:"<<absolutePath<<endl;
					}
#endif
					videos.push_back(EasyncVideo(absolutePath));
                }
            }
        }
    }
}
