//
//  time.cpp
//  VideoShots
//
//  Created by Pedro Figueirêdo on 15/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "time.hpp"

Time::Time():
hours_{0},
minutes_{0},
seconds_{0},
miliseconds_{0}{}

Time::~Time(){}

Time::Time(int hours, int minutes, int seconds, int miliseconds):
hours_{hours},
minutes_{minutes},
seconds_{seconds},
miliseconds_{miliseconds}{}
