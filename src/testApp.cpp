#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetVerticalSync(true);
	ofBackgroundHex(0xfdefc2);
    ofTrueTypeFont::setGlobalDpi(72);
    ofSetFrameRate(60);
    loaduser = false;
    user1load = false;
    user2load = false;
    player1win = false;
    player2win = false;
    whichuser = 0;
    loaduser = false;
    drawusers = false;
    countdownnum = 400;
    countdownnumbool = false;
    startScreen = true;
    startGameBool = false;
    login = true;
    alpha = 125;
    alphaincrement = 1;
    moveRightDown = false;
    moveLeftDown = false;
    verdana22.loadFont("verdana.ttf", 22, true, true);
	verdana22.setLineHeight(18.0f);
	verdana22.setLetterSpacing(1.037);
    loadSettings("settings.xml");
    receiver.setup( port );
	box2d.init();
  	box2d.setGravity(0, 10);
	//box2d.createGround();
	box2d.setFPS(30.0);
    box2d.createBounds(ofGetWidth()/2, 0, 1 , ofGetHeight());
    counter = 0;
    count = 0;
    paddlewidth = ofGetWidth()/50;
	// register the listener so that we get the events
	ofAddListener(box2d.contactStartEvents, this, &testApp::contactStart);
	ofAddListener(box2d.contactEndEvents, this, &testApp::contactEnd);

    

    // load the 8 sfx soundfile
	for (int i=0; i<N_SOUNDS; i++) {
		sound[i].loadSound("sfx/"+ofToString(i)+".mp3");
		sound[i].setMultiPlay(true);
		sound[i].setLoop(false);
	}
    
    vimeologo.loadImage("images/vimeo_logo.png");
    
    //right now i'm making two vectors for each side of movie. if running slowly, can put this into one vector and do a test if both shapes have been deleted before stopping and deleting the movie
//    for(int i=0; i< 13; i++){
//        ofVideoPlayer * v = new ofVideoPlayer();
//        v->loadMovie("movies/fingers" + ofToString(i) +".mov");
//        v->play();
//        leftInvaderVideos.push_back(v);
//    }
//    for(int i=0; i< 13; i++){
//        ofVideoPlayer * v = new ofVideoPlayer();
//        v->loadMovie("movies/fingers" + ofToString(i) +".mov");
//        v->play();
//        rightInvaderVideos.push_back(v);
//    }
    rows = 4;
    columns = 4;
    yincrement = 25;
    xLincrement = 25;
    xRincrement = 25;
    xlimit = ofGetWidth()-20;
    xmin = 20;
    
    }

//--------------------------------------------------------------
void testApp::contactStart(ofxBox2dContactArgs &e) {
    if(e.a != NULL && e.b != NULL) { 
		
		// if we collide with the ground we do not
		// want to play a sound. this is how you do that
		if((e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_polygon) || (e.a->GetType() == b2Shape::e_polygon && e.b->GetType() == b2Shape::e_circle )) {
			
			Data * aData = (Data*)e.a->GetBody()->GetUserData();
			Data * bData = (Data*)e.b->GetBody()->GetUserData();
            
            if(aData){
                if((aData->type == 0 && bData->type == 1)|| (bData->type == 0 && aData->type == 1)) {
                 //   sound[aData->soundID].play(); 
                    //apply a force back at the ball when it hits a paddle
                    aData->hit = true;
                    bData->hit = true; 
                    printf("paddle collision!");
                }
                
                if(aData->type == 0 && bData->type == 2){
                    aData->hit = true;
                    bData->hit = true;
                    printf("collision!");
                }
                
                if(bData->type == 0 && aData->type == 2){
                    aData->hit = true;  
                    bData->hit = true;
                    printf("collision!");
                    
                    
               }
            }
            
 		}
	}

	}

//--------------------------------------------------------------
void testApp::contactEnd(ofxBox2dContactArgs &e) {
}



