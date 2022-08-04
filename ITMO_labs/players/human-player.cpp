#include "players/human-player.h"

std::pair<int, int> human_player_t::make_move(const field_t &my_field, const field_t &enemy_field){
    
    this->output_stream << "Your field:" << std::endl << my_field << std::endl << "Enemy's field" << std::endl;
    this->output_stream << enemy_field << std::endl << "Input you turn! [Example 1A or 10B or 6J]" << std::endl;
    while(1)
    {
        std::string s, first_coord, second_coord;
        first_coord.clear();
        second_coord.clear();
        std::pair<int, int> res; 
        int x, y;


        
        std::getline(this->input_stream, s);
        bool check_first_coord = true;
        int err = 0;
        int q = 0;
        for(size_t i = 0; i < s.size(); i++)
        {
            q++;
            char symb = s[i];
            if(check_first_coord)
            {
                if(symb <= '9' && symb >= '0')
                {
                    first_coord.push_back(symb);
                }else if(symb <= 'Z' && symb >= 'A')
                {
                    check_first_coord = false;
                    second_coord.push_back(symb);
                }else
                {
                    err++;
                    break;
                }
            }else
            {
                if(symb <= 'Z' && symb >= 'A')
                {
                    second_coord.push_back(symb);
                }else
                {
                    err++;
                    break;
                }
            }
        }
        if(first_coord.size() > 2 || second_coord.size() > 1)
            err++;
        if(err == 0)
        {
            int flag = 1;
            x = 0;
            for(int i = first_coord.size() - 1; i > -1; i--)
            {
                x += flag*int(first_coord[i] - '0');
                flag*=10;
            }
            x--;
            flag = 1;
            y = 0;
            for(int i = second_coord.size() - 1; i > -1; i--)
            {
                y += flag*int(second_coord[i] - 'A');
                flag*=26;
            }

            if(!field_t::is_cell_valid(x, y))
            {
                err++;
            }
        }
        if(err > 0)
        {
            this->output_stream << "Incorrect move! [Correct example 1A or 10B or 6J]" << std::endl;
        }else{
            res.first = x;
            res.second = y;
            return res;
        }
    }
    //return res;    
     
}

void human_player_t::on_duplicate_move(int x, int y)
{
    char second_coord = 'A' + y;
    this->output_stream << x+1 << second_coord << " DUPLICATE! Repeat move!" << std::endl;
}

void human_player_t::on_miss(int x, int y)
{
    char second_coord = 'A' + y;
    this->output_stream << x+1 << second_coord << " MISS! Enemy's turn!" << std::endl;
}

void human_player_t::on_hit(int x, int y)
{
    char second_coord = 'A' + y;
    this->output_stream << x+1 << second_coord << " HIT! Your next move!" << std::endl;
    
}

void human_player_t::on_kill(int x, int y)
{
    char second_coord = 'A' + y;
    this->output_stream << x+1 << second_coord << " KILL! Your next move!" << std::endl;
    
}

void human_player_t::on_win()
{
    this->output_stream << "You WIN!" << std::endl;
    
}

void human_player_t::on_lose()
{
    this->output_stream << "You LOSE!" << std::endl;
}

