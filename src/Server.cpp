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
#include "Server.hpp"


/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(std::ifstream file){
    if (!file) {
        return -1;
    }

    file.seekg(0, std::ios::end);
    int len= file.tellg();
    return len;
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board){

    this->p1_setup_board= scan_setup_board(p1_setup_board);
    this->p2_setup_board= scan_setup_board(p2_setup_board);
    if (!this->p1_setup_board || !this->p2_setup_board) {
        throw ServerException("File(s) not found");
    }
    this->board_size= board_size;

    //file length should be board_size^2 + board_size to account for the extra newline
    if ((get_file_length(std::ifstream(p1_setup_board))) != (board_size * board_size + board_size)) {
        throw ServerException("Bad board size- p1");
    }
    if (get_file_length(std::ifstream(p2_setup_board)) != (board_size * board_size + board_size)) {
        throw ServerException("Bad board size- p2");
    }

}


Server::~Server() {
}


BitArray2D *Server::scan_setup_board(string setup_board_name){
    BitArray2D *bitBoard = new BitArray2D(board_size, board_size);

//    std::ifstream board(setup_board_name);
//    char c;
//
//    for (int i= 0; i< board_size; i++) {
//        for (int j= 0; j< board_size; j++) {
//            board.seekg(i * (board_size + 1) + j, ios::beg);
//            board.get(c);
//
//            bool hit= c == 'C' || c == 'B' || c == 'R' || c == 'S' || c == 'D';
//
//            if (hit) {
//                bitBoard->set(i, j);
//                cout << "set at " << i << j;
//            }
//        }
//    }
    return bitBoard;
}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y) {
    if (player < 1) throw ServerException("Bad player number- low");
    if (player > 2) throw ServerException("Bad player number- high");

    if (x >= board_size | x < 0) {
        return OUT_OF_BOUNDS;
    }
    if (y >= board_size | y < 0) {
        return OUT_OF_BOUNDS;
    }

    bool strike;
    switch(player) {
        case 1:
            strike = p2_setup_board->get(x, y);
            break;
        case 2:
            strike = p1_setup_board->get(x, y);
            break;
    }

    if (strike) {
        return HIT;
    }
    else {
        return MISS;
    }
}


int Server::process_shot(unsigned int player) {
    if (player < 1) throw ServerException("Bad player number- low");
    if (player > 2) throw ServerException("Bad player number- high");

    ostringstream os;
    os << "player_" << player << ".shot.json";
    string fileName= os.str();
    std::ifstream shot(fileName);

    if (!shot) {
        return NO_SHOT_FILE;
    }

    cereal::JSONInputArchive ar(shot);
    unsigned int x, y;
    ar(x, y);
    int result= evaluate_shot(player, x, y);

    ostringstream out;
    out << "player_" << player << ".result.json";
    string resultFile= out.str();
    std::ofstream outfile(resultFile);
    cereal::JSONOutputArchive archive(outfile);
    archive(CEREAL_NVP(result));

    remove(const_cast<char*>(("player_" + to_string(player) + ".shot.json").c_str()));

    return SHOT_FILE_PROCESSED;
}