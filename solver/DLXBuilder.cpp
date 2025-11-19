#include "DLXBuilder.h"
#include <limits>
#include <algorithm>
#include <cassert>
#include <unordered_set>

vector<vector<int>> DLXBuilder::findSolutions()
{
    build();
    search(0);
    std::cout << "Sol size: " << solutionSize << endl;

    return allSolutions;
}

void DLXBuilder::build()
{
    int M = matrix.size();
    int N = matrix[0].size();
    lenVec.resize(N);

    // To simplify the handling, replace 1's in the matrix by
    // node indexes
    int idx = N + 2;
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (matrix[i][j] > 0)
            {
                matrix[i][j] = idx++;
                lenVec[j]++;
            }
        }
        // skip spacer indexes
        idx++;
    }
    // attach the header
    vector<int> head;
    for (int i = 1; i <= N; i++)
    {
        head.push_back(i);
    }
    matrix.insert(matrix.begin(), head);

    int maxSize = N * M + N + (N + 1); // matrix + header + spacers
    nodes.clear();
    nodes.resize(maxSize);

    // set uo the root node
    nodes[0].up = -1;
    nodes[0].down = -1;
    nodes[0].left = N;
    nodes[0].right = 1;
    nodes[0].top = 0;
    nodes[0].row = 0;
    nodes[0].len = -1;

    auto posMod = [](int a, int b)
    {
        return ((a % b) + b) % b;
    };

    // link the columns
    int nodeCount = 0;
    for (int i = 0; i < M + 1; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (matrix[i][j] > 0)
            {
                int f = posMod(i - 1, M + 1);
                while (true)
                {
                    if (matrix[f][j] > 0)
                        break;
                    f = posMod(f - 1, M + 1);
                }
                int up = matrix[f][j];
                f = posMod(i + 1, M + 1);
                while (true)
                {
                    if (matrix[f][j] > 0)
                        break;
                    f = posMod(f + 1, M + 1);
                }
                int down = matrix[f][j];
                Node node;
                node.up = up;
                node.down = down;
                node.left = -1;
                node.right = -1;
                node.row = i;
                node.top = j + 1;
                if (i == 0)
                {
                    node.left = posMod(j, N + 1);
                    node.right = posMod(j + 2, N + 1);
                    node.len = lenVec[j];
                }
                else
                {
                    node.len = -1;
                }
                nodes[matrix[i][j]] = node;
                nodeCount++;
            }
        }
    }
    nodes.resize(nodeCount + M + 2);
    // set the spacers
    int prev = -1;
    for (int i = N + 1; i < nodes.size(); i++)
    {
        if (nodes[i].top < 0)
        {
            if (prev >= 0)
            {
                nodes[i].up = prev + 1;
                nodes[prev].down = i - 1;
            }
            prev = i;
        }
    }
}

void DLXBuilder::cover(int col)
{
    int left = nodes[col].left;
    int right = nodes[col].right;
    nodes[right].left = left;
    nodes[left].right = right;

    int p = nodes[col].down;
    while (p != col)
    {
        hide(p);
        p = nodes[p].down;
    }
}

void DLXBuilder::hide(int p)
{
    int q = p + 1;
    while (q != p)
    {
        int x = nodes[q].top;
        if (x < 0) // q is a spacer
        {
            q = nodes[q].up;
            continue;
        }
        int up = nodes[q].up;
        int down = nodes[q].down;

        nodes[up].down = down;
        nodes[down].up = up;
        nodes[x].len = nodes[x].len - 1;
        q++;
    }
}

void DLXBuilder::uncover(int col)
{
    int p = nodes[col].up;
    while (p != col)
    {
        unhide(p);
        p = nodes[p].up;
    }
    int left = nodes[col].left;
    int right = nodes[col].right;
    nodes[left].right = col;
    nodes[right].left = col;
}

void DLXBuilder::unhide(int p)
{
    int q = p - 1;
    while (q != p)
    {
        int x = nodes[q].top;
        if (x < 0) // q is a spacer
        {
            q = nodes[q].down;
            continue;
        }
        int up = nodes[q].up;
        int down = nodes[q].down;

        nodes[up].down = q;
        nodes[down].up = q;
        nodes[x].len = nodes[x].len + 1;
        q--;
    }
}

void DLXBuilder::search(int x)
{
    int maxSolutions = 100;
    if (nodes[0].right == 0)
    {
        std::vector<int> currentSolution(solution.begin(), solution.begin() + x);
        allSolutions.push_back(currentSolution);
        std::cout << "Solution #" << allSolutions.size() << " found\n";
        return;
    }

    int col = chooseCol();
    cover(col);

    int r = nodes[col].down;
    while (r != col)
    {
        solution[x] = nodes[r].row;
        int j = r + 1;
        while (j != r)
        {
            if (nodes[j].top < 0) // spacer
            {
                j = nodes[j].up;
                continue;
            }
            cover(nodes[j].top);
            j++;
        }

        search(x + 1);

        int t = r - 1;
        while (t != r)
        {
            if (nodes[t].top < 0) // spacer
            {
                t = nodes[t].down;
                continue;
            }
            uncover(nodes[t].top);
            t--;
        }

        r = nodes[r].down;

        if (allSolutions.size() >= maxSolutions)
        {
            break;
        }
    }
    uncover(col);
}

// choose the first column with minimum length (MRV heuristic)
int DLXBuilder::chooseCol()
{
    int best = -1;
    int bestLen = numeric_limits<int>::max();

    for (int col = nodes[0].right; col != 0; col = nodes[col].right)
    {
        if (nodes[col].len < bestLen)
        {
            bestLen = nodes[col].len;
            best = col;
        }
    }
    return best;
}
