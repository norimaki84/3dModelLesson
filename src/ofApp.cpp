#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE); //ログレベルを取る(表示する)
    ofBackground(50, 0); //背景色設定：(int r, int g, int b, int a=255)
    
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords. openGLでテクスチャ画像を使用するための準備的なやつ
    
    ofSetFrameRate(60);
    ofEnableSmoothing();
    
//    camera.lookAt(ofVec3f(0,0,0));
//    camera.setPosition(0,100,0);
    
    
    
    bAnimate = false;
    bAnimateMouse = false;
    animationPosition = 0;
    
    model.setScale(0.5, 0.5, 0.5);
    model.loadModel("astroBoy_walk.dae", false); //モデルデータの読み込み、第2匹数はモデルを最適化(optimize)するかどうか
    model.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0); //modelのポジション設定:(float x, float y, float z)
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL); //modelのアニメーションフレームをループ
    model.playAllAnimations(); //modelのアニメーション開始
    if(!bAnimate) {
        model.setPausedForAllAnimations(true); //modelのアニメーションの一時停止(true or false)
    }
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    model.update(); //modelをアップデート
    
    if(bAnimateMouse) {
        model.setPositionForAllAnimations(animationPosition);
    }
    
    mesh = model.getCurrentAnimatedMesh(0);
    
    camera.lookAt(ofVec3f(model.getPosition().x,model.getPosition().y,model.getPosition().z));
    camera.setPosition(200*cos(ofGetElapsedTimef()*2), 0, 200*sin(ofGetElapsedTimef()*2));

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    camera.begin(); //カメラ開始
    
    ofSetColor(255); //塗りの色を設定
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA); //ブレンドモードの定義
    
    ofEnableDepthTest(); //深度テストを有効にする関数
    
    
    //mdoelの顔部分
#ifndef TARGET_PROGRAMMABLE_GL
    glShadeModel(GL_SMOOTH); //some model / light stuff
#endif
    light.enable(); //ライティングを有効に
    ofEnableSeparateSpecularLight();
    
    ofPushMatrix(); //ofPushMatrix〜ofPopMatrixで囲まれた範囲内は外部の座標系に影響しない
        ofTranslate(model.getPosition().x+100, model.getPosition().y, 0);
        ofRotate(-mouseX, 0, 1, 0); //マウスのX座標移動により回転
        ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
        model.drawFaces(); //modelの顔を描画
    ofPopMatrix();
    
    //model全体
#ifndef TARGET_PROGRAMMABLE_GL
    glEnable(GL_NORMALIZE);
#endif
    //
    ofPushMatrix();
        ofTranslate(model.getPosition().x-300, model.getPosition().y, 0);
        ofRotate(-mouseX, 0, 1, 0);
        ofTranslate(-model.getPosition().x, -model.getPosition().y, 0);
    
        ofxAssimpMeshHelper & meshHelper = model.getMeshHelper(0);
    
        ofMultMatrix(model.getModelMatrix());
        ofMultMatrix(meshHelper.matrix);
    
        ofMaterial & material = meshHelper.material;
        if(meshHelper.hasTexture()){
            meshHelper.getTextureRef().bind();
        }
        material.begin();
        mesh.drawWireframe();
        material.end();
        if(meshHelper.hasTexture()){
            meshHelper.getTextureRef().unbind();
        }
    ofPopMatrix();
    
    ofDisableDepthTest(); //深度テストを無効に
    light.disable(); //ライティングを無効に
    ofDisableLighting();
    ofDisableSeparateSpecularLight();
    
    //塗り色を黒に
    ofSetColor(255, 255, 255 );
    
    camera.end(); //カメラ終了
    
    //ログの表示
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
    ofDrawBitmapString("keys 1-5 load models, spacebar to trigger animation", 10, 30);
    ofDrawBitmapString("drag to control animation with mouseY", 10, 45);
    ofDrawBitmapString("num animations for this model: " + ofToString(model.getAnimationCount()), 10, 60);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofPoint modelPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75);
    switch (key) { //キーにより読み込むmodelをスイッチ
        case '1':
            model.loadModel("astroBoy_walk.dae");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            ofEnableSeparateSpecularLight();
            break;
        case '2':
            model.loadModel("TurbochiFromXSI.dae");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            model.setRotation(0, -180, 1, 0, 0);
            ofEnableSeparateSpecularLight();
            break;
        case '3':
            model.loadModel("dwarf.x");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            ofDisableSeparateSpecularLight();
            break;
        case '4':
            model.loadModel("monster-animated-character-X.X");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            model.setRotation(0, -90, 0, 0, 1);
            ofDisableSeparateSpecularLight();
            break;
        case '5':
            model.loadModel("squirrel/NewSquirrel.3ds");
            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
            ofDisableSeparateSpecularLight();
            break;
        case 'a':
            ofBackground(0, 0);
            break;
        case 's':
            ofBackground(50, 0);
            break;
        case 'p':
            ofPushMatrix();
            ofTranslate(model.getPosition().x+500, model.getPosition().y, 0);
            model.drawFaces();
            ofPopMatrix();
            break;
//        case 'c':
//            //  何かキーを押したらスタート
//            isStart = true;
//            break;
        case ' ':
            bAnimate = !bAnimate;
            break;
        default:
            break;
    }
    
    mesh = model.getMesh(0);
    
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    if(!bAnimate) {
        model.setPausedForAllAnimations(true);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case 'a':
            ofBackground(50, 0);
            break;
        default:
            break;
    }
    
    //
    //    if (key == 'a'){
    //        ofBackground(0, 0);
    //    } else if (key == 's'){
    //        ofBackground(50, 0);
    //    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // scrub through aninations manually.
    animationPosition = y / (float)ofGetHeight();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // pause all animations, so we can scrub through them manually.
    model.setPausedForAllAnimations(true);
    animationPosition = y / (float)ofGetHeight();
    bAnimateMouse = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    // unpause animations when finished scrubbing.
    if(bAnimate) {
        model.setPausedForAllAnimations(false);
    }
    bAnimateMouse = false;
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

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

