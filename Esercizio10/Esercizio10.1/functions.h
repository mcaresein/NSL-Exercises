void InitializeRandomNumberGenerator();
void Input();
void Mutate(RoadBook&, Sehenswurdigkeiten);
void Anneal(RoadBook&, Sehenswurdigkeiten);
void PrintBest(RoadBook, Sehenswurdigkeiten, std::string );
std::vector<int> GetDistanceSortIndexes(RoadBook& rdbk, Sehenswurdigkeiten cities);
std::vector<int> sort(const std::vector<double> &v);
