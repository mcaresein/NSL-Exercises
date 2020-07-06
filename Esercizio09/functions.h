void InitializeRandomNumberGenerator();
void Input();
void GiveBirth(RoadBook&, Random&, Sehenswurdigkeiten);
void SaveBest(RoadBook rdbk, Sehenswurdigkeiten citieshape, std::string filename);
int  SumBestHalfDistances(RoadBook rdbk, Sehenswurdigkeiten citieshape, Statistics&, int);
std::vector<int> GetDistanceSortIndexes(RoadBook& rdbk, Sehenswurdigkeiten cities);
std::vector<int> Selection(Random& rnd, RoadBook& rdbk, Sehenswurdigkeiten cities);
std::vector<int> sort(const std::vector<double> &v);
