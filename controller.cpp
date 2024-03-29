//
//  controller.cpp
//  VideoShots
//
//  Created by Pedro Figueirêdo on 15/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include "controller.hpp"
#include <string>
#include <fstream>
#include <iostream>


Controller::Controller(){}

Controller::Controller(std::string shotDetectionScriptPath, std::string shotGenerationScriptPath, std::string keyframesExtractionScriptPath):
shotDetectionScriptPath_(shotDetectionScriptPath),
shotGenerationScriptPath_(shotGenerationScriptPath),
keyframesExtractionScriptPath_(keyframesExtractionScriptPath)
{}


Controller::~Controller(){}


/*  GENERATEKEYFRAMES
*
*   Method Definition:
*       - Method for extracting keyframes from a video whose location is indicated by inputPath;
*       - Keyframes will be files with jpg termination at the outputpath indicated;
*       - There's a third parameter which determinates how much frames per second will be extracted;
*       - Calls .sh file.
*   Return:
*        This method returns true if all keyframes were extracted and false otherwise.
*/

bool Controller::GenerateKeyFrames(std::string inputPath, std::string outputPath, int framesPerSecond)
{
    if (keyframesExtractionScriptPath_.empty()) {
        return false;
    }
    
    pid_t pid;
    char* args_ [5];
    
    args_[0] = *new char*((char*)keyframesExtractionScriptPath_.c_str());
    args_[1] = *new char*((char*)inputPath.c_str());
    args_[2] = *new char*((char*)outputPath.c_str());
    args_[3] = *new char*((char*)(std::to_string(framesPerSecond)).c_str());
    args_[4] = NULL;
    
    
    // Executes Script on Child Process and do not delay Parent.
    if ((pid = fork()) == 0) {
        execv(args_[0], args_);
        exit(0);
    }
    
    
    return true;
}


/*  GENERATESHOTS(1)
 *
 *   Method Definition:
 *       - Method for extracting all shots from a video whose location is indicated by inputPath;
 *       - Shots will be files with mp4 termination at the outputpath indicated;
 *       - Calls .sh file.
 *   Return:
 *        This method returns true if all shots were extracted and false otherwise.
 */
bool Controller::GenerateShots(std::string inputPath, std::string outputPath)
{
    std::string shots_srt_path_, scenes_srt_path;
    std::string shots_txt_path_, scenes_txt_path;
    std::string outputFile;
    std::string fileName;
    std::stringstream shotCounter;
    std::vector<VideoCuts> shots;
    
    // System call to detection script (Generates Files to be processed)
    if (!ExecuteShotsDetectionScript(inputPath))
        return false;
    
    // Get Generated Files Path
    shots_srt_path_ = getScriptFilesPath(inputPath, SHOTS_SRT);
    shots_txt_path_ = getScriptFilesPath(inputPath, SHOTS_TXT);
    scenes_srt_path = getScriptFilesPath(inputPath, SCENES_SRT);
    scenes_txt_path = getScriptFilesPath(inputPath, SCENES_TXT);
    
    //Load shots information on program
    if (!ComputeVideoCuts(shots, shots_srt_path_, shots_txt_path_)){
        return false;
    }

    // Delete Files generated by the script
    unlink(shots_srt_path_.c_str());
    unlink(shots_txt_path_.c_str());
    unlink(scenes_srt_path.c_str());
    unlink(scenes_txt_path.c_str());
    
    
    fileName = retrieveFileName(inputPath);
    
    outputPath.append(fileName + "_shots/");
    
    // Create Folder to Put Shots
    mkdir(outputPath.c_str(), S_IRWXU);
    
    
    for (int i = 0; i < shots.size(); i++) {
        
        shotCounter << "shot" << i << ".mp4";
        outputFile = outputPath + shotCounter.str();
        shotCounter.str("");
        
        if (!ExecuteShotsGenerationScript(inputPath,
                                     outputFile,
                                     getTimeInSeconds(shots[i].start_time_.hours_, shots[i].start_time_.minutes_, shots[i].start_time_.seconds_, shots[i].start_time_.miliseconds_),
                                     getTimeInSeconds(shots[i].end_time_.hours_, shots[i].end_time_.minutes_, shots[i].end_time_.seconds_, shots[i].end_time_.miliseconds_)
                                     ))
            return false;
    }
    
    return true;
}


