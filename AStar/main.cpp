#include <iostream>
#include <array>
#include <queue>
#include <vector>
#include <utility>
#include <sstream>
using namespace std;

// logging utility
void log(auto what, auto... rest)
{
    cout << what;
    if constexpr (sizeof...(rest) > 0)
    {
        log(rest...);
    }
}

template<typename L, typename R>
std::pair<L,R> operator+(const std::pair<L,R> & a, const std::pair<L,R> & b)
{
    return { a.first + b.first, a.second + b.second };
}

template<typename SelfType>
concept MatrixConcept = requires(SelfType M, unsigned int row, unsigned int col, pair<unsigned int, unsigned int> rowcol)
{
    { M.at(row, col) } -> int &;
    { M.at(rowcol) } -> int &;
    { M.Blank() } -> pair<int, int>;
    { M.Adj() } -> vector<SelfType>;
    { M.g } -> int;
    { M.Rows } -> const unsigned int;
    { M.Cols } -> const unsigned int;
};

// Дъската е матрица m на n
// Матрицата също представлява Node в графа на всички възможни състояния на дъската
// За всяко състояние може да вземем неговите съседи с .Adj() метода на класа, който строи на момента нови матрици,
// като мести блокчета в празното място
// За всеки съсед на сегашното състояние знаем, че разстоянието от началото до него е равно на състоянието до сегашното състояние + 1 (преместването на блокчето в .Adj())
// Този факт прави имплементацията на g функцията по-долу тривиална
template<const unsigned int m, const unsigned int n>
struct Matrix
{
    int & at(unsigned int row, unsigned int col)
    {
        return M[row * n + col];
    }

    int at(unsigned int row, unsigned int col) const
    {
        return M[row * n + col];
    }
    
    int & at(pair<int, int> loc)
    {
        return at(loc.first, loc.second);
    }

    // Връща индексите на празното блокче
    pair<int, int> Blank()
    {
        for(int i = 0; i < m; ++i)
            for(int j = 0; j < n; ++j)
                if(at(i, j) == -1) return { i, j };
        
        return { -1, -1 };
    }

    // Разменя блокчета i <-> j
    bool Swap(pair<int, int> i, pair<int, int> j)
    {
        if(i.first < m && i.second < n && i.first >= 0 && i.second >= 0 &&
           j.first < m && j.second < n && j.first >= 0 && j.second >= 0)
        {
            int tmp = at(i);
            at(i) = at(j);
            at(j) = tmp;
            return true;
        }
        else return false;
       
    }


    void ReadFrom(std::istream & in)
    {
        for(int i = 0; i < m; ++i)
        {
            for(int j = 0; j < n; ++j)
            {
                in >> at(i, j);
            }
        }
    }

    // Връща масив от всички съседи на сегашното състояние, т.е. всички състояния, които могат да се получат от
    // преместване на блокче в празното пространство
    vector<Matrix<m, n>> Adj()
    {
        vector<Matrix<m, n>> adj;
        adj.reserve(4);
        pair<int, int> blank = this->Blank();
        pair<int, int> delta[] = 
        {
            {-1, 0 },
            { 0, 1 },
            { 1, 0 },
            { 0, -1 }
        };
        for(int i = 0; i < 4; ++i)
        {
            Matrix<m, n> tmp = *this;
            pair<int, int> blankAdj = blank + delta[i];
            if(tmp.Swap(blank, blankAdj))
            {
                // Броят стъпки до съседа е броят стъпки до нас + 1 (т.е. + тази стъпка duh)
                tmp.g = this->g + 1;
                adj.push_back(tmp);
            }
        }

        return adj;
    }

    const unsigned int Rows = m;
    const unsigned int Cols = n;
    int g = 0; // The distance to us from the start node of the 'graph', 0 for the start node and +1 each time we call Adj()
    bool operator==(const Matrix<m, n> & other) const
    {
        return this->M == other.M;
    }

private:
    array<int, m * n> M = { 0 };
};

// printing of a matrix
template<const unsigned int m, const unsigned int n>
std::ostream & operator<<(std::ostream & out, const Matrix<m,n> M)
{
    for(int i = 0; i < m; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            if(M.at(i, j) == -1)
                out << ' ';
            else
                out << M.at(i, j);
            out << ' ';
        }
        out << std::endl;
    }
    //out << "(g=" << M.g << ')' << endl;
    //out << "(h=" << h(M) << ')' << endl;
    return out;
}

template<typename State, int(*H)(const State &), int(*G)(const State &)>
vector<State> AStar(const State & start, const State & goal)
{
    struct Path
    {
        // The cost of the path = real cost of the path + heuristic value
        int f() const { return H(states.back()) + G(states.back()); };

        vector<State> states;

        bool operator<(const Path & other) const
        {
            return f() > other.f();
        }
    };
    
    std::priority_queue<Path> q;
    std::vector<pair<State, int>> visited;

    auto isVisited = [&visited](State & state) -> pair<State, int> *
    {
        for(auto & v: visited)
        {
            if(v.first == state)
                return &v;
        }
        return nullptr;
    };

    if(start == goal)
    {
        return {start};
    }

    Path startpath;
    startpath.states.push_back(start);
    q.push(startpath);
    // unsigned int ceiling = 1000;

    // AStar main:
    while(!q.empty() /* && ceiling-- */)
    {
        Path p = std::move(q.top()); 
        q.pop();
        if(p.states.back() == goal) { return p.states; }

        State st = p.states.back();
        //log("popped:------------------\n", st);

        for(auto & adj: st.Adj())
        {
            if(pair<State, int> * v = isVisited(adj); v != nullptr && v->second < H(adj) + G(adj))// this state is visited and the A* value is bigger
            {
                continue; // ignore if the state is visited with smaller A* cost
            }
            Path nextPath = p;
            nextPath.states.push_back(adj);
            q.push(nextPath);
            //log("----------- pushed\n", adj);
        }
        visited.push_back({st, H(st) + G(st)});
    }
    return vector<State>();
}

// Евристичнa функция: Бротя на плочките, които не са на местата си
auto h = [](const MatrixConcept & State) -> int 
{
    int result = 0;
    for(int i = 0; i < State.Rows; ++i)
        for(int j = 0; j < State.Cols; ++j)
            if(State.at(i, j) != i * State.Cols + j + 1)
                ++result;
    return result - 1; // '-1' will always be out of place
};

// Функция която връща реалното разстояние от началото до State
auto g = [](const MatrixConcept & State) -> int
{
    return State.g;
};

int main()
{
    Matrix<3,3> G, Goal;
    G.ReadFrom(std::cin);

    stringstream ss("1 2 3 4 5 6 7 8 -1");
    Goal.ReadFrom(ss);

    auto result = AStar<Matrix<3,3>, h, g>(G, Goal);
    
    if(result.size() == 0)
    {
        log("No solution");
    }
    else
    {
        log("============ THE SOLUTION IS ", result.size(), " STEPS ============ \n");
        for(auto & m: result)
        {
            cout << m << endl;
        }
    }
    
    return 0;
}   