//--------------------------------------------------------------
void testApp::update() {
	
	box2d.update();
    
        //OSC STUFF
    while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		if ( m.getAddress() == "/joysticks" )
		{
			// both the arguments are int32's
			joystick1 = m.getArgAsInt32( 0 );
           // joystick2 = m.getArgAsInt32( 1 );
        }
        if ( m.getAddress() == "/user" && login)
		{
            user = m.getArgAsString( 0 );
            printf("got a user!");
            loaduser = true;
        }

    }
        
   
    if(loaduser && login && whichuser == 0){
        user1.loadImage(user);
        whichuser = 1;
        user1load = true;
        loaduser = false;
    }
    
    if(loaduser && login && whichuser == 1){
        user2.loadImage(user);
        whichuser = 0;
        user2load = true;
        loaduser = false;
    }
    
    alpha  = alpha + alphaincrement;
    if(alpha > 200 || alpha < 100){
        alphaincrement = alphaincrement * -1;
    }
    
    if(countdownnumbool){
        printf("counting down: %d \n", countdownnum);
        countdownnum = countdownnum -1;
    }
    
        if(startGameBool){

//    for(int i=0; i<leftInvaderVideos.size(); i++){ 
//        leftInvaderVideos[i]->idleMovie();
//    }
//    for(int i=0; i<rightInvaderVideos.size(); i++){ 
//        rightInvaderVideos[i]->idleMovie();
//    }
    
    

    
            b2Vec2 pos1 =  players[0].body->GetPosition();
            b2Vec2 target1 = b2Vec2(mouseX/OFX_BOX2D_SCALE, pos1.y );
         // b2Vec2 target1 = b2Vec2(mapped_joystick1/OFX_BOX2D_SCALE, pos1.y );
            target1.x = ofClamp(target1.x, 0, (ofGetWidth()/2)/OFX_BOX2D_SCALE - (paddlewidth/OFX_BOX2D_SCALE));
            b2Vec2 diff1 = b2Vec2(target1.x-pos1.x,target1.y-pos1.y);
            diff1.operator*=(2);
            players[0].body->SetLinearVelocity(diff1);

            b2Vec2 pos2 =  players[1].body->GetPosition();
            b2Vec2 target2 = b2Vec2(mouseX/OFX_BOX2D_SCALE, pos2.y );
         // b2Vec2 target2 = b2Vec2(mapped_joystick2/OFX_BOX2D_SCALE, pos2.y );
            target2.x = ofClamp(target2.x, (ofGetWidth()/2)/OFX_BOX2D_SCALE + (paddlewidth/OFX_BOX2D_SCALE), ofGetWidth()/OFX_BOX2D_SCALE);
            b2Vec2 diff2 = b2Vec2(target2.x-pos2.x,target2.y-pos2.y);
            diff2.operator*=(2);
            players[1].body->SetLinearVelocity(diff2);

    
    //SHIFT EVERYONE OVER 
    
      if(count%30 == 1){
         for(int i=0; i<leftInvaders1.size(); i++){
                leftInvaders1[i].setPosition(leftInvaders1[i].getPosition().x + xLincrement, leftInvaders1[i].getPosition().y);
                leftInvaders1[i].update();
         }
         for(int i=0; i<leftInvaders2.size(); i++){
             leftInvaders2[i].setPosition(leftInvaders2[i].getPosition().x + xLincrement, leftInvaders2[i].getPosition().y);
             leftInvaders2[i].update();
         }
         for(int i=0; i<leftInvaders3.size(); i++){
             leftInvaders3[i].setPosition(leftInvaders3[i].getPosition().x + xLincrement, leftInvaders3[i].getPosition().y);
             leftInvaders3[i].update();
         }
         for(int i=0; i<leftInvaders4.size(); i++){
             leftInvaders4[i].setPosition(leftInvaders4[i].getPosition().x + xLincrement, leftInvaders4[i].getPosition().y);
             leftInvaders4[i].update();
         }
       for(int i=0; i<rightInvaders1.size(); i++){
             rightInvaders1[i].setPosition(rightInvaders1[i].getPosition().x + xRincrement, rightInvaders1[i].getPosition().y);
             rightInvaders1[i].update();
             }
         for(int i=0; i<rightInvaders2.size(); i++){
             rightInvaders2[i].setPosition(rightInvaders2[i].getPosition().x + xRincrement, rightInvaders2[i].getPosition().y);
             rightInvaders2[i].update();
             
         }
         for(int i=0; i<rightInvaders3.size(); i++){
             rightInvaders3[i].setPosition(rightInvaders3[i].getPosition().x + xRincrement, rightInvaders3[i].getPosition().y);
             rightInvaders3[i].update();
             
         }
         for(int i=0; i<rightInvaders4.size(); i++){
             rightInvaders4[i].setPosition(rightInvaders4[i].getPosition().x + xRincrement, rightInvaders4[i].getPosition().y);
             rightInvaders4[i].update();
         }

    }
        
   
    //TEST X POSITIONS FOR BOTH SIDES
    if(rightInvaders4.size() > 0){
    for(int i=0; i<rightInvaders4.size(); i++){
        if (rightInvaders4[i].getPosition().x >= xlimit){  
            moveRightDown = true;
        }
    }
    }
    else if(rightInvaders4.size() < 1 && rightInvaders3.size() > 0 ){
        for(int i=0; i<rightInvaders3.size(); i++){
            if (rightInvaders3[i].getPosition().x >= xlimit ){
                moveRightDown = true;
            } 
            }
    }
    else if(rightInvaders4.size() < 1 && rightInvaders3.size() < 1 && rightInvaders2.size() > 0){
        for(int i=0; i<rightInvaders2.size(); i++){
            if (rightInvaders2[i].getPosition().x >= xlimit){
                moveRightDown = true;
            }
        }
    }
    else if(rightInvaders4.size() < 1 && rightInvaders3.size() < 1 && rightInvaders2.size() < 1 && rightInvaders1.size() > 0){
        for(int i=0; i<rightInvaders1.size(); i++){
            if (rightInvaders1[i].getPosition().x >= xlimit)  {
                moveRightDown = true;
            } 

        }
        
    }

    if(rightInvaders1.size() > 0){
        for(int i=0; i<rightInvaders1.size(); i++){
            if (rightInvaders1[i].getPosition().x <= xmin + ofGetWidth()/2){  
                moveRightDown = true;
            }
        }
    }
    else if(rightInvaders1.size() < 1 && rightInvaders2.size() > 0 ){
        for(int i=0; i<rightInvaders2.size(); i++){
            if (rightInvaders2[i].getPosition().x <= xmin + ofGetWidth()/2 ){
                moveRightDown = true;
            } 
        }
    }
    else if(rightInvaders1.size() < 1 && rightInvaders2.size() < 1 && rightInvaders3.size() > 0){
        for(int i=0; i<rightInvaders3.size(); i++){
            if (rightInvaders3[i].getPosition().x <= xmin + ofGetWidth()/2){
                moveRightDown = true;
            }
        }
    }
    else if(rightInvaders1.size() < 1 && rightInvaders3.size() < 1 && rightInvaders2.size() < 1 && rightInvaders4.size() > 0){
        for(int i=0; i<rightInvaders4.size(); i++){
            if (rightInvaders4[i].getPosition().x <= xmin + ofGetWidth()/2)  {
                moveRightDown = true;
            } 
            
        }
        
    }

    
    if(leftInvaders1.size() > 0){
     for(int i=0; i<leftInvaders1.size(); i++){
         if (leftInvaders1[i].getPosition().x <= xmin) {
             moveLeftDown = true;
                      } 
        }
     }
     else if(leftInvaders2.size() > 0 && leftInvaders1.size() < 1){
         for(int i=0; i<leftInvaders2.size(); i++){
             if (leftInvaders2[i].getPosition().x <= xmin){
                  moveLeftDown = true;
            } 
         }
     }
     else if(leftInvaders2.size() < 1 && leftInvaders1.size() < 1 && leftInvaders3.size() > 0){
         for(int i=0; i<leftInvaders3.size(); i++){
             if (leftInvaders3[i].getPosition().x <= xmin ) {
                  moveLeftDown = true;
             } 

         }
     }
     else if(leftInvaders2.size() < 1 && leftInvaders1.size() < 1 && leftInvaders3.size() < 1 && leftInvaders4.size() > 0){
         for(int i=0; i<leftInvaders4.size(); i++){
             if (leftInvaders4[i].getPosition().x <= xmin)  {
                  moveLeftDown = true;
             } 

         }
         
     }

    if(leftInvaders4.size() > 0){
        for(int i=0; i<leftInvaders4.size(); i++){
            if (leftInvaders4[i].getPosition().x >= xlimit - ofGetWidth()/2) {
                moveLeftDown = true;
            } 
        }
    }
    else if(leftInvaders3.size() > 0 && leftInvaders4.size() < 1){
        for(int i=0; i<leftInvaders3.size(); i++){
            if (leftInvaders3[i].getPosition().x >= xlimit - ofGetWidth()/2){
                moveLeftDown = true;
            } 
        }
    }
    else if(leftInvaders2.size() < 3 && leftInvaders1.size() < 4 && leftInvaders2.size() > 0){
        for(int i=0; i<leftInvaders2.size(); i++){
            if (leftInvaders2[i].getPosition().x >= xlimit - ofGetWidth()/2) {
                moveLeftDown = true;
            } 
        }
    }
    else if(leftInvaders2.size() < 1 && leftInvaders3.size() < 1 && leftInvaders4.size() < 1 && leftInvaders1.size() > 0){
        for(int i=0; i<leftInvaders1.size(); i++){
            if (leftInvaders1[i].getPosition().x >= xlimit - ofGetWidth()/2)  {
                moveLeftDown = true;
            } 
        }
    }

       
    //SHIFT EVERYONE DOWN A ROW WHEN WE HIT THE EDGE
    if(moveLeftDown){
        printf("incremement!");
        for(int i=0; i<leftInvaders1.size(); i++){
            leftInvaders1[i].setPosition(leftInvaders1[i].getPosition().x + xLincrement*-1, leftInvaders1[i].getPosition().y + yincrement);
            leftInvaders1[i].update();
        }
        for(int i=0; i<leftInvaders2.size(); i++){
            leftInvaders2[i].setPosition(leftInvaders2[i].getPosition().x + xLincrement*-1, leftInvaders2[i].getPosition().y+ yincrement);
            leftInvaders2[i].update();
        }
        for(int i=0; i<leftInvaders3.size(); i++){
            leftInvaders3[i].setPosition(leftInvaders3[i].getPosition().x + xLincrement*-1, leftInvaders3[i].getPosition().y+ yincrement);
            leftInvaders3[i].update();
        }
        for(int i=0; i<leftInvaders4.size(); i++){
            leftInvaders4[i].setPosition(leftInvaders4[i].getPosition().x + xLincrement*-1, leftInvaders4[i].getPosition().y+ yincrement);
            leftInvaders4[i].update();
        }
        xLincrement = xLincrement * -1;
        moveLeftDown = false;    
    }
    if(moveRightDown){
        for(int i=0; i<rightInvaders1.size(); i++){
            rightInvaders1[i].setPosition(rightInvaders1[i].getPosition().x+ xRincrement*-1, rightInvaders1[i].getPosition().y+ yincrement);
            rightInvaders1[i].update();
        }
        for(int i=0; i<rightInvaders2.size(); i++){
            rightInvaders2[i].setPosition(rightInvaders2[i].getPosition().x + xRincrement*-1, rightInvaders2[i].getPosition().y+ yincrement);
            rightInvaders2[i].update();
            
        }
        for(int i=0; i<rightInvaders3.size(); i++){
            rightInvaders3[i].setPosition(rightInvaders3[i].getPosition().x + xRincrement*-1, rightInvaders3[i].getPosition().y+ yincrement);
            rightInvaders3[i].update();
            
        }
        for(int i=0; i<rightInvaders4.size(); i++){
            rightInvaders4[i].setPosition(rightInvaders4[i].getPosition().x + xRincrement*-1, rightInvaders4[i].getPosition().y+ yincrement);
            rightInvaders4[i].update();
        }
        xRincrement = xRincrement * -1;
        moveRightDown = false;
    }
    
    //INVADERS SEND BULLETS EVERY SO OFTEN
    if(int(ofRandom(0,100))== 1 && leftInvaders1.size()>0){
    ofxBox2dCircle  c2;
    c2.setPhysics(0.1, 1.0, 0.1);
    c2.setup(box2d.getWorld(), leftInvaders1[leftInvaders1.size() - 1].getPosition().x, leftInvaders1[leftInvaders1.size() - 1].getPosition().y + leftInvaders1[3].getWidth()/2 + 40, 5);
        c2.setVelocity(0, 10);
    c2.setData(new Data());
    Data * sd2 = (Data*)c2.getData();
    sd2->soundID = ofRandom(0, N_SOUNDS);
    sd2->hit	= false;		
    sd2->type = 0;
    bullets.push_back(c2);
    }
    if(int(ofRandom(0,100))== 1 && leftInvaders2.size()>0){
        ofxBox2dCircle  c2;
        c2.setPhysics(0.1, 1.0, 0.1);
        c2.setup(box2d.getWorld(), leftInvaders2[leftInvaders2.size() - 1].getPosition().x, leftInvaders2[leftInvaders2.size() - 1].getPosition().y + leftInvaders2[leftInvaders2.size() - 1].getWidth()/2 + 40, 5);
        c2.setVelocity(0, 10);
        c2.setData(new Data());
        Data * sd2 = (Data*)c2.getData();
        sd2->soundID = ofRandom(0, N_SOUNDS);
        sd2->hit	= false;		
        sd2->type = 0;
        bullets.push_back(c2);
    }
    if(int(ofRandom(0,100))== 1  && leftInvaders3.size()>0){
        ofxBox2dCircle  c2;
        c2.setPhysics(0.1, 1.0, 0.1);
        c2.setup(box2d.getWorld(), leftInvaders3[leftInvaders3.size() - 1].getPosition().x, leftInvaders3[leftInvaders3.size() - 1].getPosition().y + leftInvaders3[leftInvaders3.size() - 1].getWidth()/2 + 40, 5);
        c2.setVelocity(0, 10);
        c2.setData(new Data());
        Data * sd2 = (Data*)c2.getData();
        sd2->soundID = ofRandom(0, N_SOUNDS);
        sd2->hit	= false;		
        sd2->type = 0;
        bullets.push_back(c2);
    }
    if(int(ofRandom(0,100))== 1  && leftInvaders4.size()>0){
        ofxBox2dCircle  c2;
        c2.setPhysics(0.1, 1.0, 0.1);
        c2.setup(box2d.getWorld(), leftInvaders4[leftInvaders4.size() - 1].getPosition().x, leftInvaders4[leftInvaders4.size() - 1].getPosition().y + leftInvaders1[leftInvaders4.size() - 1].getWidth()/2 + 40, 5);
        c2.setVelocity(0, 10);
        c2.setData(new Data());
        Data * sd2 = (Data*)c2.getData();
        sd2->soundID = ofRandom(0, N_SOUNDS);
        sd2->hit	= false;		
        sd2->type = 0;
        bullets.push_back(c2);
    }
    if(int(ofRandom(0,100))== 1  && rightInvaders1.size()>0){
        ofxBox2dCircle  c2;
        c2.setPhysics(0.1, 1.0, 0.1);
        c2.setup(box2d.getWorld(), rightInvaders1[rightInvaders1.size() - 1].getPosition().x, rightInvaders1[rightInvaders1.size() - 1].getPosition().y + rightInvaders1[rightInvaders1.size() - 1].getWidth()/2 + 40, 5);
        c2.setVelocity(0, 10);
        c2.setData(new Data());
        Data * sd2 = (Data*)c2.getData();
        sd2->soundID = ofRandom(0, N_SOUNDS);
        sd2->hit	= false;		
        sd2->type = 0;
        bullets.push_back(c2);
    }
    if(int(ofRandom(0,100))== 1 && rightInvaders2.size()>0){
        ofxBox2dCircle  c2;
        c2.setPhysics(0.1, 1.0, 0.1);
        c2.setup(box2d.getWorld(), rightInvaders2[rightInvaders2.size() - 1].getPosition().x, rightInvaders2[rightInvaders2.size() - 1].getPosition().y + rightInvaders2[rightInvaders2.size() - 1].getWidth()/2 + 40, 5);
        c2.setVelocity(0, 10);
        c2.setData(new Data());
        Data * sd2 = (Data*)c2.getData();
        sd2->soundID = ofRandom(0, N_SOUNDS);
        sd2->hit	= false;		
        sd2->type = 0;
        bullets.push_back(c2);
    }
    if(int(ofRandom(0,100))== 1 && rightInvaders3.size()>0){
        ofxBox2dCircle  c2;
        c2.setPhysics(0.1, 1.0, 0.1);
        c2.setup(box2d.getWorld(), rightInvaders3[rightInvaders3.size() - 1].getPosition().x, rightInvaders3[rightInvaders3.size() - 1].getPosition().y + rightInvaders3[rightInvaders3.size() - 1].getWidth()/2 + 40, 5);
        c2.setVelocity(0, 10);
        c2.setData(new Data());
        Data * sd2 = (Data*)c2.getData();
        sd2->soundID = ofRandom(0, N_SOUNDS);
        sd2->hit	= false;		
        sd2->type = 0;
        bullets.push_back(c2);
    }
    if(int(ofRandom(0,100))== 1 && rightInvaders4.size()>0){
        ofxBox2dCircle  c2;
        c2.setPhysics(0.1, 1.0, 0.1);
        c2.setup(box2d.getWorld(), rightInvaders4[rightInvaders4.size() - 1].getPosition().x, rightInvaders4[rightInvaders4.size() - 1].getPosition().y + rightInvaders4[rightInvaders4.size() - 1].getWidth()/2 + 40, 5);
        c2.setVelocity(0, 10);
        c2.setData(new Data());
        Data * sd2 = (Data*)c2.getData();
        sd2->soundID = ofRandom(0, N_SOUNDS);
        sd2->hit	= false;		
        sd2->type = 0;
        bullets.push_back(c2);
    }
   
    
    //DELETE WHEN THERE'S A HIT
        
    for(int i=0; i<leftInvaders1.size(); i++){
        Data * theData = (Data*)leftInvaders1[i].getData();
        if(theData->hit == true){
            theData->hit = false;  
         // leftInvaders1[i].movie->stop();
       //     delete leftInvaders1[i].movie;
            box2d.getWorld()->DestroyBody(leftInvaders1[i].body);
          //  leftInvaderVideos.erase(leftInvaderVideos.begin()+i);
            leftInvaders1.erase(leftInvaders1.begin()+i);  
            }
        }

    for(int i=0; i<leftInvaders2.size(); i++){
        Data * theData = (Data*)leftInvaders2[i].getData();
        if(theData->hit == true){
            theData->hit = false;  
         //   leftInvaders2[i].movie->stop();
          //  delete leftInvaders2[i].movie;
            box2d.getWorld()->DestroyBody(leftInvaders2[i].body);
         //   leftInvaderVideos.erase(leftInvaderVideos.begin()+i+rows);
            leftInvaders2.erase(leftInvaders2.begin()+i);  
        }
    }

    for(int i=0; i<leftInvaders3.size(); i++){
        Data * theData = (Data*)leftInvaders3[i].getData();
        if(theData->hit == true){
            theData->hit = false;  
       //     leftInvaders3[i].movie->stop();
      //      delete leftInvaders3[i].movie;
            box2d.getWorld()->DestroyBody(leftInvaders3[i].body);
     //       leftInvaderVideos.erase(leftInvaderVideos.begin()+i+rows*2);
            leftInvaders3.erase(leftInvaders3.begin()+i);  
        }
    }

    for(int i=0; i<leftInvaders4.size(); i++){
        Data * theData = (Data*)leftInvaders4[i].getData();
        if(theData->hit == true){
            theData->hit = false;  
  //        leftInvaders4[i].movie->stop();
         //   delete leftInvaders4[i].movie;
            box2d.getWorld()->DestroyBody(leftInvaders4[i].body);
          //  leftInvaderVideos.erase(leftInvaderVideos.begin()+i+rows*3);
            leftInvaders4.erase(leftInvaders4.begin()+i);  
        }
    }

  for(int i=0; i<rightInvaders1.size(); i++){
        Data * theData = (Data*)rightInvaders1[i].getData();
        if(theData->hit == true){
            theData->hit = false;  
        //    rightInvaders1[i].movie->stop();
         //   delete leftInvaders1[i].movie;
            box2d.getWorld()->DestroyBody(rightInvaders1[i].body);
       //     rightInvaderVideos.erase(rightInvaderVideos.begin()+i);
            rightInvaders1.erase(rightInvaders1.begin()+i);  
        }
    }
    
    for(int i=0; i<rightInvaders2.size(); i++){
        Data * theData = (Data*)rightInvaders2[i].getData();
        if(theData->hit == true){
            theData->hit = false;  
        //    rightInvaders2[i].movie->stop();
       //     delete leftInvaders2[i].movie;
            box2d.getWorld()->DestroyBody(rightInvaders2[i].body);
       //     rightInvaderVideos.erase(rightInvaderVideos.begin()+i+rows);
            rightInvaders2.erase(rightInvaders2.begin()+i);  
        }
    }
    
    for(int i=0; i<rightInvaders3.size(); i++){
        Data * theData = (Data*)rightInvaders3[i].getData();
        if(theData->hit == true){
            theData->hit = false;  
       //     rightInvaders3[i].movie->stop();
      //      delete rightInvaders3[i].movie;
            box2d.getWorld()->DestroyBody(rightInvaders3[i].body);
    //        rightInvaderVideos.erase(rightInvaderVideos.begin()+i+rows*2);
            rightInvaders3.erase(rightInvaders3.begin()+i);  
        }
    }
    
    for(int i=0; i<rightInvaders4.size(); i++){
        Data * theData = (Data*)rightInvaders4[i].getData();
        if(theData->hit == true){
            theData->hit = false;  
       //     rightInvaders4[i].movie->stop();
         //   delete rightInvaders4[i].movie;
            box2d.getWorld()->DestroyBody(rightInvaders4[i].body);
       //     rightInvaderVideos.erase(rightInvaderVideos.begin()+i+rows*3);
            rightInvaders4.erase(rightInvaders4.begin()+i);  
        }
    }
    
    for(int i=0; i<bullets.size(); i++){
        Data * theData = (Data*)bullets[i].getData();
        if(theData->hit == true|| bullets[i].getPosition().y < 0 || bullets[i].getPosition().y > ofGetHeight() ){
            //     rightInvaders4[i].movie->stop();
            //   delete rightInvaders4[i].movie;
            box2d.getWorld()->DestroyBody(bullets[i].body);
            //     rightInvaderVideos.erase(rightInvaderVideos.begin()+i+rows*3);
            bullets.erase(bullets.begin()+i);  
            theData->hit = false;  
            bullets[i].update();
        }

        
    }


    for(int i=0; i<players.size(); i++){
        Data * theData = (Data*)players[i].getData();
        if(theData->hit == true ){
            theData->hit = false;  
            theData->paddleopacity =  theData->paddleopacity - 85 ;
           
        }
        if(theData->paddleopacity == 0)
            theData->killed = true;
        }
            //IF YOU'VE BEEN HIT 3 TIMES
            Data * theData0 = (Data*)players[0].getData();
            if(theData0->killed == true){
                player2win = true;
            }
            Data * theData1 = (Data*)players[1].getData();
            if(theData1->killed == true){
                player1win = true;
            }
            //IF YOU KILL ARE YOUR INVADERS
            if(leftInvaders1.size() == 0 && leftInvaders2.size()==0 && leftInvaders3.size()==0 && leftInvaders4.size()==0){
                player1win = true;
            }
            if(rightInvaders1.size() == 0 && rightInvaders2.size()==0 && rightInvaders3.size()==0 && rightInvaders4.size()==0){
                player2win = true;
            }
            
            //IF THE INVADERS INVADE YOU
            
          if(rightInvaders1[rightInvaders1.size() - 1].getPosition().y > ofGetHeight()-players[1].getHeight() ||rightInvaders2[ rightInvaders2.size() - 1].getPosition().y > ofGetHeight()-players[1].getHeight() ||rightInvaders3[rightInvaders3.size() - 1].getPosition().y > ofGetHeight()-players[1].getHeight() ||rightInvaders4[rightInvaders4.size() - 1].getPosition().y > ofGetHeight()-players[1].getHeight())
              player1win;

            if(leftInvaders1[leftInvaders1.size() - 1].getPosition().y > ofGetHeight()-players[1].getHeight() ||leftInvaders2[ leftInvaders2.size() - 1].getPosition().y > ofGetHeight()-players[1].getHeight() ||leftInvaders3[leftInvaders3.size() - 1].getPosition().y > ofGetHeight()-players[1].getHeight() ||leftInvaders4[leftInvaders4.size() - 1].getPosition().y > ofGetHeight()-players[1].getHeight())
                player2win;
            
                if(stopGame == true && counter > 200){
                for(int i = 0; i<leftInvaders1.size(); i++){
                    box2d.world->DestroyBody(leftInvaders1[i].body);
                   // delete leftInvaders1[i].movie;
                }
                for(int i = 0; i<leftInvaders2.size(); i++){
                    box2d.world->DestroyBody(leftInvaders2[i].body);
                    // delete leftInvaders2[i].movie;
                }
                for(int i = 0; i<leftInvaders3.size(); i++){
                    box2d.world->DestroyBody(leftInvaders3[i].body);
                    // delete leftInvaders3[i].movie;
                }
                for(int i = 0; i<leftInvaders4.size(); i++){
                    box2d.world->DestroyBody(leftInvaders4[i].body);
                    // delete leftInvaders4[i].movie;
                }
                for(int i = 0; i<rightInvaders1.size(); i++){
                    box2d.world->DestroyBody(rightInvaders1[i].body);
                    // delete rightInvaders1[i].movie;
                }
                for(int i = 0; i<rightInvaders2.size(); i++){
                    box2d.world->DestroyBody(rightInvaders2[i].body);
                    // delete rightInvaders2[i].movie;
                }
                for(int i = 0; i<rightInvaders3.size(); i++){
                    box2d.world->DestroyBody(rightInvaders3[i].body);
                    // delete rightInvaders3[i].movie;
                }
                for(int i = 0; i<rightInvaders4.size(); i++){
                    box2d.world->DestroyBody(rightInvaders4[i].body);
                    // delete rightInvaders4[i].movie;
                }
                for(int i = 0; i<bullets.size(); i++){
                    box2d.world->DestroyBody(bullets[i].body);
                }
                for(int i = 0; i<players.size(); i++){
                    box2d.world->DestroyBody(players[i].body);
                    // delete rightInvaders4[i].movie;
                }        
                player1win = false;
                player2win = false;
                startGameBool = false;
                leftInvaders1.clear();
                leftInvaders2.clear();
                leftInvaders3.clear();
                leftInvaders4.clear();
                rightInvaders1.clear();
                rightInvaders2.clear();
                rightInvaders3.clear();
                rightInvaders4.clear();
                players.clear();
                bullets.clear();
                startScreen = true;    
                drawusers = false;
                counter = 0;
            }
    
    count++;
}
            
}