/*  GENERATESHOTS(2)
 *
 *   Method Definition:
 *       - Method for extracting shot from a video whose location is indicated by inputPath;
 *       - Shot will be file with mp4 termination at the outputpath indicated;
 *       - Extract only ONE shot, whose index is passed on third parameter;
 *       - Calls .sh file.
 *   Return:
 *        This method returns true if shot was extracted and false otherwise.
 */
bool Controller::GenerateShots(std::string inputPath, std::string outputPath, int shotNumber)
{
    std::string shots_srt_path_, scenes_srt_path;
    std::string shots_txt_path_, scenes_txt_path;
    std::string outputFile;
    std::string fileName;
    std::stringstream shotCounter;
    std::vector<VideoCuts> shots;
    
    // System call to detection script (Generates Files to be processed)
     if (!ExecuteShotsDetectionScript(inputPath))
     return false;
    
    // Get Generated Files Path
    shots_srt_path_ = getScriptFilesPath(inputPath, SHOTS_SRT);
    shots_txt_path_ = getScriptFilesPath(inputPath, SHOTS_TXT);
    scenes_srt_path = getScriptFilesPath(inputPath, SCENES_SRT);
    scenes_txt_path = getScriptFilesPath(inputPath, SCENES_TXT);
    
    //Load shots information on program
    if (!ComputeVideoCuts(shots, shots_srt_path_, shots_txt_path_)){
        return false;
    }
    
    // Delete Files generated by the script
    unlink(shots_srt_path_.c_str());
    unlink(shots_txt_path_.c_str());
    unlink(scenes_srt_path.c_str());
    unlink(scenes_txt_path.c_str());
    
    fileName = retrieveFileName(inputPath);
    
    outputPath.append(fileName + "_shots/");
    
    // Create Folder to Put Shots
    mkdir(outputPath.c_str(), S_IRWXU);
    
    shotCounter << "shot" << shotNumber << ".mp4";
    outputFile = outputPath + shotCounter.str();
    shotCounter.str("");
        
    if (!ExecuteShotsGenerationScript(inputPath,
                                 outputFile,
                                 getTimeInSeconds(shots[shotNumber].start_time_.hours_, shots[shotNumber].start_time_.minutes_, shots[shotNumber].start_time_.seconds_, shots[shotNumber].start_time_.miliseconds_),
                                 getTimeInSeconds(shots[shotNumber].end_time_.hours_, shots[shotNumber].end_time_.minutes_, shots[shotNumber].end_time_.seconds_, shots[shotNumber].end_time_.miliseconds_)
                                 ))
        return false;
    
    return true;
}


/*  GENERATESHOTS(3)
 *
 *   Method Definition:
 *       - Method for extracting shots from a video whose location is indicated by inputPath;
 *       - Shots will be files with mp4 termination at the outputpath indicated;
 *       - Extract only shots within a range passed through third and fourth parameters;
 *       - Calls .sh file.
 *   Return:
 *        This method returns true if all shots were extracted and false otherwise.
 */
