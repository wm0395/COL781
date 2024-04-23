#include "./../src/sw.hpp"
#include <unordered_map>


unsigned long long uni_hash(ivec3 pos){
    unsigned long long temp = (unsigned long long)(1e12*pos.x) + (unsigned long long)(1e6*pos.y) + pos.z;
    return temp;
}

ivec3 uni_hash_inv(unsigned long long N){
    return ivec3(floor((N%(unsigned long long)1e18)/1e12), floor((N%(unsigned long long)1e12)/1e6), (N%(unsigned long long)1e6));
}

vector<vector<int>> collision_detect(unordered_map<unsigned long long, vector<int>> &uniform_grid, vector<Particle*> particles, float del_x){
    vector<vector<int>> collision;
    for(auto p : particles){
        int id = p->identity;
        ivec3 cell = ivec3((int)(p->pos.x/del_x) - 5e5, (int)(p->pos.y/del_x) - 5e5, (int)(p->pos.z/del_x) - 5e5);
        unsigned long long hash = uni_hash(cell);
        bool col = false;
        vector<int> temp_col;
        for(int i = 0; i < 27; i++){
            ivec3 temp = ivec3(cell.x, cell.y, cell.z);
            temp.x += (i%3) - 1;
            temp.y += ((i/3)%3) - 1;
            temp.z += ((i/9)%3) - 1;
            unsigned long long temp_h = uni_hash(temp);
            if(uniform_grid.find(temp_h) != uniform_grid.end() && uniform_grid[temp_h].size() > 0){
                if(uniform_grid[temp_h].size() == 1 && uniform_grid[temp_h][0] == id) continue;   // if only one particle in the cell and it is the same particle
                if(!col){
                    col = true;
                    temp_col.push_back(id);
                }
                // temp_col.insert(temp_col.end(), uniform_grid[temp_h].begin(), uniform_grid[temp_h].end());
                for (int p_id : uniform_grid[temp_h]){
                    if(p_id != id)
                    temp_col.push_back(p_id);
                }
            }
        }
        if(col){
            collision.push_back(temp_col);
        }
    }
    return collision;
}

void collision_update(unordered_map<unsigned long long, vector<int>> &uniform_grid, vector<Particle*> particles, float del_x){
    uniform_grid = unordered_map<unsigned long long, vector<int>> ();
    for(auto p : particles){
        ivec3 cell = ivec3((int)(p->pos.x/del_x) - 5e5, (int)(p->pos.y/del_x) - 5e5, (int)(p->pos.z/del_x) - 5e5);
        unsigned long long hash = uni_hash(cell);
        if(uniform_grid.find(hash) == uniform_grid.end()){
            uniform_grid[hash] = vector<int>();
        }
        uniform_grid[hash].push_back(p->identity);
    }
}