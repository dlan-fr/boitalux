#include "ofApp.h"
#include "enum.h"

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

     uint32_t igr = 0;
    for(uint16_t iface = 0;iface < 5; iface++) {

        //testcol = checkuv[iface];
        for(uint32_t rtest = 0;rtest < div_grille;rtest++) {
            for(uint32_t ctest = 0;ctest < div_grille;ctest++) {
                grille[igr++].set(couleurbase.r,couleurbase.g,couleurbase.b,couleurbase.a);

            }
        }
    }
}

void ofApp::remplirFace(ofRectangle face,ofColor color,FaceFill fill = FaceFill::full) {

    float startx,endx = 0.0f;
    float starty,endy = 0.0f;

    if(fill == FaceFill::full) {
        startx = face.x;
        endx = face.x + face.width;
        starty = face.y;
        endy = face.y + face.height;
    }
    else if(fill == FaceFill::topleft) {
        startx = face.x;
        endx =face.x + (face.width * 0.5f);
        starty = face.y;
        endy = face.y + (face.height * 0.5f);
    }
    else if(fill == FaceFill::topright) {
        startx = face.x + (face.width * 0.5f);
        endx =face.x + face.width;
        starty = face.y;
        endy = face.y + (face.height * 0.5f);
    }
    else if(fill == FaceFill::bottomleft) {
        startx = face.x;
        endx =face.x + (face.width * 0.5f);
        starty = face.y + (face.height * 0.5f);
        endy = face.y + face.height;
    }


    for(float y = starty;y < endy;y++) {

        for(float x =startx; x < endx;x++) {
            texture_buffer.setColor(x,y,color);
        }
    }


}

ofColor ofApp::pickColor(float x,float y,uint32_t start_index,ofRectangle face,FillOrient orient) {
    uint32_t col = 0;
    uint32_t row = 0;

    uint32_t colpix = (face.width / div_grille);
    uint32_t rowpix = (face.height / div_grille);

    switch(orient) {
        case FillOrient::horz:
         col = x / colpix;
         row = y / rowpix;
        break;
        case FillOrient::vertinvert:
        {
            col = div_grille - (x / colpix);
            row = (y / rowpix);

            uint32_t tmpcol = col;
            col = row;
            row = tmpcol;
        }


        break;
        case FillOrient::horzinvert:
        col = div_grille - (x / colpix);
        row = div_grille - (y / rowpix);
            break;
        case FillOrient::vert:
        break;

    }


    if(row == div_grille) {
        row = div_grille - 1;
    }

    if(col == div_grille) {
        col = div_grille - 1;
    }

    uint32_t igrid = (start_index)  + ((row * div_grille)+ col);


    return grille[igrid];


}

void ofApp::majFaceDepuisGrille(ofRectangle face,FaceIndex index,FillOrient orient) {

    uint32_t start_index = index * (div_grille * div_grille);
    float xrel = 0;
    float yrel = 0;

    for(float y = face.y;y < face.y + face.height;y++) {
        xrel = 0;
        for(float x =face.x; x < face.x + face.width;x++) {
            ofColor color = this->pickColor(xrel,yrel,start_index,face,orient);
            texture_buffer.setColor(x,y,color);
            xrel++;
        }

        yrel++;

    }

}