bool Controller::GenerateShots(std::string inputPath, std::string outputPath, int inicialShot, int finalShot )
{
    std::string shots_srt_path_, scenes_srt_path;
    std::string shots_txt_path_, scenes_txt_path;
    std::string outputFile;
    std::string fileName;
    std::stringstream shotCounter;
    std::vector<VideoCuts> shots;
    
    // System call to detection script (Generates Files to be processed)
     if (!ExecuteShotsDetectionScript(inputPath))
     return false;
    
    // Get Generated Files Path
    shots_srt_path_ = getScriptFilesPath(inputPath, SHOTS_SRT);
    shots_txt_path_ = getScriptFilesPath(inputPath, SHOTS_TXT);
    scenes_srt_path = getScriptFilesPath(inputPath, SCENES_SRT);
    scenes_txt_path = getScriptFilesPath(inputPath, SCENES_TXT);
    
    //Load shots information on program
    if (!ComputeVideoCuts(shots, shots_srt_path_, shots_txt_path_)){
        return false;
    }
    
    // Delete Files generated by the script
    unlink(shots_srt_path_.c_str());
    unlink(shots_txt_path_.c_str());
    unlink(scenes_srt_path.c_str());
    unlink(scenes_txt_path.c_str());
    
    fileName = retrieveFileName(inputPath);
    
    outputPath.append(fileName + "_shots/");
    
    // Create Folder to Put Shots
    mkdir(outputPath.c_str(), S_IRWXU);
    
    
    for (int i = inicialShot; i <= finalShot; i++) {
        
        shotCounter << "shot" << i << ".mp4";
        outputFile = outputPath + shotCounter.str();
        shotCounter.str("");
        
        if(!ExecuteShotsGenerationScript(inputPath,
                                     outputFile,
                                     getTimeInSeconds(shots[i].start_time_.hours_, shots[i].start_time_.minutes_, shots[i].start_time_.seconds_, shots[i].start_time_.miliseconds_),
                                     getTimeInSeconds(shots[i].end_time_.hours_, shots[i].end_time_.minutes_, shots[i].end_time_.seconds_, shots[i].end_time_.miliseconds_)
                                     ))
            return false;
    }
    
    return true;
}


/*
 *   SCRIPT FILES ARE CALLED THROUGH THESE METHODS
 */

bool Controller::ExecuteShotsGenerationScript(std::string inputPath,std::string outputPath, float initialTime, float endTime)
{
    char* args_[6];
    std::string beginTime, duration;
    std::stringstream floatToString;
    pid_t pid;
    
    if (shotGenerationScriptPath_.empty()) {
        return false;
    }
    
    floatToString << initialTime;
    beginTime = floatToString.str();
    floatToString.str("");
    
    floatToString << (float) endTime - initialTime;
    duration = floatToString.str();
    
    args_[0] = *new char*((char*)shotGenerationScriptPath_.c_str());
    args_[1] = *new char*((char*)inputPath.c_str());
    args_[2] = *new char*((char*)outputPath.c_str());
    args_[3] = *new char*((char*)beginTime.c_str());
    args_[4] = *new char*((char*)duration.c_str());
    args_[5] = NULL;
    
    // Fork parent process and redirect child to script
    if ((pid = fork()) == 0) {
        execv(args_[0], args_);
        exit(0);
    }
    
    return true;
}

bool Controller::ExecuteShotsDetectionScript(std::string videoPath)
{
    std::string cmd;
    
    if (shotDetectionScriptPath_.empty()) {
        return false;
    }
    
    cmd = shotDetectionScriptPath_ + " " + videoPath;
    
    system(cmd.c_str());
    
    return true;
}


/*
*   INTERNAL METHODS FOR DATA MANAGMENT
*/

bool Controller::ComputeVideoCuts(std::vector<VideoCuts> &videoCuts, std::string srt_path, std::string txt_path)
{
    std::vector<Time> start_time, end_time;
    std::vector<int> start_frame, end_frame;
    std::vector<std::vector<int>> keyframes;
    unsigned long numOfCuts;
    
    if (!getTimes(start_time, end_time, srt_path)) return false;
    
    if (!getFrames(start_frame, end_frame, keyframes, txt_path)) return false;
    
    // check if number of shots are the same on all components
    if (start_time.size() == end_time.size() && start_frame.size() == end_frame.size() &&
        start_frame.size() == keyframes.size()) {
        numOfCuts = start_time.size();
    }else return false;
    
    for (unsigned i = 0; i < numOfCuts; i++) {
        videoCuts.push_back(VideoCuts(start_frame[i],end_frame[i],keyframes[i],start_time[i], end_time[i]));
    }
    
    return true;
}

