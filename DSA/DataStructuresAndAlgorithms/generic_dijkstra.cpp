#include <iostream>
#include <concepts>
#include <algorithm>
#include <tuple>
#include <utility>
#include <climits>
#include <queue>
#include <chrono>
using namespace std;
using namespace std::chrono;

auto dijkstra(forward_iterator auto V_begin, forward_iterator auto V_end,
                auto w,
                decltype(V_begin) S,
                auto adj)
{
    using v_type = decltype(V_begin);
    using w_type = decltype(w);

    vector<float> d(distance(V_begin, V_end), numeric_limits<float>::infinity());
    d[distance(V_begin, S)] = 0;
    
    auto d_comparator = [V_begin, &d](v_type v1, v_type v2) 
    { 
        return d[distance(V_begin, v1)] < d[distance(V_begin, v2)];
    };

    using d_comparator_t = decltype(d_comparator);

    // Priority queue of Vs
    priority_queue<v_type, vector<v_type>, d_comparator_t> Q(d_comparator);

    Q.push(S); // The first V that we will process
    
    while(Q.empty() == false)
    {
        v_type v = Q.top();
        Q.pop();
        
        for(auto & u : adj(v))
        {
            pair<v_type, v_type> e { v, u };
            int v_index = distance(V_begin, v);
            int u_index = distance(V_begin, u);

            if(float trial_w = d[v_index] + w(e); d[u_index] > trial_w)
            {
                d[u_index] = trial_w;
                Q.push(u);
            }
        }
    }

    return d;
}

std::ostream & operator<<(std::ostream & out, const pair<int, int> & p)
{
    out << "{ " << p.first << ", " << p.second << " }";
    return out;
}

auto main() -> int
{
    using Vs_type = array<int, 4>;
    using V_type = Vs_type::iterator;
    
    using Es_type = array<pair<V_type, V_type>, 5>;
    using E_type = Es_type::iterator;
    
    Vs_type V = { 1, 2, 3, 4 };

    Es_type E = 
    { 
        pair<V_type, V_type>{ begin(V), begin(V) + 1 }, 
                            { begin(V), begin(V) + 2 }, 
                            { begin(V) + 1, begin(V) + 2 }, 
                            { begin(V) + 1, begin(V) + 3 }, 
                            { begin(V) + 3, begin(V) }
    };

    vector<pair<V_type, vector<V_type>>> Adjs
    {
        { begin(V), { begin(V) + 1, begin(V) + 2 } },
        { begin(V) + 1, { begin(V) + 2, begin(V) + 3 } },
        { begin(V) + 3, { begin(V) } }
    };

    /*      3
        ---->-----
        |        |
       (1) ->(2)-(3)
        |    |
        ^    v
        |---(4)
    */

    array<float, 5> weights = { 1.f, 3.f, 1.5f, 4.f, 1.f };

    auto w = [Es_begin = begin(E), Es_end = end(E), weights](const pair<V_type, V_type> & e) -> float
    {
        auto e_iter = find(Es_begin, Es_end, e);
        if(e_iter != Es_end)
        {
            int index = distance(Es_begin, e_iter);
            return weights[index];
        }
        else
        {
            return numeric_limits<float>::infinity();
        }
    };

    auto adj = [&Adjs](V_type v)
    {
        vector<V_type> v_adj;
        for(auto & a : Adjs)
        {
            if(a.first == v)
            {
                v_adj = a.second;
                return v_adj;
            }
        }
        return v_adj;
    };

    auto t1 = chrono::high_resolution_clock::now();
    
    auto d = dijkstra(begin(V), end(V), w, begin(V), adj);

    auto t2 = chrono::high_resolution_clock::now();

    cout << "dijkstra took " << duration_cast<duration<double, micro>>(t2 - t1).count() << " microseconds \n";

    for(auto & v : d)
    {
        cout << v << endl;
    }

    return 0;
}
