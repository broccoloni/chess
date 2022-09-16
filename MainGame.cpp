#include "MainGame.h"
#include <iostream>
#include <string>
#include <Errors.h>
#include <Engine.h>
#include <ResourceManager.h>

MainGame::MainGame(bool verbose, bool showdisplay){
    m_screenWidth = 1600; 
    m_screenHeight = 900; 
    m_boardSize = 750;
    m_moveNum = 0;
    m_turnColour = 0;
    m_boardOrientation = 0;
    m_curPieceExists = false;
    m_autoflip = true;
    m_isPromoting = false;
    m_mouseTile = glm::vec2(0,0);
    m_mouseCoords = glm::vec2(0,0);
    m_mouseClickPos = glm::vec2(0,0);
    m_mouseChange = glm::vec2(0,0);
    m_curPiece = nullptr;
    m_gameState = GameState::PLAY;
    m_maxFPS = 60.0f;
    m_fps = 0;
    m_time = 0;
	m_boardStart = -m_boardSize/2;
	m_squareSize = m_boardSize/8;
	m_camera.init(m_screenWidth, m_screenHeight);
    m_verbose = verbose;
    m_showdisplay = showdisplay;
}

MainGame::~MainGame(){

}

void MainGame::run(){
    if (m_showdisplay){
        //NOTE: 0,0 is center of the screen
        initSystems();
    
        //initialize background from 10 pixels outside screen 
        m_background.init(glm::vec4(-m_screenWidth/2-10, -m_screenHeight/2-10, m_screenWidth+20, m_screenHeight+20), "textures/dark_gray.png");
        m_background.setDepth(-2.0f);
        //initialize border around 8x8 chess board
        int borderWidth = 0.25*m_squareSize;
        m_border.init(glm::vec4(-4*m_squareSize - borderWidth, -4*m_squareSize - borderWidth, 
                                8*m_squareSize + 2 * borderWidth,8*m_squareSize + 2*borderWidth), "textures/darkwood.png");
        m_border.setDepth(-1.0f); //to have it behind the board
    
        //Buttons
        int buttonWidth;
        int buttonHeight;
        
        //put flip board button in top right of window
        buttonWidth = m_squareSize*3/4;
        buttonHeight = buttonWidth/2;
        m_flipBoardButton.init(glm::vec4(m_screenWidth/2-buttonWidth,m_screenHeight/2-buttonHeight,
                                        buttonWidth,buttonHeight), "textures/dark_brown.png","textures/dark_brown.png");
        
        //autoflip board button
        buttonWidth = m_squareSize*3/8;
        buttonHeight = buttonWidth;
        m_autoFlipButton.init(glm::vec4(m_screenWidth/2-buttonWidth-m_squareSize,m_screenHeight/2-buttonHeight,
                                        buttonWidth,buttonHeight), "textures/dark_brown.png","textures/dark_brown.png");
        
        //pawn promotion boxes
        m_promoBoxes.reserve(12);
        for (int i = -2; i < 2; i++){
            m_promoBoxes[i+2].init(glm::vec4(i*m_squareSize,-0.5*m_squareSize,m_squareSize,m_squareSize),"textures/light_gray.png");
            m_promoBoxes[i+2].setDepth(4.0f);
        }
        Piece* piece = new Piece();
        //white piece texture in pawn promotion box
        for (int i = -2; i < 2; i++){
            m_promoBoxes[i+6].init(glm::vec4(i*m_squareSize,-0.5*m_squareSize,m_squareSize,m_squareSize),piece->texturePath(i+3,0));
            m_promoBoxes[i+6].setDepth(5.0f);
        }
        //black piece texture in pawn promotion box
        for (int i = -2; i < 2; i++){
            m_promoBoxes[i+10].init(glm::vec4(i*m_squareSize,-0.5*m_squareSize,m_squareSize,m_squareSize),piece->texturePath(i+3,1));
            m_promoBoxes[i+10].setDepth(5.0f);
        }
    }
	m_board.init(m_boardStart, m_squareSize, m_verbose, m_showdisplay);
    
    //pawns piecetype = 0
	for (int i = 0; i < 8; i++) {
		m_board.setPiece(i,1, new Piece(m_boardStart, m_squareSize, glm::vec2(i, 1), 0, 0, m_showdisplay));
		m_board.setPiece(i,6, new Piece(m_boardStart, m_squareSize, glm::vec2(i, 6), 1, 0, m_showdisplay)); 
	}
	//rooks piecetype = 1
	m_board.setPiece(0,0, new Piece(m_boardStart, m_squareSize, glm::vec2(0, 0), 0, 1, m_showdisplay));
	m_board.setPiece(0,7, new Piece(m_boardStart, m_squareSize, glm::vec2(0, 7), 1, 1, m_showdisplay));
	m_board.setPiece(7,0, new Piece(m_boardStart, m_squareSize, glm::vec2(7, 0), 0, 1, m_showdisplay));
	m_board.setPiece(7,7, new Piece(m_boardStart, m_squareSize, glm::vec2(7, 7), 1, 1, m_showdisplay));
	//knights piecetype = 2
	m_board.setPiece(1,0, new Piece(m_boardStart, m_squareSize, glm::vec2(1, 0), 0, 2, m_showdisplay));
	m_board.setPiece(1,7, new Piece(m_boardStart, m_squareSize, glm::vec2(1, 7), 1, 2, m_showdisplay));
	m_board.setPiece(6,0, new Piece(m_boardStart, m_squareSize, glm::vec2(6, 0), 0, 2, m_showdisplay));
	m_board.setPiece(6,7, new Piece(m_boardStart, m_squareSize, glm::vec2(6, 7), 1, 2, m_showdisplay));
	//bishops piecetype = 3
	m_board.setPiece(2,0, new Piece(m_boardStart, m_squareSize, glm::vec2(2, 0), 0, 3, m_showdisplay));
	m_board.setPiece(2,7, new Piece(m_boardStart, m_squareSize, glm::vec2(2, 7), 1, 3, m_showdisplay));
	m_board.setPiece(5,0, new Piece(m_boardStart, m_squareSize, glm::vec2(5, 0), 0, 3, m_showdisplay));
	m_board.setPiece(5,7, new Piece(m_boardStart, m_squareSize, glm::vec2(5, 7), 1, 3, m_showdisplay));
	//queens piecetype = 4
	m_board.setPiece(3,0, new Piece(m_boardStart, m_squareSize, glm::vec2(3, 0), 0, 4, m_showdisplay));
	m_board.setPiece(3,7, new Piece(m_boardStart, m_squareSize, glm::vec2(3, 7), 1, 4, m_showdisplay));
	//kings piecetype = 5
	m_board.setPiece(4,0, new Piece(m_boardStart, m_squareSize, glm::vec2(4, 0), 0, 5, m_showdisplay));
	m_board.setPiece(4,7, new Piece(m_boardStart, m_squareSize, glm::vec2(4, 7), 1, 5, m_showdisplay));

	for (int j = 2; j < 6; j++){
		for (int i = 0; i < 8; i++){
			m_board.setPiece(i,j, nullptr);
		}
	}
    m_board.calculateAllMoves();    
    if (m_verbose){
        std::cout<<"******************************";
        std::cout<<" Starting Game ";
        std::cout<<"******************************";
        std::cout<<std::endl;
    }
    gameLoop();
}

