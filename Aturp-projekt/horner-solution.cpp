#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
//#include <windows.h>
#include <stdio.h>
//#include <tchar.h>

using namespace std;

struct AllocationMetrics
{
    uint32_t TotalAllocated = 0;
    uint32_t TotalFreed = 0;

    uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
};

static AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size)
{
    s_AllocationMetrics.TotalAllocated += size;

    return malloc(size);
}

void operator delete(void* memory, size_t size)
{
    s_AllocationMetrics.TotalFreed += size;

    free(memory);
}

static void PrintMemoryUsage()
{
    cout << "Memory usage " << s_AllocationMetrics.TotalAllocated << " bytes \n";
}

void PrintVector(vector<double>& vector)
{
    for (int i = 0; i < vector.size(); i++)
    {
        cout << vector[i] << endl;
    }
}

void PrintPairVector(vector<pair<double, double> >& vector)
{
    for (int i = 0; i < vector.size(); i++)
    {
        cout << vector[i].first << " -> " << vector[i].second << endl;
    }
}

vector<pair<int, int> >  DoubleToInteger(vector<pair<double, double> > results) {
    vector<pair<int, int> > roundResults;
    int x, y = 0;
    for (const auto& result : results) {
        x = int(result.first * 10);
        y = int(round(result.second * 10));
        //y = int(result.second * 10);
        roundResults.push_back(make_pair(x, y));
    }
    return roundResults;
}

int FindLowestY(vector<pair<int, int> >items) {
    int index = 0;
    for (int i = 1; i < items.size(); i++)
    {
        if (items[index].second > items[i].second)
            index = i;
    }
    return items[index].second;
}

int FindHighestY(vector<pair<int, int> >items) {
    int index = 0;
    for (int i = 1; i < items.size(); i++)
    {
        if (items[index].second < items[i].second)
            index = i;
    }
    return items[index].second;
}

bool IsFlatLine(vector<pair<int, int> >results)
{
    int index = 0;
    for (int i = 1; i < results.size(); i++)
    {
        if (results[index].second != results[i].second)
            return false;
    }
    return true;
}

bool IsPointAtGraph(vector<pair<int, int> > roundResults, int x, int y, vector<double>& coeffiecients, int n)
{
    double xDouble = (double)x; double yDouble = (double)y; //cast x and y to double
    double xDecimal = 0, yDecimal = 0;
    if (x != 0)
        xDecimal = xDouble / 10; //convert x to decimal
    if (y != 0)
        yDecimal = yDouble / 10; //convert y to decimal
    double equationResult = 0;
    if (n == 3)
        equationResult = coeffiecients[0] * pow(xDecimal, 3) + coeffiecients[1] * pow(xDecimal, 2) + coeffiecients[2] * xDecimal + coeffiecients[3];
    else if (n == 2)
        equationResult = coeffiecients[0] * pow(xDecimal, 2) + coeffiecients[1] * xDecimal + coeffiecients[2];
    else if (n == 1)
        equationResult = coeffiecients[0] * xDecimal + coeffiecients[1];
    else if (n == 0)
        equationResult = coeffiecients[0];

    int yResult = int(equationResult * 10); //cast again to integer
    if (yResult > y) //if the result from the equation is bigger than the original y from some coordinate on that interval T(x, y) than that point is ON or UNDER the graph
        return true;
    else
        return false;
}

void DrawOnePointGraph()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 1; j++)
            cout << ".";
        cout << endl;
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 1; j++)
            cout << "#";
        cout << endl;
    }
    exit(0);
}

void DrawFlatGraph(int intervalLength)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j <= intervalLength; j++)
            cout << ".";
        cout << endl;
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j <= intervalLength; j++)
            cout << "#";
        cout << endl;
    }
    exit(0);
}

void DrawSharpOrPoint(bool sharp, int intevalLength)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j <= intevalLength; j++)
        {
            if (sharp)
                cout << "#";
            else
                cout << ".";
        }
        cout << endl;
    }
}

void Draw(vector<pair<double, double> >results, int intervalLength, int rInterval, int lInterval, vector<double>& coeffiecients, int n) {
    vector<pair<int, int> > roundResults = DoubleToInteger(results);
    int yMin = FindLowestY(roundResults);
    int yMax = FindHighestY(roundResults);
    //cout<< yMin << ", "<< yMax << endl;

    int i = 0;
    int j = 0;

    if (roundResults.size() == 1)
        DrawOnePointGraph();
    if (IsFlatLine(roundResults))
        DrawFlatGraph(intervalLength);

    //for (i = yMax; i >= yMin; i--) {
    //    if (yMin > 0)
    //        yMin += 3;
    //    else
    //        yMin -= 3;
   //}

    DrawSharpOrPoint(false, intervalLength);
    for (i = yMax; i >= yMin; i--) {

        for (j = lInterval; j <= rInterval; j++) {
            if (IsPointAtGraph(roundResults, j, i, coeffiecients, n))
                cout << "#";
            else
                cout << ".";
        }
        cout << endl;
    }
    DrawSharpOrPoint(true, intervalLength);
}

void CalculatePolynom(double l, double r, int n, vector<double>& coeffiecients)
{
    int intervalLenght = round(abs(r - l) * 10);

    double equationResult;
    double x = l;
    vector<pair<double, double> >results;

    for (int i = 0; i <= intervalLenght; i++) {

        double tempRez = coeffiecients[0];

        for(int j = 1; j < coeffiecients.size(); j++) {
            tempRez = tempRez*x+coeffiecients[j];
        }

        equationResult = tempRez;

        results.push_back(make_pair(x, equationResult));

        x += 0.1;
    }

    Draw(results, intervalLenght, (r*10), (l*10), coeffiecients, n);

}

int main(int argc, char* argv[])
{
    //PrintMemoryUsage();

    ifstream infile(argv[1]);
    int a = 1;
    string l, r, n, polynomDegree;
    vector<double>coefficients;
    infile >> l >> r >> n;
    while (infile >> polynomDegree)
        coefficients.push_back(stod(polynomDegree));

    CalculatePolynom(stod(l), stod(r), stoi(n), coefficients);
    PrintMemoryUsage();
}
