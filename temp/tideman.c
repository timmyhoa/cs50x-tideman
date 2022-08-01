#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);
        printf("\n");
    }
    // preferences[0][0] = 0;
    // preferences[0][1] = 3;
    // preferences[0][2] = 1;
    // preferences[1][0] = 1;
    // preferences[1][1] = 0;
    // preferences[1][2] = 2;
    // preferences[2][0] = 3;
    // preferences[2][1] = 2;
    // preferences[2][2] = 0;
    add_pairs();
    sort_pairs();
    lock_pairs();
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count; j++)
        {
            printf("%i ", locked[i][j]);
        }
        printf("\n");
    }
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int j = 0; j < candidate_count; j++)
    {
        if (strcmp(candidates[j], name) == 0)
        {
            ranks[rank] = j;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i+1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // printf("j: %i, i: %i\n", j, i);
            if (preferences[i][j] == preferences[j][i])
            {
                continue;
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
            }
            pair_count++;
        }
    }
    // pair_count++;
    return;
}

// Sort pairs in decreasing order by strength of victory
// pairs sort(pair pairs, int length)
// {
//     int middle = (length - 1)/2

//     if (length == 1)
//     {
//         return pairs
//     }
//     else
//     {
//         pair left[length/2]
//         pair right[length/2]
//         for (int i = 0, n = middle; i <= n; i++)
//         {
//             left[i] = pairs[i];
//         }
//         for (int i = middle + 1, n = length; i < n; i++)
//         {
//             right[i] = pairs[i];
//         }
//         sort(left, length/2)
//         sort(right, length/2)

//     }
// }

void sort_pairs(void)
{
    // TODO
    int sorted = 1;
    int a = 0;
    pair temp;
    while (sorted != 0)
    {
        sorted = 0;
        for (int i = 0, n = pair_count - a - 1; i < n; i++)
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                temp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp;
                sorted += 1;
            }
        }
        a++;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    bool flag = false;
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        flag = false;
        for (int k = 0; k < candidate_count; k++)
        {
            int number_of_false = candidate_count;
            for (int j = 0; j < candidate_count; j++)
            {
                if (locked[j][k] == false)
                {
                    number_of_false--;
                }
                if (number_of_false == 0)
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
            {
                break;
            }
        }
    if (!flag)
    {
        locked[pairs[i].winner][pairs[i].loser] = false;
    }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int k = 0; k < candidate_count; k++)
    {
        int number_of_false = candidate_count;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][k] == false)
            {
                number_of_false--;
            }
            if (number_of_false == 0)
            {
                printf("%s\n", candidates[k]);
            }
        }
    }
}