bool Controller::getTimes(std::vector<Time> &start_time, std::vector<Time> &end_time, std::string path)
{
    std::string line, times;
    std::ifstream srtfile (path);
    if (srtfile.is_open())
    {
        while ( getline (srtfile,line) )
        {
            if ( (line.size() == 29 && line[0] >= '0' && line[0] <= '9') ) {
                for (auto it = line.begin() ; it != line.end(); it++) {
                    if (*it >= '0' && *it <= '9') {
                        times.push_back(*it);
                    }
                }
                start_time.push_back(Time(stoi(times.substr(0,2)), stoi(times.substr(2,2)),
                                          stoi(times.substr(4,2)), stoi(times.substr(6,3))));
                end_time.push_back(Time(stoi(times.substr(9,2)), stoi(times.substr(11,2)),
                                        stoi(times.substr(13,2)), stoi(times.substr(15,3))));
                
                times.clear();
            }
        }
        srtfile.close();
    }
    
    else return false;
    
    return true;
    
}

bool Controller::getFrames(std::vector<int> &start_frame, std::vector<int> &end_frame,
                           std::vector<std::vector<int> > &keyframes, std::string path)
{
    std::string line, frame;
    std::ifstream txtfile (path);
    std::vector<int> keyframe;
    int counter = 0;
    if (txtfile.is_open())
    {
        while ( getline (txtfile,line) )
        {
            for (auto it = line.begin(); it != line.end(); it++) {
                if (*it != ' ') {
                    frame.push_back(*it);
                }else{
                    switch (counter) {
                        case 0:
                            start_frame.push_back(stoi(frame));
                            break;
                        case 1:
                            end_frame.push_back(stoi(frame));
                            break;
                        default:
                            keyframe.push_back(stoi(frame));
                            break;
                    }
                    counter++;
                    frame.clear();
                }
            }
            keyframe.push_back(stoi(frame));
            keyframes.push_back(keyframe);
            keyframe.clear();
            frame.clear();
            counter = 0;
        }
        txtfile.close();
    }
    
    else return false;
    
    return true;
}

std::string Controller::getScriptFilesPath(std::string inputPath, int fileType){
    
    std::string fileName;
    
    //It's a zero-based operation, so gets + 1
    size_t location = inputPath.find_last_of('/', inputPath.size()) + 1;
    
    //Retrieve FileName
    fileName = retrieveFileName(inputPath);
    
    // Update inputPath to its parent folder ("cd ..")
    inputPath.erase(inputPath.begin() + location, inputPath.end());
    
    switch (fileType) {
        case SHOTS_SRT:
            return inputPath + fileName + "_shots.srt";
            break;
        case SHOTS_TXT:
            return inputPath + fileName + "_shots.txt";
            break;
        case SCENES_SRT:
            return inputPath + fileName + "_scenes.srt";
            break;
        case SCENES_TXT:
            return inputPath + fileName + "_scenes.txt";
            break;
        default:
            break;
    }
    
    return std::string();
}

std::string Controller::retrieveFileName(std::string inputPath){
    std::string fileName;
    
    //It's a zero-based operation, so gets + 1
    size_t location = inputPath.find_last_of('/', inputPath.size()) + 1;
    
    // Extract fileName from inputPath
    for (size_t count = location; count < inputPath.size(); count++) {
        fileName.push_back(inputPath[count]);
    }
    
    // Extract File Name with no extension (without .xxx)
    location = fileName.find_last_of('.', fileName.size());
    fileName.erase(fileName.begin() + location, fileName.end());
    
    return fileName;
}

float Controller::getTimeInSeconds(int hours, int minutes, int seconds, int miliseconds) {
    
    float time = (float)(hours*3600 + minutes*60 + seconds + ((float)miliseconds/1000));
    
    return time;
}