void MainGame::initSystems(){
	InitEngine();
	m_window.create("Chess", m_screenWidth, m_screenHeight, 0);
	initShaders();
	m_spriteBatch.init();
	m_fpsLimiter.init(m_maxFPS);	

}

void MainGame::initShaders(){
	m_colourProgram.compileShaders("shaders/shader.vs", "shaders/shader.fs");
	m_colourProgram.addAttribute("vertPosition");
	m_colourProgram.addAttribute("vertColour");
	m_colourProgram.addAttribute("vertUV");
	m_colourProgram.linkShaders();
}

void MainGame::gameLoop(){
	//for no display command line input
    int file,rank;
    std::string loc;

    //start game loop
    while (m_gameState != GameState::EXIT){
        if (m_showdisplay){
            m_fpsLimiter.begin();

            processInput();
            m_time += 0.01;

            m_camera.update();

            drawGame();

            m_fps = m_fpsLimiter.end();
            
            //print only once every 10 frames
            /*
            static int frameCounter = 0;
            frameCounter++;
            
            if (frameCounter == 10){
                std::cout << m_fps <<std::endl;
                frameCounter = 0;
            }
            */
        }
        else{
            std::cout << "Enter the location of the piece you wish to move (Eg. a1): ";
            std::cin >> loc;
            file = (int) tolower(loc[0])-97;
            rank = (int) (loc[1] - '0') - 1;
            if (file > 7 || file < 0 || rank > 7 || rank < 0){
                std::cout<<"Invalid input"<<std::endl;
                continue;
            }
            //empty square
            m_curPiece = m_board.isOccupied(glm::vec2(file,rank));
            if (m_curPiece == nullptr){
                std::cout<<"Empty square"<<std::endl;
                continue;
            }
            //opponents piece
            else if (m_curPiece->colour() != m_turnColour){
                std::cout<<"Not your piece"<<std::endl;
                continue;
            }
            std::cout << "Enter where you'd like to move this piece: ";
            std::cin >> loc;
            file = (int) tolower(loc[0])-97;
            rank = (int) (loc[1] - '0') - 1;
            if (file > 7 || file < 0 || rank > 7 || rank < 0){
                std::cout<<"Invalid input"<<std::endl;
                continue;
            }
            if (m_curPiece -> isAMove(glm::vec2(file,rank))){
                //pawn promotion
                if (rank == (1 - m_turnColour)*7 && m_curPiece->type() == 0){
                    std::cout<<"What would you like to promote to (R,N,B,Q): ";
                    std::cin >> loc; //temp placeholder to get promotion
                    if (loc == "R") m_curPiece->promote(1);
                    else if (loc == "N") m_curPiece->promote(2);
                    else if (loc == "B") m_curPiece->promote(3);
                    else if (loc == "Q") m_curPiece->promote(4);
                    else {
                        std::cout<<"Invalid promotion"<<std::endl;
                        continue;
                    }
                }
                takeTurn(glm::vec2(file,rank));
            }
            else{
                std::cout<<"Invlaid move"<<std::endl;
                continue;
            }
        }
	}
}

