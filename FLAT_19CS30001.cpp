//Abhilash Datta 19CS30001
#include <iostream>
#include <fstream>
#include <cmath>
//#include <chrono>
using namespace std;

//NFA data structure
struct N
{
    int n, m;
    uint32_t S = 0, Fn = 0;
    uint32_t **Del;
};

//ReadNFA function
void readNFA(string filename, N *nfa)
{
    fstream file;
    file.open(filename);

    file >> nfa->n;
    file >> nfa->m;

    nfa->Del = new uint32_t *[nfa->n];

    for (int i = 0; i < nfa->n; i++)
    {
        nfa->Del[i] = new uint32_t[nfa->m];
    }

    for (int i = 0; i < nfa->n; i++)
    {
        for (int j = 0; j < nfa->m; j++)
        {
            nfa->Del[i][j] = 0;
        }
    }

    int s;
    file >> s;
    while (s != -1)
    {
        nfa->S |= 1U << s;
        file >> s;
    }

    int f;
    file >> f;
    while (f != -1)
    {
        nfa->Fn |= 1U << f;
        file >> f;
    }

    int p, a, q;
    file >> p;
    file >> a;
    file >> q;
    while (p != -1)
    {
        nfa->Del[p][a] |= 1U << q;
        file >> p;
        if (p == -1)
            break;
        file >> a;
        file >> q;
    }
    return;
}

//printNFA function
void printNFA(N nfa)
{
    //cout<<"+++ Input NFA\n";
    cout << "    Number of states: " << nfa.n << endl;
    cout << "    Input alphabet: {";
    for (int i = 0; i < nfa.m; i++)
    {
        if (i != nfa.m - 1)
            cout << i << ", ";
        else
            cout << i << "}\n";
    }
    cout << "    Start states: {";
    int flag = 0;
    for (int i = 0; i < 32; i++)
    {
        if (nfa.S & 1U << i)
        {
            if (flag == 0)
            {
                cout << i;
                flag = 1;
            }
            else
                cout << ", " << i;
        }
    }
    cout << "}\n";

    flag = 0;
    cout << "    Final states: {";
    for (int i = 0; i < 32; i++)
    {
        if (nfa.Fn & 1U << i)
        {
            if (flag == 0)
            {
                cout << i;
                flag = 1;
            }
            else
                cout << ", " << i;
        }
    }
    cout << "}\n";

    cout << "    Transition Function\n";
    for (int i = 0; i < nfa.n; i++)
    {
        for (int j = 0; j < nfa.m; j++)
        {
            flag = 0;
            cout << "        Delta(" << i << ", " << j << ") = {";
            for (int k = 0; k < 32; k++)
            {
                if (nfa.Del[i][j] & 1U << k)
                {
                    if (flag == 0)
                    {
                        cout << k;
                        flag = 1;
                    }
                    else
                        cout << ", " << k;
                }
            }
            cout << "}\n";
        }
    }
    cout << endl;
    return;
}

//states data structure
struct states
{
    uint32_t *A;
    long long size;
};

//NFA data structure
struct D
{
    uint32_t n, m;
    int s = 0;
    states F;
    uint32_t **del;
};

//printDFA function
void printDFA(D dfa)
{
    cout << "    Number of states: " << dfa.n << endl;
    cout << "    Input alphabet: {";
    for (int i = 0; i < dfa.m; i++)
    {
        if (i != dfa.m - 1)
            cout << i << ", ";
        else
            cout << i << "}\n";
    }

    cout << "    Start state: " << dfa.s << endl;

    if (dfa.n > 64)
    {
        cout << "    " << dfa.F.size << " final states\n";
        cout << "    Transition function: Skipped\n\n";
        return;
    }
    else
    {
        int flag = 0;
        cout << "    Final states: {";

        for (int i = 0; i < dfa.n; i++)
        {
            if (dfa.F.A[i / 32] & 1U << (i % 32))
            {
                if (flag == 0)
                {
                    cout << i;
                    flag = 1;
                }
                else
                    cout << ", " << i;
            }
        }
        cout << "}\n";

        cout << "    Transition Function\n";
        cout << "    a\\p|";
        for (int i = 0; i < dfa.n; i++)
        {
            if (i < 10)
                cout << "  " << i;
            else
                cout << " " << i;
        }
        cout << endl;
        cout << "    ---+";
        for (int i = 0; i < dfa.n; i++)
        {
            cout << "---";
        }
        cout << endl;
        for (int i = 0; i < dfa.m; i++)
        {
            cout << "     " << i << " |";
            for (int j = 0; j < dfa.n; j++)
            {
                if (dfa.del[j][i] < 10)
                    cout << "  " << dfa.del[j][i];
                else
                    cout << " " << dfa.del[j][i];
            }
            cout << endl;
        }
    }
    cout << endl;
    return;
}

