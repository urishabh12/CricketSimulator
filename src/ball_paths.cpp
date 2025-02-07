// Dhoni Six Ball
// void Ball::updateBall(std::pair<int, int> bowlerPosition, std::pair<int, int> batsmanPosition) {
//     if (this->currMovementIndex == -1) {
//         //Bowler to batsman
//         int deltaX = (3000 - this->height) / 2;
//         int deltaY = (3000 - this->width) / 2;
//         int x1 = deltaX + bowlerPosition.first, y1 = deltaY + bowlerPosition.second, 
//                     x3 = deltaX + batsmanPosition.first, y3 = deltaY + batsmanPosition.second;
//         int x2 = (x3 + x1) / 2 - 30;
//         int y2 = (y3 + y1) / 2 + 30;
//         this->linearInterpolation3Points(x1, y1, x2, y2, x3, y3, 1);
//         this->currMovementIndex = 0;
//         this->lastFrameTime = QuickCG::getTicks();
//         this->currMovementDuration = 1000;

//         //Batsman to boundary
//         int x4 = deltaX + 800, y4 = deltaY + 800;
//         this->quadraticInterpolation(x3, y3, x4, y4, -1800, 500);
//     }

//     if (this->currMovementIndex >= this->movementInterpolated.size()) {
//         this->currMovementDuration = 720;
//         this->currMovementIndex = 0;
//     }

//     if (this->currMovementIndex >= this->movementInterpolated.size() / 2) {
//         this->currMovementDuration = 2500;
//     }

//     int frameChangeInterval = this->currMovementDuration/24;
//     long getCurrTime = QuickCG::getTicks();
//     long timeDiff = getCurrTime - this->lastFrameTime;
//     //std::cout << frameChangeInterval << ' ' << timeDiff << std::endl;

//     if (timeDiff > frameChangeInterval) {
//         this->currMovementIndex += ceil(timeDiff / frameChangeInterval);
//         this->lastFrameTime = getCurrTime;
//     }


//     this->worldPosX = this->movementInterpolated[this->currMovementIndex].first;
//     this->worldPosY = this->movementInterpolated[this->currMovementIndex].second;
// }