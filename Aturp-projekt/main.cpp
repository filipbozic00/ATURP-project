#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

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

double ProcessDecimal(double y)
{
    stringstream yS; string wholePart; string decimalPart;
    yS << y;
    string s = yS.str();

    if (s.find('.') != string::npos)
    {
        int pos = s.find(".");
        wholePart = s.substr(0, pos);
        decimalPart = s.substr(s.find(".") + 1);

        for (int i = 0; i < decimalPart.length(); i++)
        {
            if (i > 0)
                decimalPart[i] = '0';
        }
        string rounded = wholePart + '.' + decimalPart;
        return stod(rounded);
    }
    return y;
}

void DrawGraph(vector<pair<double, double> >results, int intervalLength)
{
    ofstream outFile("output.txt");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < intervalLength; j++)
        {
            cout << ".";
        }
        cout << endl;
    }
    int lines = 0;
    //reverse(results.begin(), results.end());
    for (int i = 0; i < results.size() - 1; i++)
    {
        if (lines > intervalLength)
            break;
        results[i].second = ProcessDecimal(results[i].second);
        //results[i+1].second = ProcessDecimal(results[i+1].second);

        double cellsY = results[i].second - results[i + 1].second;
        cellsY *= 10;
        cellsY = abs(round(cellsY * 1000.0) / 1000.0); //number of lines with the same number of hastags(on one interval of 0.1)
        //cellsY = floor(cellsY);
        int cellsX = abs((results[0].first - results[i+1].first)*10); //number of hashtags horizontally

        for (int j = 0; j < cellsY; j++)
        {
            bool hastagWritten = false;
            for (int k = 0; k <= intervalLength; k++)
            {
                if (!hastagWritten)
                {
                    for (int l = 0; l < cellsX; l++)
                    {
                        cout << "#";
                        outFile << "#";
                        hastagWritten = true;
                        k++;
                    }
                }
                cout << ".";
                outFile << ".";
            }
            lines++;
            cout << endl;
            outFile << endl;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < intervalLength; j++)
        {
            cout << "#";
        }
        cout << endl;
    }
    outFile.close();
}

vector<pair<int, int> >  DoubleToInteger(vector<pair<double, double> > results) {
	vector<pair<int, int> > roundResults;
	int x, y = 0;
	for (const auto& result : results) {
		x = int(result.first * 10);
		y = int(result.second * 10);
		roundResults.push_back(make_pair(x, y));
		//cout<< x << "->" << y <<endl;
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
    // for (int k = roundResults.size() - 1; k >= 0; k--)
    //     if (i == roundResults[k].first && j == roundResults[k].second)
    //         return true;
    //return false;
    double xDouble = (double)x; double yDouble = (double)y; //cast x and y to double
    double xDecimal = 0, yDecimal = 0;
    if(x != 0) 
        xDecimal = xDouble / 10; //convert x to decimal
    if(y != 0) 
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

    //for(i = yMax; i>=yMin; i--) {
    //if (yMin > 0)
    //    yMin += 3;
    //else
    //    yMin -= 3;

    for(i = yMax; i>=yMin; i--) {

        for(j = lInterval; j<=rInterval; j++) {
            if(IsPointAtGraph(roundResults, j, i, coeffiecients, n))
                cout << "#";
            else
                cout << ".";

        }
        cout<<endl;
    }
}

void CalculatePolynom(double l, double r, int n, vector<double>& coeffiecients)
{
    int intervalLenght = abs(r - l) * 10;
    double equationResult;
    double x = l;
    vector<pair<double, double> >results;
  
    for (int i = 0; i <= intervalLenght; i++)
    {
        if (intervalLenght == 0 && coeffiecients.size() > 0)
        {
            equationResult = coeffiecients[0];
            results.push_back(make_pair(x, equationResult));
            break;
        }
        if (n == 3)
        {
            equationResult = coeffiecients[0] * pow(x, 3) + coeffiecients[1] * pow(x, 2) + coeffiecients[2] * x + coeffiecients[3];
        }
        else if (n == 2)
        {
            equationResult = coeffiecients[0] * pow(x, 2) + coeffiecients[1] * x + coeffiecients[2];
        }
        else if (n == 1)
        {
            equationResult = coeffiecients[0] * x + coeffiecients[1];
        }
        else if (n == 0)
        {
            equationResult = coeffiecients[0];
        }
        results.push_back(make_pair(x, equationResult));
        //if (i == intervalLenght-1)
        //    x += 0.01;
        //else
        x += 0.1;
    }

    //DrawGraph(results, intervalLenght);
    Draw(results, intervalLenght, (r*10), (l*10), coeffiecients, n);
    //return results;
}

int main(int argc, char* argv[])
//int main()
{
    //preberi vhodne podatke
    //ifstream infile("C:\\Users\\icovi\\OneDrive\\Desktop\\College\\2022-2023\\ATURP\\Seminarska_naloga\\Pot_v_f(x)\\Debug\\testne_primere\\12.in");
    ifstream infile(argv[1]);
    string l, r, n, polynomDegree;
    vector<double>coefficients;
    infile >> l >> r >> n;
    while (infile >> polynomDegree)
        coefficients.push_back(stod(polynomDegree));

    //PrintVector(coefficients);

    CalculatePolynom(stod(l), stod(r), stoi(n), coefficients);

}