//subset construction function
D subsetcons(N nfa)
{
    D dfa;
    dfa.n = 1U << nfa.n;
    dfa.m = nfa.m;
    dfa.s = nfa.S;

    int n = ceil((double)dfa.n / 32);

    dfa.F.A = new uint32_t[n];

    for (int i = 0; i < n; i++)
    {
        dfa.F.A[i] = 0;
    }

    dfa.F.size = 0;

    dfa.del = new uint32_t *[dfa.n];

    for (int i = 0; i < dfa.n; i++)
    {
        dfa.del[i] = new uint32_t[dfa.m];
        for (int j = 0; j < dfa.m; j++)
        {
            dfa.del[i][j] = 0;
            for (int k = 0; k < nfa.n; k++)
            {
                if (i & (1U << k))
                {
                    dfa.del[i][j] |= nfa.Del[k][j];
                }
            }
        }
        if (i & nfa.Fn)
        {
            dfa.F.A[i / 32] |= 1U << (i % 32);
            dfa.F.size++;
        }
    }
    return dfa;
}

//DFS algorithm to find reachable states
void dfs(int s, uint32_t m, uint32_t **del, states *visited)
{
    visited->A[s / 32] |= 1U << (s % 32);
    visited->size++;

    for (int j = 0; j < m; j++)
    {
        if (((visited->A[del[s][j] / 32]) & (1U << (del[s][j] % 32))) == 0)
        {
            //cout<<visited->size<<endl;
            dfs(del[s][j], m, del, visited);
        }
    }
    return;
}

//findreachable function
states findreachable(D dfa)
{
    states visited;
    int n = ceil((double)dfa.n / 32);
    visited.A = new uint32_t[n];
    visited.size = 0;
    for (int i = 0; i < n; i++)
    {
        visited.A[i] = 0;
    }
    dfs(dfa.s, dfa.m, dfa.del, &visited);
    //cout<<visited.size<<endl;
    return visited;
}

//printing reachable states
void printReachable(states R)
{
    int arr[R.size];
    int count = 0;
    int i = 0;
    while (count < R.size)
    {
        if (R.A[i / 32] & 1U << (i % 32))
        {
            arr[count] = i;
            if (count == 0)
                cout << "{" << i;
            else
                cout << ", " << i;
            count++;
        }
        i++;
    }
    cout << "}\n"
         << endl;
    return;
}

//finding x in arr
uint32_t find(int *arr, int x, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == x)
            return i;
    }
    return 0;
}

//generating DFA without unreachable states
D rmunreachable(D dfa, states R)
{
    D d;
    d.n = R.size;
    d.m = dfa.m;

    int n = ceil((double)d.n / 32);

    d.F.A = new uint32_t[n];

    for (int i = 0; i < n; i++)
    {
        d.F.A[i] = 0;
    }

    d.F.size = 0;
    d.del = new uint32_t *[d.n];

    for (int i = 0; i < d.n; i++)
    {
        d.del[i] = new uint32_t[d.m];
        for (int j = 0; j < d.m; j++)
        {
            d.del[i][j] = 0;
        }
    }

    int arr[R.size];
    int count = 0;
    int i = 0;
    while (count < R.size)
    {
        if (R.A[i / 32] & 1U << (i % 32))
        {
            arr[count] = i;
            count++;
        }
        i++;
    }

    d.s = find(arr, dfa.s, d.n);

    for (int i = 0; i < d.n; i++)
    {
        for (int j = 0; j < d.m; j++)
        {
            d.del[i][j] = find(arr, dfa.del[arr[i]][j], d.n);
        }
    }

    for (int i = 0; i < d.n; i++)
    {
        if (dfa.F.A[arr[i] / 32] & (1U << (arr[i] % 32)))
        {
            d.F.A[i / 32] |= (1U << (i % 32));
            d.F.size++;
        }
    }

    return d;
}

//checking whether final state or not
bool check_final(int i, states F)
{
    return (F.A[i / 32] & (1U << (i % 32)));
}

