//
// Created by iris on 24/11/2019.
//

#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"
#include <typeinfo>
#include <string>

using namespace std;


/*Watchable*/
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length) {
    for(int i = 0; i < tags.size(); i++)
    {
        this->tags.push_back(tags[i]);
    }
}

int Watchable::getLength() { return this->length;}

int Watchable::getId() { return  this->id;}

std::vector<string>& Watchable::getTags() {return this->tags;}

bool Watchable::operator==(const Watchable &other) {
    if(typeid(this) == typeid(other))
    {
        if(other.id != this->id)
            return false;
        else if(other.length != this->length)
            return false;
        else return true;
    }
    return false;
}

/* Episode */
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode, const std::vector<std::string> &tags): Watchable(id,length,tags) {
    this->episode = episode;
    this->seriesName = seriesName;
    this->season = season;
    this->nextEpisodeId = id + 1; //TODO: check this
}

Watchable* Episode::getNextWatchable(Session &session) const {
    //check if the next episode exists
    for(int i=0;i<session.getContent().size();i++)
        if(session.getContent()[i]->getId()==this->nextEpisodeId)
            return session.getContent()[i];
    //otherwise
    return session.getActiveUser()->getRecommendation(session);
}

std::string Episode::toString() const {
    string msg = this->seriesName + "S" + to_string(this->season) + "E" + to_string(this->episode);
    return msg;
}


/*Movie*/
Movie::Movie(long id, const string &name, int length, const vector<std::string> &tags) : Watchable(id,length,tags){
    this->name=name;
}

Watchable * Movie::getNextWatchable(Session &session) const {
    return session.getActiveUser()->getRecommendation(session);
}

std::string Movie::toString() const {
    return this->name;
}