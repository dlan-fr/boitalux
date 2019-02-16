#include "ofApp.h"

const char my_user_data[6] = "hello";

void mylog(void* userdata,const char* str) {
    printf("%s\n",str);
}

void ofApp::initCube() {
    ofColor couleurbase = ofColor::white;

    //assignation couleur de départ
    for (int y = 0; y < texture_height; y++){
        for (int x = 0; x < texture_width; x++){

               texture_buffer.setColor(x,y,couleurbase);

        }
    }
}

void ofApp::remplirFace(ofRectangle face,ofColor color) {

    for(float y = face.y;y < face.y + face.height;y++) {

        for(float x =face.x; x < face.x + face.width;x++) {
            texture_buffer.setColor(x,y,color);
        }

    }


}

//--------------------------------------------------------------
void ofApp::setup(){

    std::string path = ofFilePath::getCurrentExeDir();

    std::string socket_path = path;

    socket_path.append("data/socket");

    appwidth = 1024;
    appheight = 768;

   texture_width = 1024;
   texture_height = 1024;

   logger = shmdata_make_logger(&mylog,&mylog,&mylog,&mylog,&mylog,&mylog,(void*)my_user_data);

   char caps[1024] = {0};

   sprintf(caps,"video/x-raw,format=(string)RGB,width=(int)%d,height=(int)%d,framerate=(fraction)1/1",texture_width,texture_height);

   texture_buffer.allocate(texture_width,texture_height,OF_PIXELS_RGB);

   texture_buffer.swapRgb();

   writer = shmdata_make_writer(socket_path.c_str(),texture_buffer.size(),caps,NULL,NULL,NULL,logger);


    initCube();

    texture.allocate(texture_buffer);

    //assignation position sur la texture des différentes faces du cube

    top.set(0,0,342,341);
    this->remplirFace(top,ofColor::red);

    front.set(0,341,342,343);
    this->remplirFace(front,ofColor::blue);


    //correspond à la face du dessous non utilisée
    //bottom.set(0,682,342,342);


    left.set(342,341,343,342);
    this->remplirFace(left,ofColor::green);

    right.set(342,0,343,341);
    this->remplirFace(right,ofColor::pink);

    back.set(683,0,341,341);

    this->remplirFace(back,ofColor::yellow);

}

//--------------------------------------------------------------
void ofApp::update(){

    initCube();


    this->remplirFace(top,ofColor::red);
    this->remplirFace(front,ofColor::blue);
    this->remplirFace(left,ofColor::green);

    this->remplirFace(right,ofColor::pink);

    this->remplirFace(back,ofColor::yellow);

    texture.loadData(texture_buffer);
    shmdata_copy_to_shm(writer,texture_buffer.getData(),texture_buffer.size());
}

int keycode = 0;

//--------------------------------------------------------------
void ofApp::draw(){

    texture.draw(0,0,appwidth,appheight);

    ofSetColor(0,0,0);

     ofRectangle& mod = right;

   std::stringstream debugpos;

  // debugpos << "Cursor pos X :" << cursor.x << " Y : " << cursor.y <<  " Z : " << cursor.z <<  " face : " << faceindex;
   debugpos << "x :" << mod.x << " y : " << mod.y <<" width : " << mod.width << " height : " << mod.height;

   ofDrawBitmapString(debugpos.str(),50,700);
    ofSetColor(255,255,255);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    ofRectangle& mod = right;

    switch(key) {
            case OF_KEY_LEFT:
                mod.setX(mod.x - 1);
            break;
            case OF_KEY_RIGHT:
                mod.setX(mod.x + 1);
            break;
            case OF_KEY_UP:
                mod.setY(mod.y + 1);
             break;
             case OF_KEY_DOWN:
                mod.setY(mod.y - 1);
            break;
             case 52:
                mod.setWidth(mod.width - 1);
            break;
           case 54:
                mod.setWidth(mod.width + 1);
            break;
            case 56:
                mod.setHeight(mod.height + 1);
            break;
            case 50:
                mod.setHeight(mod.height - 1);
            break;


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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

void ofApp::exit() {
    shmdata_delete_logger(logger);
    shmdata_delete_writer(writer);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
