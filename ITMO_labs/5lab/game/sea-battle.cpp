#include "game/sea-battle.h"


sea_battle_t::sea_battle_t(std::shared_ptr<player_interface_t> player1, const field_t &field1, std::shared_ptr<player_interface_t> player2, const field_t &field2)
{
  for(size_t i = 0; i < field_t::FIELD_SIZE; i++)
  {
    this->field1s.push_back(field1[i]);  
  }
  for(size_t i = 0; i < field_t::FIELD_SIZE; i++)
  {
    this->field2s.push_back(field2[i]);  
  }
  this->player1 = player1;
  this->player2 = player2;
  
}

sea_battle_t::turn_t sea_battle_t::change_turn(turn_t current_turn) {
  return current_turn == FIRST_PLAYER ? SECOND_PLAYER : FIRST_PLAYER;
}

std::string sea_battle_t::get_player_name(turn_t turn) {
  return turn == FIRST_PLAYER ? "First" : "Second";
}

void sea_battle_t::play()
{
  
  turn_t player = FIRST_PLAYER;
  std::pair<int, int>coords;
  int player1_ships = 10;
  int player2_ships = 10;
  
  field_t field1(this->field1s);
  field_t field2(this->field2s);
  field_t field_tmp(this->field1s);

  while(1)
  {
    if(player == FIRST_PLAYER)
    {
      player2_ships = 0;
      for(size_t i = 0; i < field_t::FIELD_SIZE; i++)
        for(size_t j = 0; j < field_t::FIELD_SIZE; j++)
        {
          if(field2[i][j] == field_t::SHIP_CELL)
          {
            player2_ships++;
            field_tmp[i][j] = field_t::EMPTY_CELL;
          }
          else{
            field_tmp[i][j] = field2[i][j];
          }
            
        }
      if(player2_ships == 0)
      {
        this->player1->on_win();
        this->player2->on_lose();
        return;
      }    
      coords = this->player1->make_move(field1, field_tmp);
      
      
      
      
      if(coords.first < 0 || coords.first >= field_t::FIELD_SIZE || coords.second < 0 || coords.second >= field_t::FIELD_SIZE)
      {
        this->player1->on_incorrect_move(coords.first, coords.second);
        continue;
      }

      if(field2[coords.first][coords.second] == field_t::HIT_CELL || field2[coords.first][coords.second] == field_t::MISS_CELL)
      {
        this->player1->on_duplicate_move(coords.first, coords.second);
        continue;
      }

      if(field2[coords.first][coords.second] == field_t::EMPTY_CELL)
      {
        this->player1->on_miss(coords.first, coords.second);
        field2[coords.first][coords.second] = field_t::MISS_CELL;
        player = change_turn(player);
        continue;
      }
      if(field2[coords.first][coords.second] == field_t::SHIP_CELL)
      {
        
        field2[coords.first][coords.second] = field_t::HIT_CELL;
        int flag = coords.first;
        int neighbour = 0;
        flag = coords.first;
        flag--;
        while(flag >= 0 && (field2[flag][coords.second] == field_t::SHIP_CELL || field2[flag][coords.second] == field_t::HIT_CELL))
        {
          if(field2[flag][coords.second] == field_t::SHIP_CELL)
            neighbour++;
          flag--;
          
        }
        flag = coords.first;
        flag++;
        while(flag < field_t::FIELD_SIZE && (field2[flag][coords.second] == field_t::SHIP_CELL || field2[flag][coords.second] == field_t::HIT_CELL))
        {
          if(field2[flag][coords.second] == field_t::SHIP_CELL)
            neighbour++;
          flag++;
          
        }
        flag = coords.second;
        flag--;
        while(flag >= 0 && (field2[coords.first][flag] == field_t::SHIP_CELL || field2[coords.first][flag] == field_t::HIT_CELL))
        {
          
          if(field2[coords.first][flag] == field_t::SHIP_CELL)
            neighbour++;
          flag--;
        }
        flag = coords.second;
        flag++;
        while(flag < field_t::FIELD_SIZE && (field2[coords.first][flag] == field_t::SHIP_CELL || field2[coords.first][flag] == field_t::HIT_CELL))
        {
          
          if(field2[coords.first][flag] == field_t::SHIP_CELL)
            neighbour++;
          flag++;
        }
      
        if(neighbour == 0)
        {
          this->player1->on_kill(coords.first, coords.second);
        }else{
          this->player1->on_hit(coords.first, coords.second); 
        }
        
          
      }

      

    }else{
      
      player1_ships = 0;
      for(size_t i = 0; i < field_t::FIELD_SIZE; i++)
        for(size_t j = 0; j < field_t::FIELD_SIZE; j++)
          if(field1[i][j] == field_t::SHIP_CELL)
          {
            player1_ships++;
            field_tmp[i][j] = field_t::EMPTY_CELL;
          }
          else
          {
            field_tmp[i][j] = field1[i][j];
          }
      if(player1_ships == 0)
      {
        this->player2->on_win();
        this->player1->on_lose();
        return;
      }      
      coords = this->player2->make_move(field2, field_tmp);

      

      if(coords.first < 0 || coords.first >= field_t::FIELD_SIZE || coords.second < 0 || coords.second >= field_t::FIELD_SIZE)
      {
        this->player2->on_incorrect_move(coords.first, coords.second);
        continue;
      }

      if(field1[coords.first][coords.second] == field_t::HIT_CELL || field1[coords.first][coords.second] == field_t::MISS_CELL)
      {
        this->player2->on_duplicate_move(coords.first, coords.second);
        continue;
      }

      if(field1[coords.first][coords.second] == field_t::EMPTY_CELL)
      {
        field1[coords.first][coords.second] = field_t::MISS_CELL;
        player = change_turn(player);
        this->player2->on_miss(coords.first, coords.second);
        continue;
      }
      if(field1[coords.first][coords.second] == field_t::SHIP_CELL)
      {
        
        field1[coords.first][coords.second] = field_t::HIT_CELL;

        int flag = coords.first;
        int neighbour = 0;
        flag--;
        while(flag >= 0 && (field1[flag][coords.second] == field_t::SHIP_CELL || field1[flag][coords.second] == field_t::HIT_CELL))
        {
          
          if(field1[flag][coords.second] == field_t::SHIP_CELL)
            neighbour++;
          flag--;
        }
        flag = coords.first;
        flag++;
        while(flag < field_t::FIELD_SIZE && (field1[flag][coords.second] == field_t::SHIP_CELL || field1[flag][coords.second] == field_t::HIT_CELL))
        {
          
          if(field1[flag][coords.second] == field_t::SHIP_CELL)
            neighbour++;
          flag++;
        }
        flag = coords.second;
        flag--;
        while(flag >= 0 && (field1[coords.first][flag] == field_t::SHIP_CELL || field1[coords.first][flag] == field_t::HIT_CELL))
        {
          
          if(field1[coords.first][flag] == field_t::SHIP_CELL)
            neighbour++;
          flag--;
        }
        flag = coords.second;
        flag++;
        while(flag < field_t::FIELD_SIZE && (field1[coords.first][flag] == field_t::SHIP_CELL || field1[coords.first][flag] == field_t::HIT_CELL))
        {
          
          if(field1[coords.first][flag] == field_t::SHIP_CELL)
            neighbour++;
          flag++;
        }
        if(neighbour == 0)
        {
          this->player2->on_kill(coords.first, coords.second);
        }else{
          this->player2->on_hit(coords.first, coords.second); 
        }
          continue;
      }

      
    }
  }
}

