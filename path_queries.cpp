//http://codeforces.com/contest/1213/problem/G
#include <bits/stdc++.h>

using namespace std;
using ut = uint32_t;
using ull = unsigned long long;

using vut = vector<ut>;
using pu = pair<ut, ut>;
using vpu = vector<pu>;

struct node{
  ut set_size = 1;

  node *parent = nullptr;
  node &dsu_find(){
    if(parent == nullptr){
      return *this;
    }
    parent = &(parent->dsu_find());
    return *parent;
  }

  void dsu_union(node &other){
    auto &my_set = dsu_find();
    auto &other_set = other.dsu_find();
    if(&my_set != &other_set){
      my_set.parent = &other_set;
      other_set.set_size += my_set.set_size;
    }
  }
};

struct edge{
  ut node_a, node_b;
  ut weight;
};


inline ull nchoose2(ull n){return (n*(n-1))/2;}

template<class U, class V>
bool order_by_first_nondec(const std::pair<U, V> &a, const std::pair<U, V> &b){
  return a.first < b.first;
}

int main(){
  ut n, m;
  cin >> n >> m;

  vector<edge> edges;
  vector<node> nodes_table(n);

  for (auto i = 0ul; i < n - 1; i++) {
    ut u, v, w;
    cin >> u >> v >> w;
    u--;
    v--;
    ut a, b;
    if(u < v){
      a = u;
      b = v;
    }
    else{
      a = v;
      b = u;
    }
    edges.push_back({a, b, w});
  }

  sort(edges.begin(), edges.end(), [](const edge& e1, const edge &e2){return e1.weight < e2.weight;});


  vpu queries;
  for(auto i = 0ul; i < m; i++){
    ut q;
    cin >> q;
    queries.emplace_back(q, i);
  }


  sort(queries.begin(), queries.end(), order_by_first_nondec<ut,ut>);

  vector<pair<ut, ull>> results;

  ut q_id = 0;
  auto next_w = queries[q_id].first;

  ull r = 0;

  for(auto &e : edges){
    while(e.weight > next_w && q_id < queries.size()){
      results.emplace_back(queries[q_id].second, r);
      q_id++;
      next_w = queries[q_id].first;
    }
    if(q_id >= queries.size()){
      break;
    }
    auto &n_a = nodes_table[e.node_a];
    auto &n_b = nodes_table[e.node_b];
    auto &n_a_cc = n_a.dsu_find();
    auto &n_b_cc = n_b.dsu_find();
    if(&n_a_cc != &n_b_cc){
      r = r - nchoose2(n_a_cc.set_size) - nchoose2(n_b_cc.set_size) + nchoose2(n_a_cc.set_size + n_b_cc.set_size);
      n_a_cc.dsu_union(n_b_cc);
    }
  }

  for(; q_id < queries.size(); q_id++){
    results.emplace_back(queries[q_id].second, r);
  }

  sort(results.begin(), results.end(), order_by_first_nondec<ut,ut>);
  ut counter = 0;
  for(auto &rs : results){
    cout << rs.second;
    if(counter < results.size()-1){
      cout << " ";
    }
    counter++;
  }
  cout << endl;

  return 0;
}
