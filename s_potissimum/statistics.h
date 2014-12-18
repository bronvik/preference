#ifndef STATISTICS_H
#define STATISTICS_H

#include <vector>

using namespace std;

class statistics
{
public:
    statistics();

private:
    struct games
    {
        vector <char> name;

        int game;
        int win;
        int loss;
        float percent;

        int passes[11];
    };

    int load(void);
    void show(void);
    void clean(void);
};

#endif // STATISTICS_H
