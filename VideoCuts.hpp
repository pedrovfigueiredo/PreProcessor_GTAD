//
//  VideoCuts.hpp
//  VideoShots
//
//  Created by Pedro Figueirêdo on 15/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef VideoCuts_hpp
#define VideoCuts_hpp
#include <vector>
#include "time.hpp"

class VideoCuts
{
public:
    
    int start_frame_;
    int end_frame_;
    Time start_time_;
    Time end_time_;
    std::vector<int> keyframes_;
    
    VideoCuts();
    VideoCuts(int start_frame, int end_frame, std::vector<int> keyframes, Time start_time, Time end_time);
    ~VideoCuts();
};

#endif /* VideoCuts_hpp */
