#include "phone-book.h"

bool is_prefix(const std::string &a, const std::string &b)
{
    if(a.size() > b.size())
        return false;
    for(size_t i = 0; i < a.size(); i++)
        if(a[i] != b[i])
            return false;

    return true;
}

bool phone_book_t::create_user(const std::string &number, const std::string &name)
{
    auto it = this->users.find(number); 
    if(it == this->users.end())
    {
        this->users.insert(std::make_pair(number, std::make_pair(name, 0)));
        return true;
    }
    return false;
}

bool phone_book_t::add_call(const call_t &call)
{
    auto it = this->users.find(call.number);
    if(it == this->users.end())
        return false;
    this->calls.push_back(call);
    it->second.second += call.duration_s;
    return true;
}

std::vector<call_t> phone_book_t::get_calls(size_t start_pos, size_t count) const
{
    std::vector<call_t>res;
    for(size_t i = start_pos; i < start_pos + count && i < this->calls.size(); i++)
    {
        res.push_back(this->calls[i]);
    }
    return res;
}

bool comp2 ( user_info_t a,  user_info_t b) 
{
    if(a.user.name != b.user.name)
        return a.user.name < b.user.name;
    if(a.total_call_duration_s != b.total_call_duration_s)
        return a.total_call_duration_s > b.total_call_duration_s;
    
    return a.user.number < b.user.number;
}



bool comp ( user_info_t a,  user_info_t b) 
{
    if(a.total_call_duration_s != b.total_call_duration_s)
        return a.total_call_duration_s > b.total_call_duration_s;
    if(a.user.name != b.user.name)
        return a.user.name < b.user.name;
    return a.user.number < b.user.number;
}

std::vector<user_info_t> phone_book_t::search_users_by_number(const std::string &number_prefix, size_t count) const
{
    std::vector<user_info_t>res;
    user_info_t tmp;
    std::vector<user_info_t>tmp_res;

    auto it = users.lower_bound(number_prefix);

    while(it != this->users.end())
    {

        if(is_prefix(number_prefix, it->first))
        {
            tmp.user.number = it->first;
            tmp.user.name = it->second.first;
            tmp.total_call_duration_s = it->second.second;
            tmp_res.push_back(tmp);
        }else{
            break;
        }
        it++;
    }
    sort(tmp_res.begin(), tmp_res.end(), comp);
    for(int i = 0; i < count && i < tmp_res.size(); i++)
    {
        res.push_back(tmp_res[i]);
    }   
    return res;
}

std::vector<user_info_t> phone_book_t::search_users_by_name(const std::string &name_prefix, size_t count) const
{
    std::vector<user_info_t>res;

    user_info_t tmp;
    std::vector<user_info_t>tmp_res;

    auto it = users.begin();

    while(it != this->users.end())
    {

        if(is_prefix(name_prefix, it->second.first))
        {
            tmp.user.number = it->first;
            tmp.user.name = it->second.first;
            tmp.total_call_duration_s = it->second.second;
            tmp_res.push_back(tmp);
        }
        it++;
    }
    sort(tmp_res.begin(), tmp_res.end(), comp2);
    for(int i = 0; i < count && i < tmp_res.size(); i++)
    {
        res.push_back(tmp_res[i]);
    }


    return res;
}

void phone_book_t::clear()
{
    this->users.clear();
    this->calls.clear();
}

size_t phone_book_t::size() const
{
    return this->users.size();
}

bool phone_book_t::empty() const
{
    return this->users.empty();
}