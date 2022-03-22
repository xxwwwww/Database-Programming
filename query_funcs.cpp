#include "query_funcs.h"
#include <iomanip>

void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    work W(*C);
    stringstream statement;
    statement<<"INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG,"
             <<"PPG, RPG, APG, SPG, BPG) VALUES ("<<team_id<<", "<<jersey_num<<", "
             <<W.quote(first_name)<<", "<<W.quote(last_name)<<", "<<mpg<<", "<<ppg
             <<", "<<rpg<<", "<<apg<<", "<<spg<<", "<<bpg<<");";
    W.exec(statement.str());
    W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    work W(*C);
    stringstream command;
    command << "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES ("
              <<W.quote(name)<<", "<<state_id<<", "<<color_id<<", "<<wins<<", "<<losses<<");";
    W.exec(command.str());
    W.commit();
}


void add_state(connection *C, string name)
{
    work W(*C);
    stringstream command;
    command << "INSERT INTO STATE (NAME) VALUES ("<<W.quote(name)<<");";
    W.exec(command.str());
    W.commit();
}


void add_color(connection *C, string name)
{
    work W(*C);
    stringstream command;
    command << "INSERT INTO COLOR (NAME) VALUES ("<<W.quote(name)<<");";
    W.exec(command.str());
    W.commit();
}



void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
   
    int use_flag[6] = {use_mpg, use_ppg, use_rpg, use_apg, use_spg, use_bpg};
    int min1[4] = {min_mpg, min_ppg, min_rpg, min_apg};
    int max1[4] = {max_mpg, max_ppg, max_rpg, max_apg};
    double min2[2] = {min_spg, min_bpg};
    double max2[2] = {max_spg, max_bpg};
    string value_name[6] = {"MPG", "PPG", "RPG", "APG", "SPG", "BPG"};
    int i = 0;
    bool flag = true;
     stringstream command;
    command << "SELECT * "
           "FROM PLAYER ";
    
    while(i<6){
       if(use_flag[i] && flag){
            command << "WHERE ";
            flag = false;
            if(i<4){
                command <<value_name[i] << " >= " << min1[i] << " AND " << value_name[i]
                        << " <= " << max1[i];
            }
            else{
                command << value_name[i] << " >= " << min2[i-4] << " AND " << value_name[i]
                        << " <= " << max2[i-4];
            }
        }
       else if(use_flag[i]){
            command << " AND ";
            if(i<4){
                command <<value_name[i] << " >= " << min1[i] << " AND " << value_name[i]
                        << " <= " << max1[i];
            }
            else{
                command << value_name[i] << " >= " << min2[i-4] << " AND " << value_name[i]
                        << " <= " << max2[i-4];
            }
        }
        i++;
    }
    command << ";";
    nontransaction N(*C);
    result R(N.exec(command.str()));
    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"
         << endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<int>() << " " << c[1].as<int>() << " " << c[2].as<int>() << " "
             << c[3].as<string>() << " " << c[4].as<string>() << " " << c[5].as<int>() << " "
             << c[6].as<int>() << " " << c[7].as<int>() << " " << c[8].as<int>() << " "
             << fixed << setprecision(1) << c[9].as<double>() << " " << c[10].as<double>()
             << endl;
    }
}


void query2(connection *C, string team_color)
{
    string command;
    work W(*C);
    command = "SELECT TEAM.NAME "
                "FROM TEAM, COLOR "
                "WHERE COLOR.NAME = \'" + team_color + "\' AND TEAM.COLOR_ID = COLOR.COLOR_ID;";
    W.commit();
    nontransaction N(*C);
    result R(N.exec(command));
    cout<<"NAME"<<endl;
    for(result::iterator i = R.begin(); i!=R.end();i++){
        cout<<i[0].as<string>()<<endl;
    }
}


void query3(connection *C, string team_name)
{
    string command;
    work W(*C);
    command =  "SELECT FIRST_NAME, LAST_NAME "
                 "FROM PLAYER, TEAM "
                 "WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.NAME = \'" + team_name+ "\' ORDER BY PPG DESC;";
    W.commit();
    nontransaction N(*C);
    result R(N.exec(command));
    cout<<"FIRST_NAME LAST_NAME"<<endl;
    for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
        cout << c[0].as<string>() << " " << c[1].as<string>() << endl;
    }
}


void query4(connection *C, string team_state, string team_color)
{
    string command;
    work W(*C);
    command = "SELECT PLAYER.UNIFORM_NUM, PLAYER.FIRST_NAME, PLAYER.LAST_NAME "
                "FROM PLAYER, TEAM, STATE, COLOR "
                "WHERE STATE.NAME = \'" + team_state + "\' AND COLOR.NAME = \'" + team_color +"\' AND TEAM.STATE_ID = STATE.STATE_ID AND TEAM.COLOR_ID = COLOR.COLOR_ID AND PLAYER.TEAM_ID = TEAM.TEAM_ID;";
    W.commit();
    nontransaction N(*C);
    result R(N.exec(command));
    cout<<"UNIFORM_NUM FIRST_NAME LAST_NAME"<<endl;
    for(result::iterator i = R.begin(); i!=R.end();i++){
        cout<<i[0].as<int>()<<" "<<i[1].as<string>()<<" "<<i[2].as<string>()<<endl;
    }
}


void query5(connection *C, int num_wins)
{
    string command;
    work W(*C);
    command = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS "
                "FROM PLAYER, TEAM "
                "WHERE TEAM.WINS > \'" + to_string(num_wins) +"\'AND PLAYER.TEAM_ID = TEAM.TEAM_ID;";
    W.commit();
    nontransaction N(*C);
    result R(N.exec(command));
    cout<<"FIRST_NAME LAST_NAME NAME WINS"<<endl;
    for(result::iterator i = R.begin(); i!=R.end();i++){
        cout<<i[0].as<string>()<<" "<<i[1].as<string>()
            <<" "<<i[2].as<string>()<<" "<<i[3].as<int>()<<endl;
    }
}