//--------------------------------------------------------------
void testApp::draw() {
    if(startScreen){
        verdana22.drawString("VFA INVADERS", ofGetWidth()/2, ofGetHeight()/2);
        verdana22.drawString("Press button to start a new game", ofGetWidth()/2, ofGetHeight()/2 + 100);
        printf("startscreen \n");
    }
    
    if(!user1load && login && !startScreen){
        //    printf("alpha: %d: \n", alpha);
        ofSetColor(238,58,130, alpha);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofRect(0,0, ofGetWidth()/2, ofGetHeight());
        ofSetColor(0);
        verdana22.drawString("Left Player Tap!", ofGetWidth()/4, ofGetHeight()/2 + 100);
    }
    
    if(user1load && !user2load && login){
        ofSetColor(238,58,130, alpha);
        ofSetRectMode(OF_RECTMODE_CORNER);
        ofRect(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
        ofSetColor(255);
        user1.draw(ofGetWidth()/4, ofGetWidth()/20, 100, 100);
        ofSetColor(0);
        verdana22.drawString("Right Player Tap!", ofGetWidth()/4 + ofGetWidth()/2, ofGetHeight()/2 + 100);
    }
    
    if(user1load &&  user2load){
        countdownnumbool = true;
        ofSetColor(0);
        if(countdownnum > 300)
            verdana22.drawString("3", ofGetWidth()/2 , ofGetHeight()/2 + 100);
        if(countdownnum > 200 && countdownnum < 300 )
            verdana22.drawString("2", ofGetWidth()/2 , ofGetHeight()/2 + 100);
        if(countdownnum > 100 && countdownnum < 200)
            verdana22.drawString("1", ofGetWidth()/2, ofGetHeight()/2 + 100);
        if(countdownnum > 0 && countdownnum < 100)
            verdana22.drawString("GO!", ofGetWidth()/2, ofGetHeight()/2 + 100);
        if (countdownnum == 0){
            countdownnumbool = false;
            startGame();
            drawusers = true;
            login = false;
            user1load = false;
            user2load = false;
        }
        
    }
 

    if(drawGame){
    
        ofLine(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());

        
    //DRAW THE INVADERS
    
    for(int i=0; i<leftInvaders1.size(); i++){
        leftInvaders1[i].draw();
    }
    for(int i=0; i<leftInvaders2.size(); i++){
        leftInvaders2[i].draw();
    }
    for(int i=0; i<leftInvaders3.size(); i++){
        leftInvaders3[i].draw();
    }
    for(int i=0; i<leftInvaders4.size(); i++){
        leftInvaders4[i].draw();
    }
    for(int i=0; i<rightInvaders1.size(); i++){
        rightInvaders1[i].draw();
    }
    for(int i=0; i<rightInvaders2.size(); i++){
        rightInvaders2[i].draw();
    }
    for(int i=0; i<rightInvaders3.size(); i++){
        rightInvaders3[i].draw();
    }
    for(int i=0; i<rightInvaders4.size(); i++){
        rightInvaders4[i].draw();
    }
    for(int i = 0; i<bullets.size(); i++){
        ofSetHexColor(0x4ccae9);
        bullets[i].draw();
    }
    
    }
    if(player1win){
        ofSetColor(245, 58, 135);
        verdana22.drawString("YOU WIN!", ofGetWidth()/4, ofGetHeight()/2);
        verdana22.drawString("YOU LOSE!", ofGetWidth()/4 + ofGetWidth()/2, ofGetHeight()/2);
        stopGame = true;
        counter++;
        printf("counter: %d\n", counter);
        drawGame = false;
    }
    if(player2win){
        ofSetColor(245, 58, 135);
        verdana22.drawString("YOU LOSE!", ofGetWidth()/4, ofGetHeight()/2);
        verdana22.drawString("YOU WIN!", ofGetWidth()/4 + ofGetWidth()/2, ofGetHeight()/2);
        stopGame = true;
        counter++;
        printf("counter: %d\n", counter);
        drawGame = false;
    }
    

    ofSetColor(255, 255, 255);
    
    
    //DRAW PROFILE PHOTOS AS PLAYERS
        if(drawusers){
            Data * data1 = (Data*)players[0].getData();
            ofSetColor(255,255,255, data1->paddleopacity);
            user1.draw(players[0].getPosition().x, players[0].getPosition().y, players[0].getWidth() * 2, players[0].getWidth() * 2);
            verdana22.drawString(ofToString(score1, 1), 100,20);
            verdana22.drawString(ofToString(score2, 1), ofGetWidth() - 50,20);
             Data * data2 = (Data*)players[1].getData();
              ofSetColor(255,255,255, data2->paddleopacity);
            user2.draw(players[1].getPosition().x, players[1].getPosition().y, players[1].getWidth() * 2, players[1].getWidth() * 2);
        }
        else{
           for(int i=0; i<players.size(); i++) {
                ofFill();
                Data * data = (Data*)players[i].getData();
                ofSetColor(0,0,255, data->paddleopacity);
                players[i].draw();
            }
        }
        
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    
    //EVENTUALLY KNOW WHICH PLAYER IS SHOOTING
    if (key == 'L' || key == 'l'){
    ofxBox2dCircle  c1;
        c1.setPhysics(0.1, 1.0, 0.1);
    c1.setup(box2d.getWorld(), players[0].getPosition().x, players[0].getPosition().y - players[0].getHeight()*2, 5);
    c1.setVelocity(0, -50);
    c1.setData(new Data());
    Data * sd1 = (Data*)c1.getData();
    sd1->soundID = ofRandom(0, N_SOUNDS);
    sd1->hit	= false;		
    sd1->type = 0;
    bullets.push_back(c1);
    }
if (key == 'R' || key == 'r'){
        ofxBox2dCircle  c2;
        c2.setPhysics(0.1, 1.0, 0.1);
        c2.setup(box2d.getWorld(), players[1].getPosition().x, players[1].getPosition().y - players[1].getHeight()*2, 5);
        c2.setVelocity(0, -50);
        c2.setData(new Data());
        Data * sd2 = (Data*)c2.getData();
        sd2->soundID = ofRandom(0, N_SOUNDS);
        sd2->hit	= false;		
        sd2->type = 0;
        bullets.push_back(c2);
    }
    
    if(key == 's' && startScreen == true){
        startScreen = false;
        login = true;
        
    }

}

void testApp::startGame(){
    startGameBool = true;
    drawGame = true;
    ofxBox2dRect player1;
    player1.setPhysics(0.1, 1.0, 0.0);
    player1.setup(box2d.getWorld(), 0, ofGetHeight()-10, paddlewidth, paddlewidth, b2_kinematicBody);
    player1.setData(new Data());
    //  paddle1.body->SetUserData(paddle1);
    Data * sd1 = (Data*)player1.getData();
    sd1->soundID = ofRandom(0, N_SOUNDS);
    sd1->hit	= false;		
    sd1-> paddleopacity = 255;
    sd1-> killed = false;;
    sd1->type = 1;
    players.push_back(player1);	
    
    ofxBox2dRect player2;
    player2.setPhysics(0.1, 1.0, 0.0);
    player2.setup(box2d.getWorld(), ofGetWidth(), ofGetHeight()-10, paddlewidth, paddlewidth, b2_kinematicBody);
    //   paddle2.body->SetUserData(paddle2);
    player2.setData(new Data());
    Data * sd2 = (Data*)player2.getData();
    sd2->soundID = ofRandom(0, N_SOUNDS);
    sd2->hit	= false;
    sd2->killed = false;
	sd2-> paddleopacity = 255;
	sd2->type = 1;
    players.push_back(player2);	
    
    for(int i=0; i < 4; i++){
        Invader v;
        v.setPhysics(1.0, 0.5, 0.3);
        v.setup(box2d.getWorld(), float(((ofGetWidth()/2 - 60)/columns)+ (ofGetWidth()/2)), float(((ofGetHeight() - ofGetHeight()/2)/(rows))*i + 60), 40,40, b2_staticBody);
        v.setupTheCustomData();
        //      v.movie = rightInvaderVideos[i];
        rightInvaders1.push_back(v);
    }
    
    for(int i=0; i < rows; i++){
        Invader v;
        v.setPhysics(1.0, 0.5, 0.3);
        v.setup(box2d.getWorld(), float(((ofGetWidth()/2 - 60)/columns)*2 + (ofGetWidth()/2)), float(((ofGetHeight() - ofGetHeight()/2)/(rows))*i + 60), 40,40, b2_staticBody);
        v.setupTheCustomData();
        //       v.movie = rightInvaderVideos[i + rows];
        rightInvaders2.push_back(v);
    }
    
    for(int i=0; i < rows; i++){
        Invader v;
        v.setPhysics(1.0, 0.5, 0.3);
        v.setup(box2d.getWorld(), float(((ofGetWidth()/2 - 60)/columns)*3 + (ofGetWidth()/2 )), float(((ofGetHeight() - ofGetHeight()/2)/(rows))*i + 60), 40,40, b2_staticBody);
        v.setupTheCustomData();
        //       v.movie = rightInvaderVideos[i + rows*2];
        rightInvaders3.push_back(v);
    }
    
    
    for(int i=0; i < rows; i++){
        Invader v;
        v.setPhysics(1.0, 0.5, 0.3);
        v.setup(box2d.getWorld(), float(((ofGetWidth()/2 - 60)/columns)*4 + (ofGetWidth()/2)), float(((ofGetHeight() - ofGetHeight()/2)/rows)*i + 60), 40,40, b2_staticBody);
        v.setupTheCustomData();
        //     v.movie = rightInvaderVideos[i + rows*3];
        rightInvaders4.push_back(v);
    }
    
    
    for(int i=0; i < rows; i++){
        Invader v;
        v.setPhysics(1.0, 0.5, 0.3);
        v.setup(box2d.getWorld(), float(((ofGetWidth()/2 - 60)/columns)), float(((ofGetHeight() - ofGetHeight()/2)/rows)*i + 60), 40,40, b2_staticBody);
        v.setupTheCustomData();
        //      v.movie = leftInvaderVideos[i];
        leftInvaders1.push_back(v);
    }
    
    for(int i=0; i < rows; i++){
        Invader v;
        v.setPhysics(1.0, 0.5, 0.3);
        v.setup(box2d.getWorld(), float(((ofGetWidth()/2 - 60)/columns)*2), float(((ofGetHeight() - ofGetHeight()/2)/rows)*i + 60), 40,40, b2_staticBody);
        v.setupTheCustomData();
        //       v.movie = leftInvaderVideos[i + rows];
        leftInvaders2.push_back(v);
    }
    
    
    for(int i=0; i < rows; i++){
        Invader v;
        v.setPhysics(1.0, 0.5, 0.3);
        v.setup(box2d.getWorld(), float(((ofGetWidth()/2 - 60)/columns)*3), float(((ofGetHeight() - ofGetHeight()/2)/rows)*i + 60), 40,40, b2_staticBody);
        v.setupTheCustomData();
        //     v.movie = leftInvaderVideos[i + rows*2];
        leftInvaders3.push_back(v);
    }
    
    
    for(int i=0; i < rows; i++){
        Invader v;
        v.setPhysics(1.0, 0.5, 0.3);
        v.setup(box2d.getWorld(), float(((ofGetWidth()/2 - 60)/columns)*4), float(((ofGetHeight() - ofGetHeight()/2)/rows)*i + 60), 40,40, b2_staticBody);
        v.setupTheCustomData();
        //     v.movie = leftInvaderVideos[i  + rows*3];
        leftInvaders4.push_back(v);
    }
    

    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
  	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

void testApp::loadSettings(string fileString){
	string host_address;
	string host_address1;
	string host_address2;
	string filename;
	
	//--------------------------------------------- get configs
    ofxXmlSettings xmlReader;
	bool result = xmlReader.loadFile(fileString);
	if(!result) printf("error loading xml file\n");
	
	
	host_address = xmlReader.getValue("settings:master:address","test",0);
	port = xmlReader.getValue("settings:master:port",5204,0);
	host = (char *) malloc(sizeof(char)*host_address.length());
	strcpy(host, host_address.c_str());
    
		
	filename = xmlReader.getValue("settings:movie:","test",0);
	
	
//		int w = xmlReader.getValue("settings:dimensions:width", 640, 0);
	//int h = xmlReader.getValue("settings:dimensions:height", 480, 0);
	
		//ofSetWindowShape(w, h);

	
	if(xmlReader.getValue("settings:go_fullscreen", "false", 0).compare("true") == 0) {
		fullscreen = true;
		ofSetFullscreen(true);
	}
}


