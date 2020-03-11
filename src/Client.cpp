//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "common.hpp"
#include "Client.hpp"
#include <string>
#include <sstream>

Client::~Client() {
}


void Client::initialize(unsigned int player, unsigned int board_size){
    this->player= player;
    this->board_size= board_size;

    //create player#.action_board.json
    ostringstream os;
    os << "player_" << player << ".action_board.json";
    string fileName= os.str();
    std::ofstream file(fileName);
    cereal::JSONOutputArchive archive(file);

    //action_board is a vector<vector<int>> and is initialized to board_size rows of 0's
    std::vector<int> line;
    line.assign(board_size, 0);
    std::vector<std::vector<int>> board;
    board.assign(board_size, line);

    archive(CEREAL_NVP(board));


    this->initialized= true;
}


void Client::fire(unsigned int x, unsigned int y) {
    ostringstream os;
    os << "player_" << player << ".shot.json";
    string fileName= os.str();
    std::ofstream file(fileName);
    cereal::JSONOutputArchive archive(file);
    archive(CEREAL_NVP(x), CEREAL_NVP(y));
}


bool Client::result_available() {
    ostringstream os;
    os << "player_" << player << ".result.json";
    string fileName= os.str();
    std::ifstream shot(fileName);
    if (!shot) {
        return false;
    }
    else {
        return true;
    }
}


int Client::get_result() {
    if (result_available()) {
        ostringstream os;
        os << "player_" << player << ".result.json";
        string fileName = os.str();
        std::ifstream shot(fileName);

        cereal::JSONInputArchive ar(shot);
        int result;
        ar(result);

        remove(const_cast<char*>(("player_" + to_string(player) + ".result.json").c_str()));

        switch(result) {
            case HIT:
                return HIT;
            case MISS:
                return MISS;
            case OUT_OF_BOUNDS:
                return OUT_OF_BOUNDS;
            default:
                throw ClientException("Bad result");
        }
    }
}



void Client::update_action_board(int result, unsigned int x, unsigned int y) {
    ostringstream os;
    os << "player_" << player << ".action_board.json";
    string fileName= os.str();
    std::ifstream file(fileName);
    cereal::JSONInputArchive ar(file);

    std::vector<std::vector<int>> board;
    ar(board);

    board[x][y]= result;

    std::ofstream outFile(fileName);
    cereal::JSONOutputArchive archive(outFile);
    archive(CEREAL_NVP(board));
}


string Client::render_action_board(){
    ostringstream os;
    os << "player_" << player << ".action_board.json";
    string fileName= os.str();
    std::ifstream file(fileName);
    cereal::JSONInputArchive ar(file);

    std::vector<std::vector<int>> board;
    ar(board);

    ostringstream out;
    out << " ";
    for (int i= 0; i< board_size; i++) {
        for (int j= 0; j< board_size; j++) {
            out << board[j][i] << " ";
        }
        out << "\n ";
    }
    return out.str();
}