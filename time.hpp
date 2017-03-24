//
//  time.hpp
//  VideoShots
//
//  Created by Pedro Figueirêdo on 15/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef time_hpp
#define time_hpp
#include <string>

class Time
{
public:
    
    int miliseconds_;
    int seconds_;
    int minutes_;
    int hours_;
    
    Time();
    Time(int hours, int minutes, int seconds, int miliseconds);
    ~Time();
};

#endif /* time_hpp */
