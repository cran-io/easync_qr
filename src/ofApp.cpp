#include "ofApp.h"

//--------------------------------------------------------------
ofApp::ofApp(ofxArgs* args){
	this->args = args;
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetWindowTitle("Easync Media - QR Slate");
	ofLogNotice("Easync Media - QR Slate")<<"Starting application";

#ifdef APP_NO_WINDOW
	ofLogNotice("Easync Media - QR Slate")<<"No window mode";
#endif
#ifdef TRY_HARDER
	ofLogNotice("Easync Media - QR Slate")<<"Try harder is ON";
#endif
#ifdef USE_VLC
	ofLogNotice("Easync Media - QR Slate")<<"Using VLC library";
#endif

	font.loadFont("font.ttf",12);

	outputDir = "";
	if(args->contains("-o")){
		outputDir = args->getString("-o");
	}

	vector< string > files;
	for(int i=1;i<args->getCount();i++){
		string arg=args->getString(i);
		if(arg != "-o" && arg != outputDir)
			files.push_back(arg);
	}

	if(files.size()){
		for(int i=1;i<files.size();i++){
			string path=files[i];
			ofFile file(path);
			if(file.exists()){
				if(file.getExtension()=="json"){
					ofLogNotice("Easync Media - QR Slate")<<"Parsing file: "<<path;
					parseJson(file.getAbsolutePath());
				}
				else{
					ofLogWarning("Easync Media - QR Slate")<<"File is not json: "<<path;
				}
			}
			else{
				ofLogWarning("Easync Media - QR Slate")<<"File does not exist: "<<path;
			}
		}
	}
	else{
		ofLogWarning("Easync Media - QR Slate")<<"No command line arguments received";
	}

	if(!videos.size()){
		ofLogWarning("Easync Media - QR Slate")<<"No videos added. Adding default video test.mp4 from data folder";
		EasyncVideo test("test.mp4",outputDir);
		videos.push_back(test);
	}
    
    current=0;
	videos[current].start();    

#ifdef TRY_HARDER
    for(int i=0; i<PROCESS_IMAGES; i++)
        process[i].allocate(videos[current].getWidth(), videos[current].getHeight());
#endif

	fps=nfps=ofGetFrameRate();
}

//--------------------------------------------------------------
void ofApp::parseJson(string path){
	ofxJSONElement input;
	input.open(path);
	for(int i=0;i<input["videos"].size();i++){
		EasyncVideo video(input["videos"][i]["path"].asString(),outputDir);
		videos.push_back(video);
	}
}


//--------------------------------------------------------------
void ofApp::update(){
	nfps=ofGetFrameRate();
	if(nfps<20 && fps>=20){
		ofLog(OF_LOG_WARNING)<<"App frame rate below 20 FPS. Results might be deceiving";
	}
	//else if(nfps>=20 && fps<20){
	//	ofLog(OF_LOG_WARNING)<<"App frame rate OK";
	//}
	fps=nfps;

    if(videos[current].processed){
        unsigned int next=current;
        do{
            next=(next+1)%videos.size();
        }while(videos[next].processed && next!=current);
        if(next!=current){
            current=next;
            videos[current].start();
#ifdef TRY_HARDER
            for(int i=0; i<PROCESS_IMAGES; i++)
                process[i].allocate(videos[current].getWidth(), videos[current].getHeight());
#endif
        }
    }
    
    videos[current].update();
    if(videos[current].isFrameNew()){
        if(!videos[current].processed) {
#ifdef TRY_HARDER
            for(usedProcess=0; usedProcess<PROCESS_IMAGES; usedProcess++){
                process[usedProcess].setFromPixels(videos[current].getPixelsRef());
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
			result = ofxZxing::decode(videos[current].getPixelsRef());
#endif
            videos[current].save(result);
			if(videos[current].processed){
				videos[current].writeResult();
			}
        }
		videos[current].next();
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
    videos[current].draw(0,0);
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
        process[i].draw(i*videos[current].getWidth()/PROCESS_IMAGES,0,videos[current].getWidth()/PROCESS_IMAGES,videos[current].getHeight()/PROCESS_IMAGES);
    }
    ofSetColor(125);
    ofLine(0,videos[current].getHeight()/PROCESS_IMAGES,videos[current].getWidth(),videos[current].getHeight()/PROCESS_IMAGES);
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
		videos[current].start();
    }
	else if(key == 'p'){
		videos[current].processed=true;
	}
	else if(key == 'n'){
		videos[current].nextFrame();
	}
	else if(key == 's'){
		ofSaveImage(videos[current].getPixelsRef(),ofGetTimestampString()+".png");
		ofLog(OF_LOG_NOTICE)<<"Saving image frame to disk";
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
	ofLog(OF_LOG_VERBOSE)<<"Drag event";
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
						ofLog(OF_LOG_WARNING)<<"[WINDOWS] Absolute file path doesnt have ':'. Adding 'C:' to the path, please move them to the Local Disk directory: "<<absolutePath;
					}
#endif
					videos.push_back(EasyncVideo(absolutePath));
                }
            }
        }
    }
}
