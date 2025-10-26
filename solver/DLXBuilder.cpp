#include "DLXBuilder.h"
#include <limits>
#include <algorithm>
#include <cassert>
#include <unordered_set>

vector<int> DLXBuilder::findSolution()
{
    build();
    search(0);
    std::cout << "Sol size: " << solutionSize << endl;
    solution.resize(solutionSize);
    validateSolutionDebug();
    return solution;
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
    if (solutionFound)
    {
        return;
    }
    if (nodes[0].right == 0)
    {
        solutionFound = true;
        solutionSize = x;
        std::cout << "\nSolution found! " << x << " rows: \n";
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
            if (nodes[j].top < 0)
            {
                j = nodes[j].up;
                continue;
            }

            cover(nodes[j].top);
            j++;
        }
        search(x + 1);
        // Backtrack
        int t = r - 1;
        while (t != r)
        {
            if (nodes[t].top < 0)
            {
                t = nodes[t].down;
                continue;
            }

            uncover(nodes[t].top);
            t--;
        }
        r = nodes[r].down;
        if (solutionFound)
        {
            return;
        }
    }
    uncover(col);
}

// choose the first column with minimum length (MRV heuristic)
int DLXBuilder::chooseCol()
{
    int best = -1;
    int bestLen = numeric_limits<int>::max();

    for (int c = nodes[0].right; c != 0; c = nodes[c].right)
    {
        if (nodes[c].len < bestLen)
        {
            bestLen = nodes[c].len;
            best = c;
        }
    }
    return best;
}

bool DLXBuilder::validateSolutionDebug()
{
    if (!solutionFound)
    {
        std::cout << "No solution.\n";
        return false;
    }

    int cols = matrix[0].size();
    std::vector<int> coverage(cols, 0);
    std::vector<std::vector<int>> byRow(cols);
    std::sort(solution.begin(), solution.end());
    for (int i = 0; i < solutionSize; i++)
    {
        int rowIdx = solution[i];
        if (rowIdx < 0 || rowIdx >= (int)matrix.size())
        {
            std::cout << "Virhe: solution sisältää rivin " << rowIdx << "\n";
            return false;
        }

        const auto &row = matrix[rowIdx];
        for (int j = 0; j < cols; j++)
        {
            if (row[j] > 0)
            {
                coverage[j]++;
                byRow[j].push_back(rowIdx);
            }
        }
    }

    bool ok = true;
    for (int j = 0; j < cols; j++)
    {
        if (coverage[j] != 1)
        {
            ok = false;
            if (coverage[j] == 0)
            {
                std::cout << "❌ Sarake " << j + 1
                          //   << " (" << constraintName(j)
                          << ") jäi kattamatta!\n";
            }
            else
            {
                std::cout << "❌ Sarake " << j + 1
                          //   << " (" << constraintName(j)
                          << " peitetty " << coverage[j] << " kertaa.\n";
                std::cout << "   Mukana rivit: ";
                for (int r : byRow[j])
                    std::cout << r << " ";
                std::cout << "\n";
            }
        }
    }

    if (ok)
    {
        std::cout << "✅ Sudoku-ratkaisu on validi exact cover!\n";
    }
    else
    {
        std::cout << "❌ Sudoku-ratkaisussa on ongelmia.\n";
    }

    return ok;
}