//generating M matrix
bool **findequiv(D dfa)
{
    bool **M;
    M = new bool *[dfa.n];
    for (int i = 0; i < dfa.n; i++)
    {
        M[i] = new bool[dfa.n];
        //true for marked
        for (int j = 0; j < dfa.n; j++)
        {
            M[i][j] = false;
        }
    }
    int count = 0;
    //Initialization
    for (int i = 0; i < dfa.n; i++)
    {
        for (int j = 0; j < dfa.n; j++)
        {
            if ((check_final(i, dfa.F) && !check_final(j, dfa.F)) || (!check_final(i, dfa.F) && check_final(j, dfa.F)))
            {
                M[i][j] = true;
                count++;
            }
        }
    }
    //Loop
    int countprev = count;
    int countnext = count;
    while (1)
    {
        countprev = count;
        for (int p = 0; p < dfa.n; p++)
        {
            for (int q = 0; q < dfa.n; q++)
            {
                if (M[p][q])
                    continue;
                for (int a = 0; a < dfa.m; a++)
                {
                    if (M[dfa.del[p][a]][dfa.del[q][a]])
                    {
                        M[p][q] = true;
                        count++;
                    }
                }
            }
        }
        countnext = count;
        if (countnext == countprev)
            break;
    }
    return M;
}

//collapse function
D collapse(D dfa, bool **M)
{
    D d;

    int group[dfa.n];
    for (int i = 0; i < dfa.n; i++)
    {
        group[i] = i;
    }
    int count = 0;
    for (int i = 0; i < dfa.n; i++)
    {
        if (group[i] != i)
            continue;

        for (int j = 0; j < dfa.n; j++)
        {
            if (M[i][j] == 0 && i != j)
            {
                group[j] = group[i];
            }
        }
        count++;
    }

    cout << "+++ Equivalent states\n";
    int k = 0;
    for (int i = 0; i < count; i++)
    {
        if (i < 10)
            cout << "    Group  " << i << ": {";
        else
            cout << "    Group " << i << ": {";
        int flag = 0;
        int f = 0;
        while (f == 0)
        {
            for (int j = 0; j < dfa.n; j++)
            {
                if (group[j] == k)
                {
                    f = 1;
                    group[j] = i;
                    if (flag == 0)
                    {
                        cout << j;
                        flag = 1;
                    }
                    else
                        cout << ", " << j;
                }
            }
            k++;
        }
        cout << "}\n";
    }
    cout << endl;
    d.n = count;
    d.s = group[dfa.s];
    d.m = dfa.m;

    int n = ceil((double)d.n / 32);

    d.F.A = new uint32_t[n];

    for (int i = 0; i < n; i++)
    {
        d.F.A[i] = 0;
    }

    d.F.size = 0;
    d.del = new uint32_t *[d.n];

    for (int i = 0; i < d.n; i++)
    {
        d.del[i] = new uint32_t[d.m];
        for (int j = 0; j < d.m; j++)
        {
            d.del[i][j] = 0;
        }
    }

    for (int i = 0; i < dfa.n; i++)
    {
        for (int j = 0; j < d.m; j++)
        {
            d.del[group[i]][j] = group[dfa.del[i][j]];
        }
    }

    for (int i = 0; i < dfa.n; i++)
    {
        if (dfa.F.A[i / 32] & (1U << (i % 32)))
        {
            d.F.A[group[i] / 32] |= (1U << (group[i] % 32));
        }
    }

    return d;
}

//Main function
int main(int argc, char* argv[])
{
    //auto start = chrono::high_resolution_clock::now(); 
    string filename = argv[1];

    N nfa;
    readNFA(filename, &nfa);
    cout << "+++ Input NFA\n";
    printNFA(nfa);

    /* auto stop = chrono::high_resolution_clock::now(); 
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start); 
    cout << "Time taken: " << duration.count() << " milliseconds" << endl; */

    D dfa = subsetcons(nfa);
    cout << "+++ Converted DFA\n";
    printDFA(dfa);

    /* stop = chrono::high_resolution_clock::now(); 
	duration = chrono::duration_cast<chrono::milliseconds>(stop - start); 
    cout << "Time taken: " << duration.count() << " milliseconds" << endl; */

    states R = findreachable(dfa);
    cout << "+++ Reachable states: ";
    printReachable(R);

    /*stop = chrono::high_resolution_clock::now(); 
	duration = chrono::duration_cast<chrono::milliseconds>(stop - start); 
    cout << "Time taken: " << duration.count() << " milliseconds" << endl; */

    D d_dash = rmunreachable(dfa, R);
    cout << "+++ Reduced DFA after removing unreachable states\n";
    printDFA(d_dash);

    /*stop = chrono::high_resolution_clock::now(); 
	duration = chrono::duration_cast<chrono::milliseconds>(stop - start); 
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;*/ 

    bool **M = findequiv(d_dash);
    D dd_dash = collapse(d_dash, M);
    cout << "+++ Reduced DFA after collapsing equivalent states\n";
    printDFA(dd_dash);

    /* stop = chrono::high_resolution_clock::now(); 
	duration = chrono::duration_cast<chrono::milliseconds>(stop - start); 
    cout << "Time taken: " << duration.count() << " milliseconds" << endl; */
    
    return 0;
}