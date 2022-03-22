#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include <vector>
#include "exerciser.h"
#include <sstream>
#include "query_funcs.h"

using namespace std;
using namespace pqxx;

void read_file(const char *fileName){


}

void executeQuery(vector<string> content, connection *C){
    std::string s;
    for (vector<string>::const_iterator i = content.begin(); i != content.end(); ++i) {
        s += *i;
    }
    work W(*C);
    W.exec(s);
    W.commit();
}
void execute_file(const char *fileName, connection *C, int type ){
    string line;
    ifstream myfile (fileName);
    if (myfile.is_open()){
        switch (type) {
            default :
                cout << "wrong argument";
                break;
            case 0: {
                string content;
                while (getline(myfile, line)) {
                    content = "DROP TABLE IF EXISTS " + line + " CASCADE;";
                    work W(*C);
                    W.exec(content);
                    W.commit();
                }
                break;
            }
            case 1: {
                vector<string> content;
                while (getline(myfile, line)) {
                    content.push_back(line);
                }
                executeQuery(content, C);
                break;
            }
            case 2:{
//                cout<<"get into the case 2"<<endl;
                double player_id, team_id, uniform_num, mpg, ppg, rpg, apg, spg, bpg;
                string first_name, last_name;
                while(getline(myfile,line)){
                stringstream converter;
                converter<<line;
                converter>>player_id>>team_id>>uniform_num>>first_name>>last_name>>mpg>>ppg>>rpg>>apg>>spg>>bpg;
//                    cout<<"add_player function"<<endl;
                add_player(C,team_id,uniform_num,first_name,last_name,mpg,ppg,rpg,apg,spg,bpg);
                }
                break;
            }
            case 3:{
                int id;
                string name;
                while(getline(myfile,line)){
                    stringstream converter;
                    converter << line;
                    converter >> id >> name;
                    add_state(C, name);
                }
                break;
            }
            case 4:{
                int team_id, state_id, color_id, wins, losses;
                string name;
                while(getline(myfile,line)){
                    stringstream converter;
                    converter<<line;
                    converter>>team_id>>name>>state_id>>color_id>>wins>>losses;
                    add_team(C,name,state_id,color_id,wins,losses);
                }
                break;
            }
            case 5:{
                int id;
                string name;
                while(getline(myfile,line)){
                    stringstream converter;
                    converter << line;
                    converter >> id >> name;
                    add_color(C, name);
                }
                break;
            }
        }
        myfile.close();
        return;
    }
    else{
        cout << "Unable to open file" << fileName;
        return;
    }

};




int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }
    //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
    //      load each table with rows from the provided source txt files


//drop table
    execute_file("dropTables.txt", C, 0 );
//create table
    execute_file("createTables.txt", C, 1 );
    //add player.txt
    execute_file("player.txt", C, 2 );
    //add state.txt
    execute_file("state.txt", C, 3 );
    //add team.txt
    execute_file("team.txt", C, 4 );
    //add color.txt
    execute_file("color.txt", C, 5 );

  exercise(C);
  //Close database connection
  C->disconnect();

  return 0;
}