void ofApp::updateGrille(FaceIndex index, int poscol,int fillval,ofColor colfill) {

    uint32_t start_index = index * (div_grille * div_grille);


    for(int f = 0; f < fillval;f++) {
        uint32_t index = (start_index + poscol) + (f * div_grille);
        grille[index] = colfill;
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

   div_grille = 48;

   //une grille 6*6 pour nos 5 face (pas de grille pour la face cachée)
   total_face_grille = (div_grille * div_grille);
   total_grille = total_face_grille * 5;
   grille.reserve(total_grille);

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
    //this->remplirFace(top,ofColor::red);

    front.set(0,341,342,343);
    //this->remplirFace(front,ofColor::blue);


    //correspond à la face du dessous non utilisée
    //bottom.set(0,682,342,342);


    left.set(342,341,343,342);
    //this->remplirFace(left,ofColor::green);

    right.set(342,0,343,341);
    //this->remplirFace(right,ofColor::pink);

    back.set(683,0,341,341);

    //this->remplirFace(back,ofColor::yellow);

    //initialisation grille
    //ordre top / front / left / right / back

    for(uint32_t grillei = 0; grillei < total_grille;grillei++) {
        grille.push_back(ofColor::white);
    }


    uint32_t igr = 0;//total_face_grille * 2;
    ofColor testcol = ofColor::blue;

    /*vector<ofColor> checkuv;

    checkuv.push_back(ofColor::red);
    checkuv.push_back(ofColor::blue);
    checkuv.push_back(ofColor::green);
    checkuv.push_back(ofColor::pink);
    checkuv.push_back(ofColor::yellow);

    for(uint16_t iface = 0;iface < 5; iface++) {
        testcol = ofColor::blue;
        //testcol = checkuv[iface];
        for(uint32_t rtest = 0;rtest < div_grille;rtest++) {
            for(uint32_t ctest = 0;ctest < div_grille;ctest++) {
                grille[igr++].set(testcol.r,testcol.g,testcol.b,testcol.a);
                testcol.r += 10;
            }

            testcol.g += 10;
        }
    }
*/


    //initialisation fft et son

    plotHeight = 16;
    bufferSize = 2048;

    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_BARTLETT);

    beat = new ofxBeat(bufferSize,512);

    beat->setBeatValue();

    drawBins.resize(fft->getBinSize());
    middleBins.resize(fft->getBinSize());
    audioBins.resize(fft->getBinSize());

    //liste des sortie et entrée audio disponibles pour openframework

   // std::vector<ofSoundDevice> listdevices = ofSoundStreamListDevices();

    ofSoundStreamSettings settings;


    //std::cout << "Liste des devices" << std::endl;

    //soundStream.printDeviceList();


    /*for(std::vector<ofSoundDevice>::iterator it = listdevices.begin(); it != listdevices.end(); ++it) {
        if(it->name.find("QUAD-CAPTURE") != std::string::npos) {
            settings.setInDevice((*it));
            ofSoundStream.setDevice((*it));
            std::cout << "Carte Son trouvée !" << std::endl;
        }
        //std::cout << it->name << std::endl;
    }*/


    // 0 output channels,
    // 1 input channel
    // 44100 samples per second
    // [bins] samples per buffer
    // 4 num buffers (latency)
   // settings.setApi(ofSoundDevice::Api::PULSE);
    settings.bufferSize = bufferSize;
    settings.sampleRate = 44100;
    settings.numBuffers = 4;
    settings.numOutputChannels = 0;
    settings.numInputChannels = 1;
    settings.setInListener(this);

    auto devices = soundStream.getMatchingDevices("QUAD-CAPTURE");

    if(!devices.empty()){
            settings.setInDevice(devices[0]);
            std::cout << "Carte Son trouvée : " << devices[0].name <<std::endl;
            //soundStream.setDevice(devices[0]);
            //soundStream.setDeviceID(devices[0].deviceID);
     }

    soundStream.setup(settings);

   // soundStream.start();


   // ofSoundStreamSetup(settings);



    //configuration détection beat
    storedkick = 0.0f;
    storedsnare = 0.0f;
    treshkick = 0.8f;
    treshsnare = 0.3f;
}

void ofApp::audioIn(ofSoundBuffer &buffer)
{


    float maxValue = 0;

    //std::cout << "max value : " << maxValue << std::endl;

    for(int i = 0; i < buffer.size(); i++) {
        if(abs(buffer.getSample(i,1)) > maxValue) {
            maxValue = abs(buffer.getSample(i,1));
        }
    }

    //std::cout << "max value 2 : " << maxValue << std::endl;

    std::vector<float>& input = buffer.getBuffer();

    const float* input_const = &input[0];

     beat->audioReceived(input_const,input.size(),buffer.getNumChannels());


    /*for(int i = 0; i < buffer.size(); i++) {
        input[i] /= maxValue;
    }*/

    fft->setSignal(input);

    float* curFft = fft->getAmplitude();
    memcpy(&audioBins[0], curFft, sizeof(float) * fft->getBinSize());

   /* maxValue = 0;
    for(int i = 0; i < fft->getBinSize(); i++) {
        if(abs(audioBins[i]) > maxValue) {
            maxValue = abs(audioBins[i]);
        }
    }
    for(int i = 0; i < fft->getBinSize(); i++) {
        audioBins[i] /= maxValue;
    }*/

    soundMutex.lock();
    middleBins = audioBins;
    soundMutex.unlock();
}