void MainGame::processInput(){
	SDL_Event evnt;
	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.1f;

	while (SDL_PollEvent(&evnt) == true){
		switch (evnt.type){
			case SDL_QUIT:
				m_gameState = GameState::EXIT;
				break;

			case SDL_KEYDOWN:
				m_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			
			case SDL_KEYUP:
				m_inputManager.releaseKey(evnt.key.keysym.sym);
				break;

			case SDL_MOUSEBUTTONDOWN:
				m_inputManager.pressKey(evnt.button.button);
				break;

			case SDL_MOUSEBUTTONUP:
            {
                //record event
                m_inputManager.releaseKey(evnt.button.button);
				
                //get mouse coordinates
                m_mouseCoords = m_camera.screenCoordsToWorld(m_inputManager.getMouseCoords());
				
                //find tile mouse is over
                m_mouseTile = m_board.getTile(m_mouseCoords, m_boardOrientation);
    			
                if (m_isPromoting){
                    //just to make sure nothing gets clicked when pawn is being promoted
                }

                //if current piece exists
                else if (m_curPieceExists){
                    //if tile is on board
                    if (m_board.isOnBoard(m_mouseTile)){
                        //if current piece is being held
                        if (m_curPiece -> isBeingHeld()){
                            //move piece by dragging it
                            //verify if it's a valid move
                            if (m_curPiece -> isAMove(m_mouseTile)){
                                //check for pawn promotion
                                if (m_mouseTile.y == (1-m_turnColour)*7 && m_curPiece -> type() == 0){
                                    m_isPromoting = true;
                                    m_board.movePiece(m_curPiece, m_mouseTile);
                                    m_curPiece -> drop();
                                }
                                else{
                                    m_curPiece -> drop();
                                    takeTurn(m_mouseTile);
                                }
                            }
                            //invalid move
                            //reset piece to original position if not a valid move
			    	    	else {
				    	    	m_board.resetPiece(m_curPiece);
    					        m_curPiece->drop();
                            }
                        }
                        //current piece is not being held
                        //if click location contains a piece
                        else if (m_board.isOccupied(m_mouseTile) != nullptr){
                            //if click location is curpiece
                            if (m_mouseTile == m_curPiece->boardPos()){
                                //click off curpiece
                                dropCurPiece();
                            }
                            //if piece is players (not opponents)
                            else if (m_board.isOccupied(m_mouseTile)->colour() == m_turnColour){
                                dropCurPiece();
                                setCurPiece(m_mouseCoords,m_mouseTile);
                            }
                            //piece is opponents 
                            else{
                                dropCurPiece();
                            }
                        }                    
                        //click location is empty
                        else{
                            dropCurPiece();
                        }
                    }
                    //tile not on board but curpiece exists
                    else{
                        //check if piece was being held
                        if (m_curPiece -> isBeingHeld()){
                            //reset piece
		    		    	m_board.resetPiece(m_curPiece);
    		                m_curPiece->drop();
                        }
                        dropCurPiece();
                    }
                }
                //If the click release location was not on the board
                if (!m_board.isOnBoard(m_mouseTile)){
                    //check if flip board button was clicked
                    if (m_flipBoardButton.isUnder(m_mouseCoords)){
                        m_boardOrientation = 1 - m_boardOrientation;
                    }
                    //if the autoflip button was clicked
                    if (m_autoFlipButton.isUnder(m_mouseCoords)){
                        m_autoflip = !m_autoflip;
                        if (m_autoflip) m_boardOrientation = m_turnColour;
                    }
                }
                break;
            }

			case SDL_MOUSEMOTION:
				m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			
		}
	}
	if (m_inputManager.isKeyPressed(SDLK_w)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}

	if (m_inputManager.isKeyPressed(SDLK_s)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}	
	
	if (m_inputManager.isKeyPressed(SDLK_a)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	
	if (m_inputManager.isKeyPressed(SDLK_d)) {
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}				
	
	if (m_inputManager.isKeyPressed(SDLK_q)) {
		m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
	}
	
	if (m_inputManager.isKeyPressed(SDLK_e)) {
		m_camera.setScale(m_camera.getScale() - SCALE_SPEED);
	}
	
	if (m_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
        //get mouse coordinates
		m_mouseCoords = m_camera.screenCoordsToWorld(m_inputManager.getMouseCoords());
		
        //find tile from mouse coordinates
        m_mouseTile = m_board.getTile(m_mouseCoords, m_boardOrientation);
		
        //check if promoting a piece
        if (m_isPromoting){
            for (int i = 0; i < 4; i++){
                if (m_promoBoxes[i].isUnder(m_mouseCoords)){
                    if (m_verbose) {
                        std::vector<std::string> piecetypes{"Rook","Knight","Bishop","Queen"};
                        std::cout<<"Promoting pawn to "<<piecetypes[i]<<std::endl;
                    }
                    m_curPiece->promote(i+1);
                    m_isPromoting = false;
                    takeTurn(m_curPiece->boardPos());
                    break;
                }
            }

        }

        //if current piece exists
        else if (m_curPieceExists){
            //update position of piece if it's being held
		    if (m_curPiece -> isBeingHeld()){
                m_mouseChange = (m_mouseCoords - m_mouseClickPos);
                if (m_boardOrientation == 0){
                    m_curPiece->setPos(m_curPiece->tileToPos(m_curPiece->boardPos()) + m_mouseChange); 
                }
                else{
                    m_curPiece->setPos(m_curPiece->tileToPos(m_curPiece->boardPos()) - m_mouseChange);  
                }
            }
            //if not holding a piece but curpiece exists
            else{ 
                //check if click location is valid move
                if (m_curPiece->isAMove(m_mouseTile)){
                    //check for pawn promotion
                    if (m_mouseTile.y == (1-m_turnColour)*7 && m_curPiece->type() == 0){
                        m_isPromoting = true;
                        m_board.movePiece(m_curPiece, m_mouseTile);
                    }
                    else{
                        //make move
                        takeTurn(m_mouseTile);
                    }
                }
                //if click location is not a valid move
                //but there is a piece there
                else if (m_board.isOccupied(m_mouseTile) != nullptr){
                    //if piece is current piece - do nothing
                    if (m_mouseTile == m_curPiece->boardPos()){
                        //this prevents it from resetting current piece and holding
                        //it so that we can drop the cur piece in the mouse button up
                    }
                    //if piece is one of the players
                    else if (m_board.isOccupied(m_mouseTile) -> colour() == m_turnColour){
                        dropCurPiece();
                        setCurPiece(m_mouseCoords,m_mouseTile);
                    }
                }
            }
        }
        //no current piece exists
        else{
            //Check if tile is occupied by a piece
            if (m_board.isOccupied(m_mouseTile) != nullptr){
                //if piece is players (not opponents)
                if (m_board.isOccupied(m_mouseTile)->colour() == m_turnColour){
                    //set current piece
                    setCurPiece(m_mouseCoords, m_mouseTile);
                } 
            }    
		}
	}
}

void MainGame::setCurPiece(glm::vec2 mouseCoords, glm::vec2 mouseTile){
    //Reset the mouse change to zero 
    m_mouseChange = glm::vec2(0,0);
                
    //record the initial click position
    m_mouseClickPos = mouseCoords;

    //set a new current piece
    m_curPiece = m_board.isOccupied(mouseTile);
    m_curPiece -> hold();
    m_curPiece -> clickOn();
    m_curPiece -> setDepth(6.0f);
    m_curPieceExists = true;
    
}

void MainGame::dropCurPiece(){
    //click off current piece
    m_curPiece -> clickOff();
    m_curPiece -> setDepth(2.0f);
    m_curPiece = nullptr;
    m_curPieceExists = false;
}

void MainGame::takeTurn(glm::vec2 mouseTile){
    int numMoves;
    
    //Move the piece
    m_board.movePiece(m_curPiece, mouseTile);

    //drop current piece
    dropCurPiece();

    //Update values
    m_moveNum += 1;
    m_turnColour = 1 - m_turnColour;
    numMoves = m_board.calculateNextTurnMoves(m_turnColour);
            
    if (m_autoflip) m_boardOrientation = m_turnColour;

    if (numMoves == -1) std::cout<<"DRAW!"<<std::endl;
    else if (numMoves == 0){
        if (m_turnColour == 0) std::cout<<"BLACK WINS!"<<std::endl;
        else std::cout<<"WHITE WINS"<<std::endl;
    }
    else{
        std::cout<<"##############################";
        if (m_turnColour == 0) std::cout<<" White's Turn ";
        else std::cout<< " Black's Turn ";
        std::cout<<"##############################"<<std::endl;
        std::cout<<"Legal Moves: "<<numMoves<<std::endl;
    }
}


void MainGame::drawGame(){

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_colourProgram.use();

	glActiveTexture(GL_TEXTURE0);

	GLint textureLocation = m_colourProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);

	//GLuint timeLocation = m_colourProgram.getUniformLocation("time");
	//glUniform1f(timeLocation, m_time);

	GLuint PLocation = m_colourProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();
	glUniformMatrix4fv(PLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_spriteBatch.begin(GlyphSortType::FRONT_TO_BACK);
	
    //Draw square objects
	m_background.draw(m_spriteBatch);
	m_border.draw(m_spriteBatch);
    m_flipBoardButton.draw(m_spriteBatch);
    m_autoFlipButton.draw(m_spriteBatch);

    if (m_isPromoting){
        //draw the promotion box background
        for (unsigned int i = 0; i < 4; i++){
            m_promoBoxes[i].draw(m_spriteBatch);
        }
        //draw the promotion box piece textures
        for (unsigned int i = 0; i < 4; i++){
            m_promoBoxes[4 + 4*m_turnColour + i].draw(m_spriteBatch);
        }
    }

    //Draw board
	m_board.draw(m_spriteBatch, m_boardOrientation);
	
	//m_spriteBatch.draw(pos, uv, texture.id, colour, 0.0f);
	
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	m_colourProgram.unuse();	

	m_window.swapBuffer();
}



