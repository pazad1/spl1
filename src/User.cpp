//
// Created by iris on 24/11/2019.
//

#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <vector>
#include <map>
using namespace std;

User::User(const std::string &name) : name(name){}

std::string User::getName() const {return this->name;}

std::vector<Watchable*> User::get_history() const {return this->history;}

User::~User() {
    int size = this->history.size();
    for(int i = 0; i < size; i++)
    {
        delete(this->history[i]);
    }
    vector<Watchable*> temp;
    this->history.swap(temp); //to delete the vector
}



/* LengthRecommenderUser */

LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name){}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    //calculating the average of the contents' length
    int size = s.getContent().size();
    double avg = 0.0;
    int closest = 0, index = -1;
    for(int i = 0; i < size; i++)
    {
        avg += s.getContent().at(i)->getLength();
    }
    avg /= size;

    //finding the closest movie/episode by length to the average
    for(int i = 0; i < size; i++)
    {
        if(s.getContent().at(i)->getLength() > closest & s.getContent().at(i)->getLength() <= avg)
        {
            index = i;
            closest = s.getContent().at(i)->getLength();
        }
    }
    if(index == -1)//didn't find any movie/episode
        return nullptr;
    return s.getContent()[index];
}



/*RerunRecommenderUser*/
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name){
    this->index=-1;
}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {

    //the user didn't watch any content
    if(this->history.size()==0)
        return nullptr;
    //otherwise
    else if(this->index==-1)
        this->index=0;
    else
        index=(this->index+1)%(this->history.size());
    return this->history[index];

}



/*GenreRecommenderUser*/
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name){}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    //the user didn't watch any content
    if(this->history.size()==0)
        return nullptr;

    //create a map of tags and their popularity
   map<string,int> tags;
    int size = s.getContent().size();
    for(int i=0;i< size; i++)
    {
        int sizei = s.getContent()[i]->getTags().size();
        for(int j = 0 ;j < sizei;j++)
        {
            if(tags.count(s.getContent()[i]->getTags()[j])!=0)
                tags[s.getContent()[i]->getTags()[j]]+=1;
            else
                tags.insert(pair<string,int>(s.getContent()[i]->getTags()[j],1));
        }
    }

    //sort the tags by their popularity
    vector<pair<string,int>> vec=this->sortTags(tags);


    //get recommendation

    //for each tag
    for(int i=0;i<vec.size();i++)
    {
        string tag=vec[i].first;

        //for each content
        for(int j=0;j<size;j++)
        {
            //check if this content doesn't belong to the user's history
            if (std::find(s.getActiveUser()->get_history().begin(), s.getActiveUser()->get_history().end(),s.getContent()[j])==s.getActiveUser()->get_history().end())
            {
                //check if this content contains tag
                if(std::find(s.getContent()[j]->getTags().begin(),s.getContent()[j]->getTags().end(),tag)!=s.getContent()[j]->getTags().end())
                {
                    //clear memory
                    delete(vec);
                    delete(tags);

                    //return recommendation
                    return s.getContent()[j];
                }
            }
        }
    }

    //there is no content to be recommended
    return nullptr;


}



vector<pair<string, int>> GenreRecommenderUser::sortTags(map<string, int> &map) {
    //create an empty vector of pairs
    vector<pair<string,int>> vec;

    //copy the pairs from the map to the vector
    std::copy(map.begin(),map.end(),std::back_inserter<std::vector<pair<string,int>>>(vec))

    //sort
    std::sort(vec.begin(), vec.end(),
              [](const pair& l, const pair& r) {
                  if (l.second != r.second)
                      return l.second > r.second;

                  return l.first > r.first;
              });

    //return sorted vector
    return vec;
}