void ofApp::plot(vector<float>& buffer, float scale)
{
    initCube();

    int grillefillmax = (div_grille * 4);

    if(grillefillmax > buffer.size()) {
        grillefillmax = buffer.size();
    }

    //pour répartir les samples sur l'ensemble des éléments de la grille
    //TODO : prendre en compte la partie décimale pour intégrer l'ensemble des samples
    //contenus dans le buffer (a tester 3/4 de la grille avec une agg de 5 et le reste avec une agg de 6)
    int agregate = buffer.size() / grillefillmax;



    ofColor graphcolor = ofColor::blueSteel;

    for (int i = 0; i < grillefillmax; i++) {

        float acc = 0;

        for(int iacc = 0;iacc < agregate;iacc++) {
            acc += buffer[i+acc];
        }

        float prop = (acc / agregate) * scale;

        int fillval = (int)(prop * div_grille);

        if(fillval > div_grille) {
            fillval = div_grille;
        }


        if(i < div_grille) {
            this->updateGrille(FaceIndex::left,i,fillval,graphcolor);
        }

        if(i >=div_grille && i < (div_grille + div_grille)) {
            this->updateGrille(FaceIndex::front,i-div_grille,fillval,graphcolor);
        }

        if(i >= (div_grille * 2) && i < (div_grille * 2) + div_grille) {
            this->updateGrille(FaceIndex::right,i-(div_grille*2),fillval,graphcolor);
        }

        if(i >= (div_grille * 3) && i < (div_grille * 3) + div_grille) {
             this->updateGrille(FaceIndex::back,i-(div_grille*3),fillval,graphcolor);
        }


    }


    //maj face du dessus à partir du beat détecté
    float kick = beat->kick();
    float snare = beat->snare();
    float hihat = beat->hihat();

    ofColor snarecolor = ofColor::red;

    snarecolor.setSaturation(snare * 255);
    this->remplirFace(top,snarecolor,FaceFill::topleft);

    ofColor kickcolor = ofColor::black;
    kickcolor.setBrightness(255 - (255 * kick));
    this->remplirFace(top,kickcolor,FaceFill::topright);

    ofColor hihatcolor = ofColor::gold;
    hihatcolor.setSaturation(hihat * 255);
    this->remplirFace(top,hihatcolor,FaceFill::bottomleft);


    /*if(snare > storedsnare && snare > treshsnare) {
        storedsnare = snare;

    }

    if(storedsnare > 0.0f) {
        snarecolor.setSaturation(storedsnare * 255);
        this->remplirFace(top,snarecolor);
    }
    else {
        this->remplirFace(top,ofColor::white);
    }*/

   /* if(kick >= 0.05f || snare >= 0.05f) {
        std::cout << "kick : " << kick << std::endl;
        std::cout << "snare : " << snare << std::endl;
    }*/


    /*if(kick >= 0.005f || snare >= 0.005f) {
        this->remplirFace(top,ofColor::red);
    }
    else {
        this->remplirFace(top,ofColor::white);
    }*/




}

//--------------------------------------------------------------
void ofApp::update(){

    beat->update(ofGetElapsedTimeMillis());

    /*initCube();


    this->remplirFace(top,ofColor::red);
    this->remplirFace(front,ofColor::blue);
    this->remplirFace(left,ofColor::green);

    this->remplirFace(right,ofColor::pink);

    this->remplirFace(back,ofColor::yellow);*/

    soundMutex.lock();
    drawBins = middleBins;
    soundMutex.unlock();
    plot(drawBins, plotHeight);

    if(storedsnare > 0.0f) {
        storedsnare -= 0.2f;

        if(storedsnare < 0.0f) {
            storedsnare = 0.0f;
        }
    }




    //this->majFaceDepuisGrille(top,FaceIndex::top,FillOrient::horz);

    this->majFaceDepuisGrille(front,FaceIndex::front,FillOrient::vertinvert);


    this->majFaceDepuisGrille(left,FaceIndex::left,FillOrient::horzinvert);

    this->majFaceDepuisGrille(right,FaceIndex::right,FillOrient::vertinvert);

    this->majFaceDepuisGrille(back,FaceIndex::back,FillOrient::vertinvert);

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

int colval = 0;
int fillval = 0;
FaceIndex indx = FaceIndex::left;

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    ofRectangle& mod = right;

    /*switch(key) {
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


    }*/



    switch(key) {
                case OF_KEY_LEFT:
                    colval--;
                break;
                case OF_KEY_RIGHT:
                    colval++;
                break;
                case OF_KEY_UP:
                    fillval++;
                 break;
                 case OF_KEY_DOWN:
                   fillval--;
                break;
                case OF_KEY_F1:
                    colval = 0;
                    fillval = 0;
                    break;

               case 52:
                    indx = (FaceIndex)((int)indx+1);
               break;
              case 54:
                   indx = (FaceIndex)((int)indx-1);
               break;


        }

    initCube();
    this->updateGrille(indx,colval,fillval,ofColor::green);




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
    delete beat;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
