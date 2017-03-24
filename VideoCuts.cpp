//
//  VideoCuts.cpp
//  VideoShots
//
//  Created by Pedro Figueirêdo on 15/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "VideoCuts.hpp"

VideoCuts::~VideoCuts( void ){}

VideoCuts::VideoCuts():
start_frame_{0},
end_frame_{0},
keyframes_{std::vector<int>()},
start_time_{Time()},
end_time_{Time()}{}


VideoCuts::VideoCuts(int start_frame, int end_frame, std::vector<int> keyframes, Time start_time, Time end_time):
start_frame_{start_frame},
end_frame_{end_frame},
keyframes_{keyframes},
start_time_{start_time},
end_time_{end_time}{}
