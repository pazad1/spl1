//
// Created by iris on 24/11/2019.
//
#include "../include/Session.h"
#include "../include/json.hpp"
#include "../include/User.h"
#include <fstream>
#include <string>
#include "../include/Watchable.h"
using namespace std;
using namespace nlohmann;

Session::Session(const std::string &configFilePath) {

    ifstream json_file(configFilePath, std::ifstream::binary);
    json json;
    json_file >> json;

    int i = 0;
    for(; json["movies"][i] != nullptr; i++)
    {
        vector<string> tags;
        for(int j = 0; json["movies"][i]["tags"][j] != nullptr; j++)
        {
            tags.push_back(json["movies"][i]["tags"][j]);
        }
        Watchable* w = new Movie(i,json["movies"][i]["name"],json["movies"][i]["length"],tags);
        this->content.push_back(w);
    }

    for(int t = i, k = 0; json["tv_series"][k] != nullptr; k++, t++)
    {
        vector<string> tags2;
        for(int j = 0; json["tv_series"][i]["tags"][j] != nullptr; j++)
        {
            tags2.push_back(json["tv_series"][i]["tags"][j]);
        }
        auto seasons = json["seasons"].get<std::vector<int>>();
        for(int j = 0; json["tv_series"][i]["seasons"][j] != nullptr; j++)//seasons in every series
        {
            for(int k = 1; k <= j; k++)//episodes in each season
            {
                Episode* e = new Episode(t, json["tv_series"][i]["name"], json["tv_series"][i]["episode_length"], j, k, tags2) ;
                this->content.push_back(e);
            }

        }
    }
}
string* Session::getActionInput() {return this->actionInput;}

User*& Session::getActiveUser() { return this->activeUser;}

vector<Watchable*>& Session::getContent() {return this->content;}

User*& Session::getUser(string &userName) {return this->userMap[userName];}

void Session::addNewUser(string &userName, User *user) {
    this->userMap.insert(pair<string,User*>(userName,user));
}

bool Session::isUserExists(string &userName) {
    if(this->userMap.find(userName) != this->userMap.end())
        return true;
    return false;
}

bool Session::deleteUser(string &userName) {
    return this->userMap.erase(userName);
}

vector<BaseAction*>& Session::getActionsLog() {return this->actionsLog;}

void Session::setActiveUser(string &userName) {this->activeUser = this->userMap[userName];}

void Session::addContentToHistory(Watchable &content) {this->activeUser->get_history().push_back(&content);}

Session::~Session() {
    delete(this->activeUser);
    delete(this->actionInput);
    //delete the vectors
}

void Session::setInputForWatch(int id) {
    this->actionInput[0]=id;
}