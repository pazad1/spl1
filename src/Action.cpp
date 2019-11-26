//
// Created by iris on 24/11/2019.
//
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include <string>
#include <vector>

using namespace std;

BaseAction::BaseAction() :errorMsg(""), status(PENDING){}

ActionStatus BaseAction::getStatus() const {
    return this->status;
}

void BaseAction::complete() {
    this->status = COMPLETED;
    this->errorMsg = "";
}

void BaseAction::error(const std::string &errorMsg) {
    this->status = ERROR;
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const { return this->errorMsg;}

/* Create User */
void CreateUser::act(Session &sess) {
    string* input = sess.getActionInput();
    //check valid input
    if(input[1].compare("len") != 0 | input[1].compare("rer") != 0 | input[1].compare("gen") != 0)
    {
        this->error("3-letter code is invalid - no such algorithm!");
        return;
    }
    if(sess.isUserExists(input[0]))
    {
        this->error("There is already user with that name!");
        return;
    }


    //all the parameters are fine - create new user
    User* newUser;
    if(input[1].compare("len") == 0 )
    {
        newUser = new LengthRecommenderUser(input[0]);
    }
    else if(input[1].compare("rer") == 0 )
    {
        newUser = new RerunRecommenderUser(input[0]);
    }
    else
    {
        newUser = new GenreRecommenderUser(input[0]);
    }
    sess.addNewUser(input[0], newUser);

    //clear memory
    delete(input);
}

void DeleteUser::act(Session &sess) {
    string* input = sess.getActionInput();
    //check input
    if(!sess.isUserExists(input[0]))
    {
        this->error("The user that you wish to delete does not exists!");
        return;
    }

    //delete user
    sess.deleteUser(input[0]);

    //clear memory
    delete(input);
}

void DuplicateUser::act(Session &sess) {
    string* input = sess.getActionInput();

    string newUserName = input[1];
    string originalUserName = input[0];

    //check input
    if(!sess.isUserExists(originalUserName))
    {
        string errorMsg = "No such user " + originalUserName;
        this->error(errorMsg);
        return;
    }
    else if(sess.isUserExists(newUserName))
    {
        string errorMsg = "The user " + newUserName + " already exists!";
        this->error(errorMsg);
        return;
    }

    //duplicate the user
    sess.addNewUser(newUserName, sess.getUser(originalUserName));
    //clear memory
    delete(input);
}

void PrintWatchHistory::act(Session &sess) {
    cout << "Watch history for " << sess.getActiveUser()->getName();
    cout << " Where " << sess.getActiveUser()->getName() << " is the name of the current active user" << endl;
    int size = sess.getContent().size();
    for(int i = size - 1, j = 1; i >= 0 & j <= size; i--, j++)
    {
        cout << j << ". " << sess.getContent().at(i)->toString() << endl;
    }
}

void PrintActionsLog::act(Session &sess) {
    //printing from the last to first - in vector the first place is the last action
    for(int i = 0; i < sess.getActionsLog().size(); i++) {
        cout << sess.getActionsLog().at(i)->toString() << endl;
    }
}

/*Change active user*/
void ChangeActiveUser::act(Session &sess) {
    string* input = sess.getActionInput();
    //check valid input
    if(!sess.isUserExists(input[0]))
    {
        this->error("There is no such user");
        return;
    }

    //change active user
    sess.setActiveUser(input[0]);

    //clear memory
    delete(input);
}

/*Print content list*/
void PrintContentList::act(Session &sess) {
    vector<Watchable*> content=sess.getContent();
    //print content list
    for(int i=0;i<content.size();i++)
    {
        string id=i+". ";
        string name=content[i]->toString()+" ";
        string length=content[i]->getLength()+" minutes ";
        vector<string> vecTags=content[i]->getTags();
        string tags="[";
        for(int j=0;j<vecTags.size();j++)
        {
            tags+=vecTags[j];
            if(j!=vecTags.size()-1)
                tags+=", ";
        }
        tags+="]";

        cout<<id << name << length << tags << endl;
    }
}



/*Watch*/
void Watch::act(Session &sess) {
    string* input=sess.getActionInput();
    vector<Watchable*> contents=sess.getContent();

    //print watch command
    cout<<"Watching " << contents[stoi(input[0])]->toString() <<endl;

    //adds content to watching history
    sess.addContentToHistory(*contents[stoi(input[0])]);

    //recommendation
    User* user=sess.getActiveUser();
    Watchable* recommend = user->getRecommendation(sess);
    cout<<"We recommend watching " << recommend->toString() << ", continue watching? [y/n]" << endl;


    string continueWatching;
    cin >> continueWatching;
    //Watch recommended content
    if(continueWatching.compare("y"))
    {
        int index = 0;
        for (auto & elem : contents)
        {
            if (elem == recommend)
            {
                break;
            }
            index++;
        }
        sess.setInputForWatch(index);
        this->act(sess);
    }

    //end watching

    //clear memory
    delete(input);
    delete(user);
    delete(recommend);
}

/*Exit*/
void Exit::act(Session &sess) {
    return;
}

std::string CreateUser::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
       msg = "Create new User Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Create new User COMPLETED";
    else
        msg = "Create new User PENDING";
    return msg;
}

std::string ChangeActiveUser::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Change Active User Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Change Active User COMPLETED";
    else
        msg = "Change Active User PENDING";
    return msg;
}

std::string DeleteUser::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Delete User Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Delete User COMPLETED";
    else
        msg = "Delete User PENDING";
    return msg;
}
std::string DuplicateUser::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Duplicate User Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Duplicate User COMPLETED";
    else
        msg = "Duplicate User PENDING";
    return msg;
}
std::string PrintContentList::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Print Content List Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Print Content List COMPLETED";
    else
        msg = "Print Content List PENDING";
    return msg;
}
std::string PrintWatchHistory::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Print Watch History Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Print Watch History COMPLETED";
    else
        msg = "Print Watch History PENDING";
    return msg;
}
std::string Watch::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Watch Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Watch COMPLETED";
    else
        msg = "Watch PENDING";
    return msg;
}
std::string PrintActionsLog::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Print Actions Log Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Print Actions Log COMPLETED";
    else
        msg = "Print Actions Log PENDING";
    return msg;
}

std::string Exit::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Exit Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Exit COMPLETED";
    else
        msg = "Exit PENDING";
    return msg;
}