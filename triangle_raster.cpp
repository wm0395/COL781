#include <vector>
#include <utility>
#include <iostream>

using namespace std;

// UTILITY FUNCTIONS
// method to overload for pair<T,U>
template <typename T,typename U>                                                   
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first+r.first,l.second+r.second};                                    
}
template <typename T,typename U>
std::pair<T,U> operator-(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first-r.first,l.second-r.second};                                    
}

int dot(pair<int,int> &a, pair<int,int> &b){
    return a.first*b.first + a.second*b.second;
}
template<typename T>
void swap(T &a, T &b){
    T temp = a;
    a = b;
    b = temp;
}
template<typename T>
void cycle_cw(T &a, T &b, T &c){
    T temp = a;
    a = b;
    b = c;
    c = temp;
}
template<typename T>
void cycle_aw(T &a, T &b, T &c){
    T temp = c;
    c = b;
    b = a;
    a = temp;
}


// MAIN FUNCTIONS
const int N = 10;
bool isLeft(pair<int,int> &a, pair<int,int> &b, pair<int,int> &p){
    pair<int,int> n = make_pair(a.second-b.second, b.first - a.first), d = p - a;
    return (dot(n, d) >= 0);
}

void make_anti_clockwise(vector<pair<int,int>> &points){
    if(!isLeft(points[0], points[1], points[2])){
        swap(points[0], points[1]);
    }
}

void make_ordered(vector<pair<int,int>> &points){
    pair<int,int> a = points[0];
    pair<int,int> b = points[1];
    pair<int,int> c = points[2];

    if(c.second>= a.second && c.second >= b.second){
        return;
    }
    else if(b.second >= a.second){
        cycle_aw(points[0], points[1], points[2]);
    }
    else{
        cycle_cw(points[0], points[1], points[2]);
    }
}

bool isInside(pair<int,int> &a, pair<int,int> &b, pair<int,int> &c, pair<int,int> &p){
    // cout<<"(" << isLeft(a, b, p) << isLeft(b, c, p) << isLeft(c, a, p)<<")";
    return isLeft(a, b, p) && isLeft(b, c, p) && isLeft(c, a, p);
}

void print_inline(vector<pair<int,int>> &points){
    for(int j = 0; j<N; j++){
        for(int i = 0; i<N; i++){
            pair<int,int> p = make_pair(i,j);
            if(isInside(points[0], points[1], points[2], p)){
                cout << "[]";
            }
            else{
                cout << "..";
            }
        }
        cout << endl;
    }
}

int main(){
    vector<pair<int,int>> points(3, pair<int,int>(0,0));
    for(int i = 0; i < 3; i++){
        cin >> points[i].first >> points[i].second;
    }

    cout<<points[0].first<<" "<<points[0].second<<"\t"<<points[1].first<<" "<<points[1].second<<"\t"<<points[2].first<<" "<<points[2].second<<endl;
    make_anti_clockwise(points);
    
    cout<<points[0].first<<" "<<points[0].second<<"\t"<<points[1].first<<" "<<points[1].second<<"\t"<<points[2].first<<" "<<points[2].second<<endl;
    print_inline(points);
    
    return 0;
}