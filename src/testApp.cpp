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
    moveDown = false;
    verdana22.loadFont("verdana.ttf", 22, true, true);
	verdana22.setLineHeight(18.0f);
	verdana22.setLetterSpacing(1.037);
    loadSettings("settings.xml");
    receiver.setup( port );
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.createGround();
	box2d.setFPS(30.0);
  //  box2d.createBounds(0, 0,ofGetScreenWidth(), 0);

    score1 = 0;
    score2 = 0;
    counter = 0;
    count = 0;
    paddlewidth = ofGetWidth()/50;
	// register the listener so that we get the events
	ofAddListener(box2d.contactStartEvents, this, &testApp::contactStart);
	ofAddListener(box2d.contactEndEvents, this, &testApp::contactEnd);

    ofxBox2dRect player1;
    player1.setPhysics(0.1, 1.0, 0.0);
    player1.setup(box2d.getWorld(), 0, ofGetHeight()-10, paddlewidth, paddlewidth, b2_kinematicBody);
    player1.setData(new Data());
  //  paddle1.body->SetUserData(paddle1);
    Data * sd1 = (Data*)player1.getData();
    sd1->soundID = ofRandom(0, N_SOUNDS);
    sd1->hit	= false;		
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
	sd2->type = 1;
    players.push_back(player2);	

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
    xincrement = 25;
    xlimit = ofGetWidth()-20;
    xmin = 20;
    
    for(int i=0; i < 4; i++){
        Invader v;
        v.setPhysics(1.0, 0.5, 0.3);
        v.setup(box2d.getWorld(), float(((ofGetWidth()/2 - 60)/columns)+ (ofGetWidth()/2)), float(((ofGetHeight() - ofGetHeight()/2)/(rows))*i + 60), 40,40, b2_staticBody);
     v.setupTheCustomData();
  //      v.movie = rightInvaderVideos[i];
        rightInvaders1.push_back(v);
        printf("right position \n", rightInvaders1[i].getPosition().x);
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
void testApp::contactStart(ofxBox2dContactArgs &e) {
    if(e.a != NULL && e.b != NULL) { 
		
		// if we collide with the ground we do not
		// want to play a sound. this is how you do that
		if((e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_polygon) || (e.a->GetType() == b2Shape::e_polygon && e.b->GetType() == b2Shape::e_circle )) {
			
			Data * aData = (Data*)e.a->GetBody()->GetUserData();
			Data * bData = (Data*)e.b->GetBody()->GetUserData();
            
            if(aData){
                if((aData->type == 0 && bData->type == 1)|| (bData->type == 0 && aData->type == 1)) {
                    sound[aData->soundID].play(); 
                    //apply a force back at the ball when it hits a paddle
                    b2Vec2 veloc = e.b->GetBody()->GetLinearVelocity();
                    veloc.operator*=(1.5);
                    veloc.operator-();
                    e.b->GetBody()->SetLinearVelocity(veloc);
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
            joystick2 = m.getArgAsInt32( 1 );
        }
        if ( m.getAddress() == "/user" )
		{
			// both the arguments are int32's
			user = m.getArgAsString( 0 );
            loaduser = true;
        }
    }
        
   
        if(loaduser && whichuser == 0){
            user1.loadImage(user);
            whichuser = 1;
            user1load = true;
            loaduser = false;
        }
        
        if(loaduser && whichuser == 1){
            user2.loadImage(user);
            whichuser = 0;
            user2load = true;
            loaduser = false;
        }
    

//    for(int i=0; i<leftInvaderVideos.size(); i++){ 
//        leftInvaderVideos[i]->idleMovie();
//    }
//    for(int i=0; i<rightInvaderVideos.size(); i++){ 
//        rightInvaderVideos[i]->idleMovie();
//    }
    
    
   //MOVE THE PLAYERS
//        mapped_joystick1 = int(ofMap(joystick1, 0, 360, 0, ofGetHeight()));
//        mapped_joystick2 = int(ofMap(joystick2, 0, 360, 0, ofGetHeight()));
    
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
    
      if(count%50 == 1){
         for(int i=0; i<leftInvaders1.size(); i++){
                leftInvaders1[i].setPosition(leftInvaders1[i].getPosition().x + xincrement, leftInvaders1[i].getPosition().y);
                leftInvaders1[i].update();
         }
         for(int i=0; i<leftInvaders2.size(); i++){
             leftInvaders2[i].setPosition(leftInvaders2[i].getPosition().x + xincrement, leftInvaders2[i].getPosition().y);
             leftInvaders2[i].update();
         }
         for(int i=0; i<leftInvaders3.size(); i++){
             leftInvaders3[i].setPosition(leftInvaders3[i].getPosition().x + xincrement, leftInvaders3[i].getPosition().y);
             leftInvaders3[i].update();
         }
         for(int i=0; i<leftInvaders4.size(); i++){
             leftInvaders4[i].setPosition(leftInvaders4[i].getPosition().x + xincrement, leftInvaders4[i].getPosition().y);
             leftInvaders4[i].update();
         }
       for(int i=0; i<rightInvaders1.size(); i++){
             rightInvaders1[i].setPosition(rightInvaders1[i].getPosition().x + xincrement, rightInvaders1[i].getPosition().y);
             rightInvaders1[i].update();
             }
         for(int i=0; i<rightInvaders2.size(); i++){
             rightInvaders2[i].setPosition(rightInvaders2[i].getPosition().x + xincrement, rightInvaders2[i].getPosition().y);
             rightInvaders2[i].update();
             
         }
         for(int i=0; i<rightInvaders3.size(); i++){
             rightInvaders3[i].setPosition(rightInvaders3[i].getPosition().x + xincrement, rightInvaders3[i].getPosition().y);
             rightInvaders3[i].update();
             
         }
         for(int i=0; i<rightInvaders4.size(); i++){
             rightInvaders4[i].setPosition(rightInvaders4[i].getPosition().x + xincrement, rightInvaders4[i].getPosition().y);
             rightInvaders4[i].update();
         }

    }
        
    for(int i=0; i<rightInvaders4.size(); i++){
        if (rightInvaders4[i].getPosition().x >= xlimit)  moveDown = true;
    }
    
    for(int i=0; i<leftInvaders1.size(); i++){
        if (leftInvaders1[i].getPosition().x <= xmin)  moveDown = true;
    }
       
    //SHIFT EVERYONE DOWN A ROW WHEN WE HIT THE EDGE
        
    if(moveDown){
        printf("incremement!");
        for(int i=0; i<leftInvaders1.size(); i++){
            leftInvaders1[i].setPosition(leftInvaders1[i].getPosition().x + xincrement*-1, leftInvaders1[i].getPosition().y + yincrement);
            leftInvaders1[i].update();
        }
        for(int i=0; i<leftInvaders2.size(); i++){
            leftInvaders2[i].setPosition(leftInvaders2[i].getPosition().x + xincrement*-1, leftInvaders2[i].getPosition().y+ yincrement);
            leftInvaders2[i].update();
        }
        for(int i=0; i<leftInvaders3.size(); i++){
            leftInvaders3[i].setPosition(leftInvaders3[i].getPosition().x + xincrement*-1, leftInvaders3[i].getPosition().y+ yincrement);
            leftInvaders3[i].update();
        }
        for(int i=0; i<leftInvaders4.size(); i++){
            leftInvaders4[i].setPosition(leftInvaders4[i].getPosition().x + xincrement*-1, leftInvaders4[i].getPosition().y+ yincrement);
            leftInvaders4[i].update();
        }
        for(int i=0; i<rightInvaders1.size(); i++){
            rightInvaders1[i].setPosition(rightInvaders1[i].getPosition().x+ xincrement*-1, rightInvaders1[i].getPosition().y+ yincrement);
            rightInvaders1[i].update();
        }
        for(int i=0; i<rightInvaders2.size(); i++){
            rightInvaders2[i].setPosition(rightInvaders2[i].getPosition().x + xincrement*-1, rightInvaders2[i].getPosition().y+ yincrement);
            rightInvaders2[i].update();
            
        }
        for(int i=0; i<rightInvaders3.size(); i++){
            rightInvaders3[i].setPosition(rightInvaders3[i].getPosition().x + xincrement*-1, rightInvaders3[i].getPosition().y+ yincrement);
            rightInvaders3[i].update();
            
        }
        for(int i=0; i<rightInvaders4.size(); i++){
            rightInvaders4[i].setPosition(rightInvaders4[i].getPosition().x + xincrement*-1, rightInvaders4[i].getPosition().y+ yincrement);
            rightInvaders4[i].update();
        }
        xincrement = xincrement * -1;
        moveDown = false;

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
        if(theData->hit == true || bullets[i].getPosition().y < 0){
            theData->hit = false;  
            //     rightInvaders4[i].movie->stop();
            //   delete rightInvaders4[i].movie;
            box2d.getWorld()->DestroyBody(bullets[i].body);
            //     rightInvaderVideos.erase(rightInvaderVideos.begin()+i+rows*3);
            bullets.erase(bullets.begin()+i);  
        }

        
    }

    
    
    count++;
}

//--------------------------------------------------------------
void testApp::draw() {
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
 
    //DRAW PROFILE PHOTOS AS PLAYERS
        if(user1load){
            user1.draw(players[0].getPosition().x, players[0].getPosition().y, players[0].getWidth() * 2, players[0].getWidth() * 2);
            verdana22.drawString(ofToString(score1, 1), 100,20);
        }
        
        if(user2load){
            verdana22.drawString(ofToString(score2, 1), ofGetWidth() - 50,20);
            user2.draw(players[1].getPosition().x, players[1].getPosition().y, players[1].getWidth() * 2, players[1].getWidth() * 2);
        }
        //IF IMAGES AREN'T LOADED JUST DRAW THEM BLUE
        if (!user1load && !user2load){
            for(int i=0; i<players.size(); i++) {
                ofFill();
                Data * data = (Data*)players[i].getData();
                ofSetHexColor(0x4ccae9);
                players[i].draw();
            }
        }
        
        for(int i = 0; i<bullets.size(); i++){
             ofSetHexColor(0x4ccae9);
            bullets[i].draw();
        }
   
//    for(int i=0; i < rightInvaders.size(); i++){
//                    printf(" x location: %d: \n", int(rightInvaders[i].body->GetPosition().x));
//            printf(" y location: %d: \n", int(rightInvaders[i].body->GetPosition().y));
//        
//    }
//    
//    for(int i=0; i < leftInvaders.size(); i++){
//        printf(" x location: %d: \n", int(leftInvaders[i].body->GetPosition().x));
//        printf(" y location: %d: \n", int(leftInvaders[i].body->GetPosition().y));
//        
//    }


    
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
    
    //EVENTUALLY KNOW WHICH PLAYER IS SHOOTING
    if (key == 'L' || key == 'l'){
    ofxBox2dCircle  c1;
    c1.setPhysics(0.1, 1.0, 0.1);
    c1.setup(box2d.getWorld(), players[0].getPosition().x, players[0].getPosition().y, 5);
    c1.setVelocity(0, 100);
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
        c2.setup(box2d.getWorld(), players[1].getPosition().x, players[1].getPosition().y, 5);
        
        c2.setVelocity(0, 100);
        c2.setData(new Data());
        Data * sd2 = (Data*)c2.getData();
        sd2->soundID = ofRandom(0, N_SOUNDS);
        sd2->hit	= false;		
        sd2->type = 0;
        bullets.push_back(c2);

        
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


